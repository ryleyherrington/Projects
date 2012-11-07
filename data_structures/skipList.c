/*Ryley Herrington
CS261 - Winter
 */

#include "skipList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void skipListInit(struct skipList *slst){
	assert(slst);
	slst->topSentinel = (struct skiplink *) malloc(sizeof(struct skiplink));
	assert(slst->topSentinel != 0);
	slst->topSentinel->next = 0;
	slst->size = 0;
}
void skipListAdd(struct skipList *slst, EleType d){
	struct skipLink *downLink, *newLink;
	downLink = skipLinkAdd(slideRight(slst->topSentinel,d),d);
	if (downLink != 0 && skipFlip()) {
		newLink = newSkipLink(d, 0, downLink);
		slst->topSentinel = newSkipLink(0, newLink, slst->topSentinel);
	}
	slst->size++;
}
int skipListContains(struct skipList *slst, EleType d){
	struct skipLink *tmp = slst->topSentinel;
	while (tmp != 0) {
		tmp = slideRight(tmp, d);
		if ((tmp->next != 0) && EQ(d, tmp->next->value))
			return 1;
		tmp = tmp->down;
	}
	return 0;

}
void skipListRemove(struct skipList *slst, EleType d){
	struct skipLink *current, *tmp;
	current = slst->topSentinel;
	while (current != 0) {
		current = slideRight(current, d);
		if (current->next != 0 && EQ(d, current->next->value)) {
			tmp = current->next;
			current->next = current->next->next;
			free(tmp);
			if (current->down != 0)
				slst->size--;
		}
		current = current->down;
	}
}
int skipListSize(struct skipList *slst){
	return slst->size;
}
void skipListPrint(struct skipList *slst){
	struct skipLink* current = slst->topSentinel;
	while (current != 0){
		printf("%lf\n",current->value);
	}
	if(current->down!=0){
		printf("%lf\n",current->down->value);
	}
}

/* internal routines */
int skipFlip(){
	return rand() % 2;
}
struct skipLink * slideRight(struct skipLink * current, EleType testValue) {
	// Finds the link RIGHT BEFORE the place the value we want should be, if it is there
	while ((current->next != 0) && LT(current->next->value, testValue))
		current = current->next;
	return current;
}

struct skipLink * skipLinkAdd(struct skipLink * current, EleType d){
	struct skipLink *newLink, *downLink;
	newLink = 0;
	if (current->down == 0) {
		newLink = newSkipLink(d, current->next, 0);
		current->next = newLink;
	}
	else {
		downLink = skipLinkAdd(slideRight(current->down, d), d);
		if (downLink != 0 && skipFlip()) {
			newLink = newSkipLink(d, current->next, downLink);
			current->next = newLink;
		}
	}
	return newLink;
}
struct skipLink * newSkipLink(EleType d, struct skipLink * nlnk, struct skipLink* dlnk){
	struct skipLink * tmp = (struct skipLink *) malloc(sizeof(struct skipLink));
	assert(tmp != 0);
	tmp->value = d;
	tmp->next = nlnk;
	tmp->down = dlnk;
	return tmp;
}
