#include <ijvm.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "frames.h"
#include "stack.h"
#include "operations.h"
#include "functions.h"

//------------------TASK 2------------------//
void op_bipush(){
    program_counter++;
    int8_t bi_push = data.text_values[program_counter];
    stack_push (bi_push);
}

void op_iadd(){
    stack_push (stack_pop() + stack_pop());
}

void op_isub(){
    int32_t x = stack_pop();
    int32_t y = stack_pop();
    int32_t calc = y - x;
    stack_push (calc);
}

void op_iand(){
    stack_push (stack_pop() & stack_pop());
}

void op_ior(){
    stack_push (stack_pop() | stack_pop());
}

void op_swap(/* arguments */) {
    int32_t a = stack_pop ();
    int32_t b = stack_pop ();
    stack_push (a);
    stack_push (b);
}

void op_dup(){
    int dup = stack_top();
    stack_push(dup);
}

void op_in() {

    int x;
    x = fgetc(in);

    if (x == EOF){
        stack_push(0);
    }

    else {
        stack_push(x);
    }

}

void op_out() {
    int op_out = stack_pop();
    fprintf(out, "%c", op_out);
}

//------------------TASK 3------------------//



void op_goto(){
     program_counter = program_counter + get_signed_short(program_counter) - 1;
}

void op_ifeq(){
    if (stack_pop() == 0){
        program_counter = program_counter + get_signed_short(program_counter) - 1;
    }

    else {
        program_counter = program_counter + 2;
    }
}

void op_iflt(){
    if (stack_pop() < 0){
        program_counter = program_counter + get_signed_short(program_counter) - 1;
    }
    else {
        program_counter = program_counter + 2;
    }
}

void op_if_icmpeq(){

    if (stack_pop() == stack_pop()){
        program_counter = program_counter + get_signed_short(program_counter) - 1;
    }

    else{
        program_counter = program_counter + 2;
    }
}

//------------------TASK 4------------------//

void op_ldc_w() {
    stack_push(data.constant_values[get_unsigned_short(program_counter)]);
    program_counter = program_counter + 2;
}

void op_iload(){
    int32_t local_var = get_local_variable(data.text_values[program_counter+1]);
    stack_push(local_var);

    program_counter++;

}

void op_istore(){

    int32_t local_var = stack_pop();
    frames[frame_size].local_vars[data.text_values[program_counter+1]] = local_var;

    program_counter++;

}

void op_iinc(){
    int8_t constant = data.text_values[program_counter + 2];
    int current_value = frames[frame_size].local_vars[data.text_values[program_counter+1]];

    frames[frame_size].local_vars[data.text_values[program_counter+1]] = current_value + constant;

    program_counter = program_counter + 2;
}

void op_wide(){

    program_counter++;

    switch (get_instruction()) {

        case OP_ILOAD:{

            int32_t local_var;
            local_var = get_local_variable(get_unsigned_short(program_counter));

            stack_push(local_var);
            program_counter = program_counter + 2;

            break;

        }

        case OP_IINC :{

            int8_t constant = data.text_values[program_counter + 1];
            int current_value = frames[frame_size].local_vars[get_unsigned_short(program_counter)];

            frames[frame_size].local_vars[get_unsigned_short(program_counter)] += current_value + constant;

            program_counter = program_counter + 2;

            break;

        }

        case OP_ISTORE :{

            int32_t local_var = stack_pop();
            frames[frame_size].local_vars[get_unsigned_short(program_counter)] = local_var;

            program_counter = program_counter + 2;

            break;

        }

    }

}

//------------------TASK 5------------------//


void op_invokevirtual(/* arguments */) {
    int16_t i = get_signed_short(program_counter);
    int pcAfterInvoke = program_counter + 2;
    program_counter = data.constant_values[i]-1;

    int16_t numberOfArguments = get_unsigned_short(program_counter);
    int16_t local_var_size = get_unsigned_short(program_counter+2);

    program_counter = program_counter + 4; //jump past the text area of INVOKEVIRTUAL
    frame_size++;

    if(frame_size == frame_capacity){
        frame_capacity = frame_capacity * 3;
        frames = (Frame *) realloc(frames, frame_capacity * sizeof(Frame));
    }

    int stackComeBackPoint = stack_size_info() - numberOfArguments;
    frames[frame_size] = make_frame(pcAfterInvoke, local_var_size, stackComeBackPoint, numberOfArguments);

    if (numberOfArguments >= 0) {
        for(int i = numberOfArguments - 1; i > 0; i--){
            frames[frame_size].local_vars[i] = stack_pop();
        }
    }
}

void op_ireturn(){
    int32_t stack = stack_pop();

     do{
        stack_pop();
     }
     while(stack_size_info() - frames[frame_size].previous_stack != 0);

     program_counter = frames[frame_size].previous_frame;

     stack_push(stack);

     frame_size--;
}


//------------------BONUS NETWORK------------------//
/*
void op_netbind() {
    int port_integer = stack_pop();
    char port[10];
    sprintf(port, "%d", port_integer); //for the specification I am using the port has to be a string
    printf("%s\n",port );

    struct addrinfo hints;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_INET; //specyfying to connect to ipv4 addresses since only those appear in the ijvm file
    hints.ai_socktype=SOCK_STREAM; //allows for tcp connection
    hints.ai_protocol=0;
    hints.ai_flags=AI_PASSIVE|AI_ADDRCONFIG; //for binding the server to socket
    struct addrinfo* res=0;
    int status=getaddrinfo(NULL,port,&hints,&res); //identifies the internet address

    if (status!=0) {
        stack_push(0);
        return;
    }

    int network_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (network_socket == -1){
        stack_push(0);
        return;
    }

    if(bind(network_socket, res->ai_addr, res->ai_addrlen) == -1){
        stack_push(0);
        return;
    }

    if(listen(network_socket, 1)){ //listen returns false if it is unable to listen
        stack_push(0);
        return;
    }

    socklen_t addr_size = sizeof hints;

    if((accept(network_socket, (struct addrinfo *)&hints, &addr_size)) < 0){
        stack_push(0);
        return;
    }

    socket_array[socket_counter] = network_socket;
    socket_counter++;

    uint32_t random_var = rand();
    stack_push(random_var);

}

void op_netconnect() {

    int port = stack_pop();
    int ip = stack_pop();
    char host_string[10];

    //https://stackoverflow.com/questions/1680365/integer-to-ip-address-c
    unsigned char bytes[10];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;

    sprintf(host_string, "%d.%d.%d.%d", bytes[3], bytes[2], bytes[1], bytes[0]);

    int network_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (network_socket < 0){
        stack_push(0);
        return;
    }

    struct sockaddr_in add;

    add.sin_family = AF_INET;
    add.sin_port = htons(port);
    add.sin_addr.s_addr = inet_addr(host_string);

    if (bind(network_socket, (struct sockaddr*) &add, sizeof(struct sockaddr_in))){
        stack_push(0);
        return;
    }

    if(connect(network_socket, (struct sockaddr *)&add, sizeof(add)) < 0){
       stack_push(0);
       return;
    }

    socket_array[socket_counter] = network_socket;
    socket_counter++;

    uint32_t random_var = rand();
    stack_push(random_var);

}

void op_netin() {
    uint32_t netref = stack_pop();
    int socket = socket_array[netref];
    char buffer[1];
    recv(socket, buffer, 1, 0); //read one byte from the socket to the buffer
}

void op_netout() {
    char msg = (char)stack_pop();
    char msg_buffer[1];
    msg_buffer[0] = msg;
    uint32_t netref = stack_pop();
    int socket = socket_array[netref];
    send(socket, msg_buffer, 1, 0);
}

void op_netclose() {
    uint32_t netref = stack_pop();
    int socket = socket_array[netref];
    close(socket);
}
*/
