  # include "dyArray.h"
# include <stdio.h>
# include <stdlib.h>
# include <assert.h>
# include <time.h>

#if 0
double getMilliseconds() {
	return (1000.0 * clock()) / CLOCKS_PER_SEC;
}
#else
double getMilliseconds()
{
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

double timed(struct dyArray * p, int M, int N, int isSorted){
	int i,dice;
	double elapsed, x;
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
			if (isSorted)
				sortedArrayContains(p, x);
			else
				dyArrayContains(p, x);
		}
	}
	elapsed+=getMilliseconds();
	return elapsed;
}

int main(int argc, char* argv[]){
	int N = 5000;
	int M = argc>1 ? atoi(argv[1]) : 10;
	int i, j, dice;
	double elapsed,sortedElapsed;

	printf("\t dyArray\t sorted\n");
	for (N=5000; N<=80000; N+=5000){
		struct dyArray stk;
		dyArrayInit(&stk,N/2);
		for(i=0; i<(N/2); i++){
			dyArrayPush(&stk, randDouble(100));
		}
		elapsed = timed(&stk, M, N, 0);  // 0 == is NOT sorted
		dyArrayFree(&stk);

		//For debugging purposes only
		//printf("For unsorted array: %lf\n", elapsed);

		struct dyArray sorted;
		dyArrayInit(&sorted, N/2);
		for(i=0; i<(N/2); i++){
			sortedArrayAdd(&sorted, randDouble(100));
		}
		sortedElapsed = timed(&sorted,M,N, 1);   // 1 == isSorted
		//For debugging purposes only
		//printf("For sorted array: %lf\n", sortedElapsed);

		printf("%d\t %lf \t %lf \n", N,elapsed, sortedElapsed);
	}
        return 0;
}
