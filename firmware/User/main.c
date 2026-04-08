#include "ch32fun.h"
#include <stdio.h>

int main(void)
{
    SystemInit();
    funGpioInitAll();

    // Blink PD0 (ALERT pin, unused in v1) as build verification
    funPinMode(PD0, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);

    while(1)
    {
        funDigitalWrite(PD0, FUN_HIGH);
        Delay_Ms(500);
        funDigitalWrite(PD0, FUN_LOW);
        Delay_Ms(500);
    }
}
