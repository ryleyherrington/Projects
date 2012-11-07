/*Ryley Herrington
CS261 - Winter 2011
*/
#include "skipList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#if 1 // true
double getMilliseconds() {
	return (1000.0 * clock()) / CLOCKS_PER_SEC;
}
#else
double getMilliseconds() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec*1000.0 + (double)t.tv_usec/1000.0;
}
#endif

double randDouble(int includeUpperLimit) {
	double div = RAND_MAX;
	if(!includeUpperLimit)
		++div;
	return (rand()/div);
}

double timed(struct skipList * p, int M, int N, int isSorted){

	//old code
	int i,dice;
	double elapsed, x;
	//elapsed = -clock();
	elapsed = -getMilliseconds();
	for (i=0;i<N;i++){
		dice = (rand() % 100)+1;
		x = randDouble(100);
		if(dice<M){
			if (isSorted)
				sortedArrayAdd(p, x);
			else
				dyArrayPush(p, x);
		}
		else{
			dyArrayContains(p, x);
		}
	}
	//elapsed+=clock();
	elapsed+=getMilliseconds();
	return elapsed;
}

int main(int argc, char* argv[]){
	int N = 5000;
	int M = argc>1 ? atoi(argv[1]) : 10;
	int i, j, dice;
	double elapsed,sortedElapsed;

	printf("\t skipList\t\n");
	for (N=5000; N<=80000; N+=5000){
		struct skipList stk;
		skipListInit(&stk);
		for(i=0; i<(N/2); i++){
			skipListAdd(&stk, randDouble(100));
		}
		elapsed = timed(&stk, M, N, 1);  // It's sorted by definition, so 1= sorted
		//For debugging purposes only
		//printf("For unsorted array: %lf\n", elapsed);

		printf("%d\t %lf \t \n", N,elapsed);
	}
        return 0;
}
