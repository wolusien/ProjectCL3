#include "little.h"

uint32_t int_to_little(int x){
    int v=x;
    uint32_t r;
    unsigned char *t =(unsigned char*)&r;
    t[0]=x%256;
    t[1]=(x/256)%256;
    t[2]=((x/256)/256)%256;
    t[3]=(((x/256)/256)/256)%256;
    return r;
}

int little_to_int(uint32_t x){
    unsigned char *buf=(unsigned char*)(&x);
    int b=(buf[0] + 256*buf[1] +(256*256)*buf[2] +(256*256*256)*buf[3]);
    return b;
}
