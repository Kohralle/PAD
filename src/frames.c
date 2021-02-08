#include <ijvm.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "frames.h"

Frame make_frame(int program_counter, int local_var_size, int stack, int numberOfArguments){
    Frame frame;
    int capacity = 10; //same concept as frame_capacity in machine.c

    frame.var_size = local_var_size;
    frame.previous_stack = stack;
    frame.previous_frame = program_counter;

    frame.local_vars = (int32_t*) malloc(capacity * sizeof(int32_t));

    if(numberOfArguments > frame.var_size){
       capacity *= 2;
       frame.local_vars = (int32_t*) realloc(frame.local_vars, capacity * sizeof(int32_t));
   }

    return frame;
}
