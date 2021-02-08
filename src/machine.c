#include <ijvm.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "functions.h"
#include "stack.h"
#include "operations.h"
#include "frames.h"

void initialize_components() {
    //initialize stack
    Stack.size = 0;
    Stack.contents = (int32_t *)malloc((Stack.size)*sizeof(int32_t));

    //initialize frames
    frame_size = 0;
    frame_capacity = 10; //I allocate 10 * variable type at first and enlarge it when capacity is exceeded
    frames = (Frame *) malloc(frame_capacity * sizeof(Frame));
    frames[frame_size] = make_frame(0,0,0,0);

    
    //Initiliaze socket array for network BONUS
    socket_array = (int *)malloc((10)*sizeof(int));
    socket_counter = 0;

    program_counter = 0;
}

int init_ijvm(char *binary_path){

    FILE *data_buffer;
    data_buffer = fopen(binary_path, "rb");

    FILE *temp_out_file = tmpfile();
    set_output(temp_out_file);

    data.magic_number = read_data(data_buffer);
    data.constant_origin = read_data(data_buffer);
    data.constant_size = read_data(data_buffer);

    data.constant_values = (uint32_t *) malloc(data.constant_size);

    for(int i = 0; i < data.constant_size / 4; i++){
        data.constant_values[i] = read_data(data_buffer);
    }

    data.text_origin = read_data(data_buffer);
    data.text_size = read_data(data_buffer);

    data.text_values = (uint8_t *) malloc(data.text_size);
    fread(data.text_values, sizeof(uint8_t), data.text_size, data_buffer);

    fclose(data_buffer);
    initialize_components();

    return 0;
}


word_t tos(){
    return stack_top();
}

word_t get_local_variable(int i){
    return frames[frame_size].local_vars[i];
}

word_t get_constant(int i){
    return data.constant_values[i];
}

int stack_size(){
    return Stack.size;
}

word_t *get_stack(){
    return &Stack.contents[0];
}

byte_t *get_text(){
    return data.text_values;
}

int text_size (){
    return data.text_size;
}

byte_t get_instruction(){
    return data.text_values[program_counter];
}

int get_program_counter(){
    return program_counter;
}

bool finished(){
    if(program_counter == data.text_size)
        return 1;
    else if (data.text_values[program_counter] == OP_HALT || data.text_values[program_counter] == OP_ERR)
        return 1;
    else
        return 0;
}

void run(){
    while (step());
}

void destroy_ijvm()
{
    free(data.constant_values);
    free(data.text_values);
    free(Stack.contents);
}

void set_input(FILE *fp)
{
    in = fp;
}

void set_output(FILE *fp)
{
    out = fp;
}

bool step (){

    if (finished()){
        return 0;
    }

    else{

        switch (get_instruction()){
//------------------TASK 2------------------//
            case OP_BIPUSH:{
            op_bipush();
            break;
            }

            case OP_IADD:{
            op_iadd();
            break;
            }

            case OP_ISUB:{
            op_isub();
            break;
            }

            case OP_IAND:{
            op_iand();
            break;
            }

            case OP_IOR:{
            op_ior();
            break;
            }

            case OP_SWAP :{
            op_swap();
            break;
            }

            case OP_DUP:{
            op_dup();
            break;
            }

            case OP_POP:{
            stack_pop();
            break;
            }

            case OP_NOP:{
            //Do nothing
            break;
            }

            case OP_IN:{
            op_in();
            break;
            }

            case OP_OUT:{
            op_out();
            break;
            }

//------------------TASK 3------------------//

            case OP_GOTO:{
            op_goto();
            break;
            }

            case OP_IFEQ:{
            op_ifeq();
            break;
            }

            case OP_IFLT:{
            op_iflt();
            break;
            }

            case OP_ICMPEQ:{
            op_if_icmpeq();
            break;
            }

//------------------TASK 4------------------//

            case OP_LDC_W:{
            op_ldc_w();
            break;
            }

            case OP_ILOAD:{
            op_iload();
            break;
            }

            case OP_ISTORE:{
            op_istore();
            break;
            }

            case OP_IINC:{
            op_iinc();
            break;
            }

            case OP_WIDE  :{
            op_wide();
            break;
            }

//------------------TASK 5------------------//

            case OP_INVOKEVIRTUAL:{
            op_invokevirtual();
            break;
            }

            case OP_IRETURN:{
            op_ireturn();
            break;
            }

//------------------BONUS------------------//
            case ((int32_t) 0xE1): {
            //op_netbind();
            break;
            }

            case ((int32_t) 0xE2): {
            //op_netconnect();
            break;
            }

            case ((int32_t) 0xE3): {
            //op_netin();
            break;
            }

            case ((int32_t) 0xE4): {
            //op_netout();
            break;
            }

            case ((int32_t) 0xE5): {
            //op_netclose();
            break;
            }

        }

     program_counter++;

     return 1;

     }
 }
