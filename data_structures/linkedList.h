#ifndef LIST_H
#define LIST_H

#ifndef EleType
#define EleType double
#endif

#ifndef EQ
#define EQ(a, b) (a == b)
#endif

#ifndef LT
#define LT(a, b) (a < b)
#endif

struct link {
	EleType value;
	struct link * next;
	struct link * prev;
};

struct linkedList {
	int size;
	struct link * frontSentinel;
	struct link * backSentinel;
};

void linkedListInit(struct linkedList * q);
void linkedListFree(struct linkedList * q);
void linkedListAddFront(struct linkedList * q, EleType e);
void linkedListAddBack(struct linkedList * q, EleType e);
void linkedListRemoveFront(struct linkedList * q);
void linkedListRemoveBack(struct linkedList * q);
int linkedListSize(struct linkedList *q);
int linkedListIsEmpty(struct linkedList * q);
EleType linkedListFront(struct linkedList * q);
EleType linkedListBack(struct linkedList * q);

/* the following make your list sort easier */
void _addLink(struct linkedList * q, struct link *lnk, EleType e);
void _removeLink(struct linkedList * q, struct link *lnk);

/* the following is optional, but make life easier */
void _moveLink(struct linkedList * q, struct link *from, struct link *to);
void listSort (struct linkedList * lst);

#endif
