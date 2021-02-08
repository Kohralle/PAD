#ifndef FUNCTIONS_H
#define FUNCTIONS_H

int program_counter;

FILE *out;
FILE *in;

int *socket_array;
int socket_counter;

struct data{
    int32_t magic_number;
    int32_t constant_origin;
    int32_t constant_size;
    int32_t text_origin;
    int32_t text_size;
    uint32_t *constant_values;
    uint8_t *text_values;
};

struct data data;

int32_t read_data(FILE *fp);

//two methods, one for dealing with signed integers and one for unsigned
int16_t get_signed_short(int i);

uint16_t get_unsigned_short(int i);


#endif
