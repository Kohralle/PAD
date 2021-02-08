#include <ijvm.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stack.h"

void stack_push(int32_t data) {
    Stack.contents = (int32_t *)realloc(Stack.contents, (Stack.size+2)*sizeof(int32_t));
    Stack.contents[Stack.size] = data;
    Stack.size++;
}

int32_t stack_pop() {
    int32_t pop_var;
    pop_var = Stack.contents[Stack.size - 1];
    Stack.contents = (int32_t *)realloc(Stack.contents, (Stack.size)*sizeof(int32_t));
    Stack.size--;
    return pop_var;
}

int32_t stack_top() {
  return Stack.contents[Stack.size-1];
}

int32_t stack_size_info () { //declared for the sake of implementation as the given function would fail tests
  return Stack.size-1;
}
