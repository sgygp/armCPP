#ifndef __DELAY_H
#define __DELAY_H
#include "air32f10x_conf.h"
#ifdef __cplusplus
extern "C" {
#endif

void delay_us(u16 us);
void delay_ms(u16 ms);

#ifdef __cplusplus
}
#endif
#endif
