
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
void LCD_string(char *str){
int count=0;
while(str[count]!='\0'){
LCD_data(str[count]);
count++;
}

}
void init(){
    uint32_t delay;
 SYSCTL_RCGCGPIO_R |= 0x23 ; // port a , b and F activation
    delay = 1; // dummy var
    GPIO_PORTA_LOCK_R = 0X4C4F434B;// the lock register must be set up first
    GPIO_PORTA_CR_R = 0XE0;
    GPIO_PORTA_DEN_R = 0xE0;
    GPIO_PORTA_DIR_R =0xFF;
    GPIO_PORTA_AFSEL_R = 0;
    GPIO_PORTA_PUR_R = 0;
    GPIO_PORTB_LOCK_R = 0X4C4F434B;// the lock register must be set up first
    GPIO_PORTB_CR_R = 0XFF;
    GPIO_PORTB_DEN_R = 0xFF;
    GPIO_PORTB_DIR_R =0xFF;
    GPIO_PORTB_AFSEL_R = 0;
    GPIO_PORTB_PUR_R = 0;
    GPIO_PORTF_LOCK_R = 0X4C4F434B;// the lock register must be set up first
    GPIO_PORTF_CR_R = 0X08;
    GPIO_PORTF_DEN_R = 0x08;
    GPIO_PORTF_DIR_R =0x08;
    GPIO_PORTF_AFSEL_R = 0;
    GPIO_PORTF_PUR_R = 0;
    LCD_command(0x30);
    LCD_command(0x38);
    LCD_command(0x06);
    LCD_command(0x01);
    LCD_command(0x0F);
}
void init_uart(){
    uint32_t delay;
SYSCTL_RCGCUART_R |= 0x04;
SYSCTL_RCGCGPIO_R |= 0x08 ;// port d activation
delay = 1; // dummy var
UART2_CTL_R &= ~0x00000001;// disable the uart until the init.
UART2_IBRD_R = 104;
UART2_FBRD_R = 11;
UART2_LCRH_R = 0x00000070;
UART2_CTL_R |= 0x00000001; // enable UART
GPIO_PORTD_AFSEL_R |= 0xC0;
GPIO_PORTD_DEN_R = 0xC0;
GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R &0x00FFFFFF) + 0x11000000; // to set 1 in pctl pc4 and pc5
GPIO_PORTD_AMSEL_R &= ~0xC0;// disable analog in pc4-5
}
void LCD_G_rc(int raw,int colu){
int adress;
switch (raw){
case 0 : adress = colu;
break;
case 1 : adress = colu + 0x40;
break;
}
LCD_command(adress|0x80);
}
