# ifndef DyArrayH
# define DyArrayH

/* Dynamic Array Stack (Header)
   Winter 2010
   CS261
*/

# ifndef EleType
# define EleType double
# endif

# ifndef EQ
# define EQ(a, b) (a == b)
# endif

struct dyArray { /* dyArray Structure */
       EleType *data;
       int size;
       int capacity;
};

/* Prototypes */
void dyArrayInit(struct dyArray *da, int initialCapacity);
void dyArrayFree (struct dyArray *da);
void dyArrayInsert (struct dyArray *da, int index, EleType e);

/* stack operations interface */
void dyArrayPush(struct dyArray *da, EleType e);
void dyArrayPop(struct dyArray *da);
EleType dyArrayTop(struct dyArray *da);
int dyArrayIsEmpty(struct dyArray *da);

/* bag operations interface */
void dyArrayAdd (struct dyArray *da, EleType d);
int dyArraySize (struct dyArray *da);
void dyArrayRemove (struct dyArray *da, EleType d);
int dyArrayContains (struct dyArray *da, EleType e);

/* array operations interface */
EleType dyArrayGet (struct dyArray *da, int index);
void dyarraySet (struct dyArray *da, int index, EleType newValue);

/* sorted array operations */
void sortedArrayAdd (struct dyArray *da, EleType newValue);
int sortedArrayContains (struct dyArray *da, EleType testValue);
void sortedArrayRemove (struct dyArray *da, EleType testValue);

# endif
