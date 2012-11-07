
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <time.h>

void printNode(struct linkedList *q, struct link*p){
	if (p == q->frontSentinel)
	    printf("FRONT SENTINEL   \t &node=%12p \t prev=%12p \t next=%12p\n", p, p->prev, p->next);
	else if (p == q->backSentinel)
	    printf("BACK SENTINEL    \t &node=%12p \t prev=%12p \t next=%12p\n", p, p->prev, p->next);
	else 
	    printf("val = %10f \t &node=%12p \t prev=%12p \t next=%12p\n", p->value, p, p->prev, p->next);
}

void printList(struct linkedList*q){
	struct link *p = q->frontSentinel;
	while (p != 0) {
	    printNode(q, p);
	    p = p->next; 
	}
}

double getMilliseconds() {
   return 1000.0 * clock() / CLOCKS_PER_SEC;
}

double
measure_sort_time(struct linkedList *llist)
{
	double elapsed = -getMilliseconds(); // start timing
	listSort(llist);
	elapsed += getMilliseconds(); // end timing
        return elapsed;
}

int main(int argc, char* argv[])
{
	int testlen = argc>1 ? atoi(argv[1]) : 5000;
	int i;
	double elapsed;
	struct link *curr;
	struct linkedList random_llist, ascending_llist, descending_llist; // structs on the stack
	
        /*
         *  Create three lists: random, ascending, descending
         */
	linkedListInit(&random_llist);
	linkedListInit(&ascending_llist);
	linkedListInit(&descending_llist);
	srand48(19910418);  //srand48(time(0));
	for (i=0; i<testlen; i++) {
	    linkedListAddBack(&random_llist, (drand48()*testlen));  /* random range of 0..testlen */
	    linkedListAddBack(&ascending_llist, i);
	    linkedListAddFront(&descending_llist, i);
	}

        /*
         *  Measure time for sorting the random list
         */
	elapsed = measure_sort_time(&random_llist);
	printf("Sort time for %d randomly ordered elements    = %lf ms\n", testlen, elapsed);
	linkedListFree(&random_llist);

        /*
         *  Measure time for sorting list in ascending order
         */
	elapsed = measure_sort_time(&ascending_llist);
	printf("Sort time for %d elements in ascending order  = %lf ms\n", testlen, elapsed);
	linkedListFree(&ascending_llist);

			
        /*
         *  Measure time for sorting list in descending order
         */
	elapsed = measure_sort_time(&descending_llist);
	printf("Sort time for %d elements in descending order = %lf ms\n", testlen, elapsed);
	linkedListFree(&descending_llist);

	return 0;
}
