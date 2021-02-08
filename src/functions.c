#include <ijvm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "functions.h"


//function for reading binary blocks made based on the example in the course manual
int32_t read_data(FILE *fp){
    int32_t size;
    fread(&size, sizeof(int32_t), 1, fp);

    int32_t swap_uint32 = ((size>>24)&0xff) | // move byte 3 to byte 0
    ((size<<8)&0xff0000) | // move byte 1 to byte 2
    ((size>>8)&0xff00) | // move byte 2 to byte 1
    ((size<<24)&0xff000000);
    
    return swap_uint32;
}

//two methods, one for dealing with signed integers and one for unsigned

int16_t get_signed_short(int i){
    int16_t offset = (short) (data.text_values[i+1] << 8 | data.text_values[i+2]);
    return offset;
}

uint16_t get_unsigned_short(int i){
    uint16_t offset = (short) (data.text_values[i+1] << 8 | data.text_values[i+2]);
    return offset;
}
