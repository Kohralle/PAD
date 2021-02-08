#ifndef STACK_H
#define STACK_H

struct stack {
    int32_t *contents;
    int32_t size;
};

struct stack Stack;

void stack_push(int32_t data);

int32_t stack_pop();

int32_t stack_top();

int32_t stack_size_info ();

#endif
