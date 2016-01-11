/**
 * \file little.c
 * \brief Manage the conversion int uint32_t
 * 
 * 
 * */
#include "little.h"

/**
 * \fn uint32_t int_to_little(int x)
 * \brief Convert int to uint32_t on little endian
 * \param x Int which will be convert
 * \return uint32_t representing the given int
 * */
uint32_t int_to_little(int x){
    uint32_t r;
    unsigned char *t =(unsigned char*)&r;
    t[0]=x%256;
    t[1]=(x/256)%256;
    t[2]=((x/256)/256)%256;
    t[3]=(((x/256)/256)/256)%256;
    return r;
}


/**
 * \fn int little_to_int(uint32_t x)
 * \brief Convert uint32_t to integer
 * \param x Uint32_t which will be convert
 * \return int representing the given uint32_t
 * */
int little_to_int(uint32_t x){
    unsigned char *buf=(unsigned char*)(&x);
    int b=(buf[0] + 256*buf[1] +(256*256)*buf[2] +(256*256*256)*buf[3]);
    return b;
}
