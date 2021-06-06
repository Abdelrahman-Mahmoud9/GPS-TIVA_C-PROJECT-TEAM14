#include "stdint.h"
#include "C:\Keil\ARM\INC\TI\TM4C123/tm4c123gh6pm1.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
 
void SystemInit(){}
bool is_exeeded(long double d){
    if(d >= 100)
        return 1;
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
SYSCTL_RCGCUART_R |= 0x02;
SYSCTL_RCGCGPIO_R |= 0x04 ;// port c activation
delay = 1; // dummy var
UART1_CTL_R &= ~0x00000001;// disable the uart until the init.
UART1_IBRD_R = 104;
UART1_FBRD_R = 11;
UART1_LCRH_R = 0x00000070;
UART1_CTL_R |= 0x00000001; // enable UART
GPIO_PORTC_AFSEL_R |= 0x30;
GPIO_PORTC_DEN_R = 0x30;
GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R &0xFF00FFFF) + 0x00220000; // to set 2 in pctl pc4 and pc5
GPIO_PORTC_AMSEL_R &= ~0x30;// disable analog in pc4-5
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

long double toRadians(const long double degree)
{ //long double M_PI = (long double) 3.141592654;
long double one_deg = 3.141592654 / 180;
return (one_deg * degree);
}


long double distance(long double lat1, long double long1,
long double lat2, long double long2)
{
// Convert the latitudes
// and longitudes
// from degree to radians.
lat1 = toRadians(lat1);
long1 = toRadians(long1);
lat2 = toRadians(lat2);
long2 = toRadians(long2);
//printf("%f %f %f %f \n", lat1,long1,lat2,long2);
// Haversine Formula
long double dlong = long2 - long1;
long double dlat = lat2 - lat1;
//printf("%f %f \n", dlong,dlat);
long double ans = pow(sin(dlat / 2), 2) +
cos(lat1) * cos(lat2) *
pow(sin(dlong / 2), 2);
//printf("%f \n", ans);
ans = 2 * asin(sqrt(ans));
//printf(("%f \n"), ans);
// Radius of Earth in
// Kilometers, R = 6371
// Use R = 3956 for miles
long double R = 6371;


 // Calculate the result
ans = ans * R;
//printf("%f \n", ans);
return ans;
}
int main(){
init_uart();
init();
float b ;
b = distance(29.896425,31.266265,29.89665,31.26629)*1000;
//printf("%f",b);
char g[5] ;
 
sprintf(g,"%f",b);
 
//scat("distanc :", dist)
//while(1){
//ReadGpsModule();
// LCD code to write a letter
LCD_command(0x1); //clear screen
//LCD_command(0x80);
LCD_G_rc(0,5);
LCD_string("distance");
//LCD_G_rc(1,0);
//LCD_command(0xC0);
//LCD_command(0xF);
//delay(50);
LCD_G_rc(1,3);
LCD_string(g);
//}
}
