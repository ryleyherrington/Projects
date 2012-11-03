
/*
 *  sieve - generate all prime number less than "N"
 *
 *  To reduce memory contention, rather than have multiple threads
 *  mark a particular value for the entire length of the sieve,
 *  this implementation makes each thread responsible for a
 *  a fraction (1/nthreads) of the sieve.
 *
 *  To reduce DTLB and cache misses, this version uses a compressed
 *  compressed bitmap for the main sieve data structure.  Each value
 *  is represented by a single bit, and even numbers are skipped, so
 *  each byte records 16 values.  0 means "prime", 1 means not prime.
 *
 *  Because any given byte in this sieve is "owned" by one
 *  thread, there is no chance for contention and the data does
 *  not have to be locked.
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <pthread.h>

#include <vector>

typedef unsigned long u_val_t;
typedef unsigned char ubyte_t;

/**********************************************************
 * Functional replacement for calloc()
 **********************************************************
 */
ubyte_t* zmalloc(size_t size) {
    ubyte_t* retval = (ubyte_t*)malloc(size);
    if (retval == NULL)
        perror("malloc"), exit(-1);
    memset(retval, 0, size);
    return retval;
}


/******************************************************************************
 * "master" sieve data
 *
 * This will be a comparatively small set of primes used to sieve the full 
 * set we are searching for.  This will be pre-computed by the main thread 
 * and then read/used by each worker thread.
 *
 * The general "sieve" algorithm is to find each prime number and then mark off
 * all its multiples less than the limit "N".  Fortunately, we only need to mark
 * off multiples of primes up through sqrt(N).  Consider any non-prime number
 * which is (by definition) the product of at least two primes factors.  At least
 * one of these prime factors *must* be <= sqrt(N), or their product would be
 * greater than N (since sqrt(N)*sqrt(N) == N).  Therefore, every non-prime less
 * than N will be marked off by the smaller factor (which is less than sqrt(N), 
 * making it unnecessary to mark off the multiples of the larger factor.
 *
 * Therefore, for a 4 billion element sieve, we only need to mark off the
 * multiples of the primes up through 64K, which takes less than a second.
 ******************************************************************************
 */


// Limit of numbers we are searching for primes (for example, 4B
u_val_t max_prime = 0;

// "master" sieve data for sqrt(max_prime) elements
u_val_t max_master_prime = 0;
ubyte_t *master_map = NULL;

u_val_t set_up_master_map() {
    u_val_t max = (u_val_t)sqrt((double)max_prime);
    if (max * max < max_prime)
        max++;

    master_map = zmalloc(max/2 + 1);

    for (int i=3; i<max; i+=2) {
        if (master_map[(i-1)/2] == 0) { //if i is prime...
            u_val_t incr = i+i;
            for (u_val_t j=i+incr; j<max; j+=incr)
                master_map[(j-1)/2] = 1;
        }
    }
    return max;
}

/**********************************************************
 * "sieve" data ("max_prime" elements)
 **********************************************************
 */
   
// Each thread will be responsible for range [low_bound .. high_bound)
struct chunk_data {
    u_val_t low_bound;
    u_val_t high_bound;
};


// Array of "max_prime/16" bytes to represent "max_prime" values.
ubyte_t *bitmask = 0;

pthread_mutex_t lock = {0};
u_val_t last_value = 0; 
u_val_t chunk_size = 0; 

bool next_chunk(chunk_data *chunk_bounds) {
    
    bool retval = true;

    pthread_mutex_lock(&lock);
    if (last_value >= max_prime)
         retval = false; 
    else {
        chunk_bounds->low_bound  = last_value;
        chunk_bounds->high_bound = last_value + chunk_size;
        if (chunk_bounds->high_bound > max_prime)
            chunk_bounds->high_bound = max_prime;
        last_value = chunk_bounds->high_bound; 
    }
    pthread_mutex_unlock(&lock);
    
    return retval;
}


// Generate next prime by iterating through odds and avoiding
// any number which has been identified as non-prime.
u_val_t next_prime(u_val_t curr_prime) {
    assert((curr_prime & 0x01) == 1);
    while (curr_prime < max_master_prime) {
        curr_prime += 2;
        if (master_map[(curr_prime-1)/2] == 0)//if curr_prime is a prime number
            return curr_prime;
    }
    return 0;
}

u_val_t byte_pos(u_val_t v) { return (v/16);     }  // v>>4
int     bit_pos(u_val_t v)  { return (v%16)/2;   }  // (v&0x0F) >> 1

// Some compilers insist on a "C" prototype for this arg to pthread_create()
extern "C" void* do_sieve(void*);

/*
 * do_sieve() - Mark all non-primes within range [low_bound .. high_bound)
 */
void *do_sieve(void*v) {
    struct chunk_data data;
    
    while (next_chunk(&data) == true) {

        u_val_t curr_prime = 1;
        while ( (curr_prime=next_prime(curr_prime)) != 0) {
    
            // Find multiple of "curr_prime" within this thread's range
            u_val_t lo  = (data.low_bound/curr_prime) * curr_prime;
            if (lo < data.low_bound)   // make sure we're within the legal range
                lo += curr_prime;
            if ((lo & 0x01) == 0)       // make sure we're starting on an odd value
                lo += curr_prime;
            u_val_t incr = curr_prime + curr_prime;
    
            if (lo == curr_prime)       // only mark multiples - not the original
                lo += incr;
            while (lo < data.high_bound) {
                u_val_t byte_index = byte_pos(lo);
                assert(bit_pos(lo) < 8);
                bitmask[byte_index] |= (1 << bit_pos(lo));
                // fprintf(stderr, "thr=%d prime = %4lu, cross out %lu, byte_index = %lu  bit_index = %d  bit_mask = 0x%02x  byte_value = 0x%02x\n",
                // data.thread_num, curr_prime, lo, byte_index, bit_pos(lo), 1<<bit_pos(lo), (ubyte_t)bitmask[byte_index]);
                lo += incr;
            }
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    time_t start = time(0);
    
    //clock_t start2; /* start time for measuring program performance */
    //clock_t stop2; /* stop time for measuring program performance */
    //double total_time; /* stop time for measuring program performance */
    //start2 = clock();

    if (argc>1 && strcmp(argv[1],"--help") == 0) {
        printf("usage: %s [num_threads [max_prime [chunk_size]]]\n", argv[0]);
        exit(-1);
    }
    int numthr   = argc>1 ? atoi(argv[1]) : 4;//sysconf 
    max_prime    = argc>2 ? strtoul(argv[2], NULL, 10) : 4294967295UL;
    chunk_size   = argc>3 ? strtoul(argv[3], NULL, 10) : 50000000;
    bool doprint = argc<=4;//can compare if you choose to

    if (chunk_size < 16) {
        fprintf(stderr, "chunk size is too small\n");
        exit(-1);
    }
    chunk_size &= ~0xF;

    pthread_attr_t attr;
    /* initialize thread attribute with defaults */
    pthread_attr_init(&attr);

    pthread_mutex_init(&lock, NULL);

    // Set up the "master" map which controls the full-size sieve
    max_master_prime = set_up_master_map();
    //this checks if it's odd or even basically
    //if the upper bound is even, we're just backing up by one because 
    //no even number will be prime
    if ((max_prime & 0x1) == 0)  // avoids overflow on a 32-bit system
        max_prime--;

    // Each thread must handle a multple of 16 (since 16 elements per byte)
    bitmask = zmalloc(max_prime/16 + 1);

    // Create a thread to handle each subsection of the bitmap
    std::vector<pthread_t> tid(numthr);
    for (int i=0; i<numthr; i++)
        pthread_create(&tid[i], &attr, do_sieve, NULL);
    for (int i=0; i<numthr; i++)
        pthread_join(tid[i], NULL);
    pthread_mutex_destroy(&lock);
    time_t stop = time(0);

    /* record stop time */
    //stop2 = clock();
    /* calculate total time */
    //total_time = (double)(stop2 - start2) / (double)CLOCKS_PER_SEC;
    //fprintf(stderr, "New time:%.6f seconds\n", total_time);

    fprintf(stderr, "Time to do everything except print = %lu seconds\n", (u_val_t)(stop-start));

    // Iterate through the bitmap to print out the primes
    if (doprint) {
        printf("2\n");
        for (u_val_t i=3; i<max_prime; i+=2) {
            if ( (bitmask[byte_pos(i)] & (1<<bit_pos(i))) == 0)
                printf("%lu\n", i);
        }
    }
    return 0;
}
