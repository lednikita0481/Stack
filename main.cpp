#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "stack.h"

int main()
{
    Stack stk1 = {};

    StackCtor(&stk1, 5);
    StackPush(&stk1, 3.14);
    StackPush(&stk1, 2.7);
    StackPush(&stk1, 3.14);
    StackPush(&stk1, 2.7);
    StackPush(&stk1, 3.14);
    StackPush(&stk1, 2.7);
 


    int err = 0;
    Elem_t el = StackPop(&stk1);
    printf("el = %lf\n", el);
    el = StackPop(&stk1);
    printf("el = %lf\n", el);
    el = StackPop(&stk1);
    printf("el = %lf\n", el);
    el = StackPop(&stk1);
    printf("el = %lf\n", el);
    el = StackPop(&stk1);
    printf("el = %lf\n", el);
    el = StackPop(&stk1);
    printf("el = %lf\n", el);
    el = StackPop(&stk1);
    printf("el = %lf\n", el);
    el = StackPop(&stk1);
    printf("el = %lf\n", el);
    el = StackPop(&stk1);
    printf("el = %lf\n", el);
    el = StackPop(&stk1);
    printf("el = %lf\n", el);

}
