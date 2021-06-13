
#include "C:\Keil\ARM\INC\TI\TM4C123/tm4c123gh6pm1.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "math.h"
float lat=0.0;
float longt=0.0;
float coordnites[2][1000];
float distanc  ;
char distancr[5] ;
int u ;
void SystemInit(){}


bool is_exeeded(float d){
    if(d >= 100){

        return 1;
    }

    return 0;
}
void delay(uint32_t d){
    uint32_t i;
    uint32_t k;
    for(i=0;i<d;i++){
        for(k=0;k<11;k++){}
    }
}
void LCD_command(char com){
GPIO_PORTA_DATA_R = 0x0;// RS=0 command & RW = 0 for write
delay(100);
GPIO_PORTA_DATA_R |=0x80;// E = 1 1000 0000
delay(100);
GPIO_PORTB_DATA_R = com;
GPIO_PORTA_DATA_R &= ~0x80;// E = 0
delay(100);
}
void LCD_data(char data){
// uint32_t i;
GPIO_PORTA_DATA_R = 0x20;
delay(100);
GPIO_PORTA_DATA_R |=0x80; // E = 1
delay(100);
GPIO_PORTB_DATA_R = data;
GPIO_PORTA_DATA_R &=~0x80;// E = 0
delay(100);
// for(i=0;i<8;i++){
// char element = data[0];
// GPIO_PORTA_DATA_R = element;
// GPIO_PORTB_DATA_R |=0x80;// set the E = 1
// delay(1);
// GPIO_PORTB_DATA_R |=0x00;// E = 0
// }
}

