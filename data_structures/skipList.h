/*
 ============================================================================
 Name        : skipList.h
 Author      : Tim Budd
 Description : Skip List Data Structure Header File
 ============================================================================
 */

#ifndef SKIPLIST_H_
#define SKIPLIST_H_

#ifndef EleType
#define EleType double
#endif

#ifndef EQ
#define EQ(a, b) (a == b)
#endif

#ifndef LT
#define LT(a, b) (a < b)
#endif

struct skipLink{
		EleType value;
		struct skipLink * next;
		struct skipLink * down;
};

struct skipList{
	struct skipLink * topSentinel;
	int size;
};

/* the public interface */

void skipListInit(struct skipList *slst);
void skipListAdd(struct skipList *slst, EleType d);
int skipListContains(struct skipList *slst, EleType d);
void skipListRemove(struct skipList *slst, EleType d);
int skipListSize(struct skipList *slst);
void skipListPrint(struct skipList *slst);

/* internal routines */
int skipFlip();
struct skipLink * slideRight(struct skipLink * current, EleType d);
struct skipLink * skipLinkAdd(struct skipLink * current, EleType d);
struct skipLink * newSkipLink(EleType d, struct skipLink * nlnk, struct skipLink* dlnk);

#endif /* SKIPLIST_H_ */
