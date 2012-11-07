/* Ryley Herrington
   Winter 2010
   CS261
 */
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void linkedListInit(struct linkedList * q){
	assert(q);
	q->frontSentinel = (struct link *) malloc(sizeof(struct link));
	assert(q->frontSentinel != 0);
	q->backSentinel = (struct link *) malloc(sizeof(struct link));
	assert(q->backSentinel);
	q->frontSentinel->next = q->backSentinel;
	q->backSentinel->prev = q->frontSentinel;
	q->size = 0;
}
void linkedListFree(struct linkedList * q){
	assert(q);
	while (q->size > 0)
		linkedListRemoveFront(q);
	free (q->frontSentinel);
	free (q->backSentinel);
	q->frontSentinel = q->backSentinel = 0;
}
void linkedListAddFront (struct linkedList *q, EleType e){
	assert(q);
	_addLink(q, q->frontSentinel, e);
}
void linkedListAddBack (struct linkedList *q, EleType e){
	assert(q);
	_addLink(q, q->backSentinel->prev, e);
}
void linkedListRemoveFront (struct linkedList *q) {
	assert(q);
	assert(! linkedListIsEmpty(q));
	_removeLink(q, q->frontSentinel->next);
}
void linkedListRemoveBack (struct linkedList *q) {
	assert(q);
	assert(!linkedListIsEmpty(q));
	_removeLink (q, q->backSentinel->prev);
}
int linkedListSize(struct linkedList *q){
	assert(q);
	return q->size;
}
int linkedListIsEmpty(struct linkedList * q){
	assert(q);
	return q->size ==0;
}
EleType linkedListFront(struct linkedList * q){
	assert(q);
	return q->frontSentinel->next->value;
}
EleType linkedListBack(struct linkedList * q){
	assert(q);
	return q->backSentinel->prev->value;
}

/* the following make your list sort easier */
void _addLink(struct linkedList * q, struct link *lnk, EleType e){
	assert(q);
	struct link *new = (struct link *) malloc(sizeof(struct link));
	new->value = e;
	lnk->next->prev = new;
	new->next = lnk->next;
	lnk->next = new;
	new->prev = lnk;
	q->size++;
}
void _removeLink(struct linkedList * q, struct link *lnk){
	assert(q);
	assert(lnk);
	lnk->prev->next = lnk->next;
	lnk->next->prev = lnk->prev;
	lnk->next = 0;
	lnk->prev = 0;
	q->size--;
}
/* the following is optional, but makes life easier */
void _moveLink(struct linkedList * q, struct link *from, struct link *to){
	assert(q);
	assert(from);
	assert(to);
	_removeLink(q,from);
	_addLink(q,to,from->value);
	free(from);

}

void listSort (struct linkedList * lst){
	struct link * current;
	current = lst->frontSentinel->next;
	while(current != lst->backSentinel){
		struct link * nextNode = current->next;
		struct link * prevNode = current-> prev;
		if (current->value < prevNode->value){
			while(current->value < prevNode->value&& prevNode!=lst->frontSentinel){
				prevNode = prevNode->prev;
			}
			_addLink(lst,prevNode, current->value);
			_removeLink(lst, current);
			free(current);
		}
		current = nextNode;
	}
}
