#ifndef FRAME_H
#define FRAME_H

int frame_size; //for counting frames
int frame_capacity; //for frame memory management

typedef struct Frame{
    int32_t *local_vars;
    int var_size;
    int previous_frame;
    int previous_stack;
}Frame;

Frame *frames; //struct array for handling frames

Frame make_frame(int program_counter, int local_var_size, int stack, int numberOfArguments); //method for making frames and dealing with memory management


#endif
