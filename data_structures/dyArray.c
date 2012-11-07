/* Ryley Herrington
   Winter 2010
   CS261
*/
#include "dyArray.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void dyArrayInit(struct dyArray *da, int initialCapacity){
	assert(da);
	da->capacity = initialCapacity;
	assert (initialCapacity> 0);
	da->size = 0;
	da->data = (EleType *) malloc(da->capacity * sizeof(EleType));
	assert (da->data != 0);
}
void dyArrayFree (struct dyArray *da){
	assert(da);
	free(da->data);
	da->data = 0;
	da->capacity = 0;
	da->size = 0;
}
void dyArrayInsert (struct dyArray *da, int index, EleType e){
	assert(da);
	int i;
	for (i = (da->size-1); i>=index; i--){
		da->data[i+1] = da->data[i];
	}
	da->data[index] = e;
	da -> size++;
}

void _dyArrayDoubleCapacity (struct dyArray * da) {
	assert(da);
	EleType * oldbuffer = da->data;
	int oldsize = da->size;
	int i;
	dyArrayInit (da, 2 * da->capacity);
	for (i = 0; i < oldsize; i++)
		da->data[i] = oldbuffer[i];
	da->size = oldsize;
	free(oldbuffer);
}

/* stack operations interface */
void dyArrayPush(struct dyArray *da, EleType e){
	assert(da);
	dyArrayAdd(da,e);
}

void dyArrayPop(struct dyArray *da){
	assert(da);
	da->size--;
}

EleType dyArrayTop(struct dyArray *da){
	assert(da);
	assert(da->size >0); // checking if the size is greater than zero
	return da->data[da->size-1];
}

int dyArrayIsEmpty(struct dyArray *da){
	assert(da);
	return da->size == 0; // returns true if size is zero
}

/* bag operations interface */
void dyArrayAdd (struct dyArray *da, EleType d){
	assert(da);
	if (da->size >= da->capacity)
		_dyArrayDoubleCapacity(da);
	da->data[da->size] = d;
	da -> size += 1;
}

int dyArraySize (struct dyArray *da){
	assert(da);
	return da->size;
}
void dyArrayRemove (struct dyArray *da, EleType d){
	assert(da);
	assert (da->size > 0);
	int i,j;
	for(i=0; i<da->size; i++){
		if(da->data[i] == d){
			for(j=i; j<da->size; j++){
				da->data[j] = da->data[j+1];
			}
			da->size--;
			return;
		}
	}
}
int dyArrayContains (struct dyArray *da, EleType e){
	assert(da);
	if(da->size== 0)
		return 0;
	int i;
	for(i=0;i<da->size;i++){
		if(da->data[i] == e){
			return 1; // true
		}
	}
	return 0; // false
}

/* array operations interface */
EleType dyArrayGet (struct dyArray *da, int index){
	assert(da);
	assert(da-> size > index);
	return da-> data[index];
}
void dyarraySet (struct dyArray *da, int index, EleType newValue){
	assert(da);
	assert(da->size > index);
	da->data[index] = newValue;
}

/* sorted array operations */
void sortedArrayAdd (struct dyArray *da, EleType newValue){

       int i;

       // since we're adding a node, make sure there's enough storage
       if (da->size >= da->capacity)
               _dyArrayDoubleCapacity(da);

       // If array is empty, we insert at data[0]
       // If element is larger than anything in array, we insert at data[size]
       for (i=0;i<da->size;i++){
               if (newValue < da->data[i]){
                       break;
               }
       }
       dyArrayInsert(da, i, newValue);
}

//defining the compare function for the bsearch call in sortedArrayContains

int 
compare(const void* p_key, const void* p_dataItem)
{
	
        const EleType *key = p_key;
        const EleType *dataItem = p_dataItem;
	return *key - *dataItem;
}

int sortedArrayContains(struct dyArray *da, EleType testValue)
{
	//dyArrayContains(da, testValue);
	// may need to cast v to int (int)
	return (bsearch(&testValue, da->data, da->size, sizeof(EleType), compare) != 0);
}

void sortedArrayRemove (struct dyArray *da, EleType testValue){
	dyArrayRemove(da, testValue);
	/*bsearch array for testValue, which returns testValue pointer
	 * then subtract data[0] then implement the last parts
	 * of remove in here (useful for large arrays
	 */
}

