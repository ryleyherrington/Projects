# include "dyArray.h"

void stackSort (int n, double * data) {
   double x;
   struct dyArray stk;
   int i = 0;

   dyArrayInit(&stk, 3);

   while (i < n) {
      x = data[i++];
      while ((!dyArrayIsEmpty(&stk)) && dyArrayTop(&stk) > x) {
         data[--i] = dyArrayTop(&stk);
	 dyArrayPop(&stk);
      }
      dyArrayPush(&stk, x);
   }

   /* now everything should be on the stack in reverse order */
   while (i > 0) {
      data[--i] = dyArrayTop(&stk);
      dyArrayPop(&stk);
   }
}
