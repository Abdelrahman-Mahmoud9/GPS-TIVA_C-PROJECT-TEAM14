
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
float toRadians(const float degree)
{ //long double M_PI = (long double) 3.141592654;
    float one_deg = 3.141592654 / 180;
return (one_deg * degree);
}


float distance(float lat1, float long1,
float lat2, float long2)
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
float dlong = long2 - long1;
float dlat = lat2 - lat1;
//printf("%f %f \n", dlong,dlat);
float ans = pow(sin(dlat / 2), 2) +
cos(lat1) * cos(lat2) *
pow(sin(dlong / 2), 2);
//printf("%f \n", ans);
ans = 2 * asin(sqrt(ans));
//printf(("%f \n"), ans);
// Radius of Earth in
// Kilometers, R = 6371
// Use R = 3956 for miles
float R = 6371;


 // Calculate the result
ans = ans * R;
//printf("%f \n", ans);
return ans;
}
void gps(){

    while(1){


    }
}

int main(void)
{

    init_uart();
    init();
    int u=0;
    distanc = 0.0;

while(1){

     readGPSModule();

}


}


//$GPRMC,200751.00,A,4047.32510,N,02929.63031,E,9.879,105.80,301117,,,A*6C
void readGPSModule(void){
    char c0,GPSValues[100],latitudeResult[10],longitudeResult[10],parseValue[12][20],*token;
    double latitude=0.0,longitude=0.0,seconds=0.0,laresult=0.0,loresult=0.0,minutes=0.0;
    const char comma[2] = ",";
    int index=0,degrees;


    while((UART2_FR_R&0x0010) !=0);
    c0=UART2_DR_R&0xFF;

    //gelen data $GPRMC mi?
    if(c0=='$'){
        while((UART2_FR_R&0x0010) !=0);
        char c1=UART2_DR_R&0xFF;
        if(c1=='G'){
            while((UART2_FR_R&0x0010) !=0);
            char c2=UART2_DR_R&0xFF;
            if(c2=='P'){
                while((UART2_FR_R&0x0010) !=0);
                char c3=UART2_DR_R&0xFF;
                if(c3=='R'){
                    while((UART2_FR_R&0x0010) !=0);
                    char c4=UART2_DR_R&0xFF;
                    if(c4=='M'){
                        while((UART2_FR_R&0x0010) !=0);
                        char c5=UART2_DR_R&0xFF;
                        if(c5=='C'){
                            while((UART2_FR_R&0x0010) !=0);
                            char c6=UART2_DR_R&0xFF;
                            if(c6==','){
                                while((UART2_FR_R&0x0010) !=0);
                                char c7=UART2_DR_R&0xFF;

                                //verileri GPSValues arrayine atama.son veri olan checksum a kadar oku(checksum:A*60 gibi)
                                while(c7!='*'){
                                    GPSValues[index]=c7;
                                    while((UART2_FR_R&0x0010) !=0);
                                    c7=UART2_DR_R&0xFF;
                                    index++;}


                                //GPSValues arrayindeki verileri virgul e gore ayirma
                                index=0;
                                token = strtok(GPSValues, comma);
                                while( token != NULL ) {
                                    strcpy(parseValue[index], token);
                                    token = strtok(NULL, comma);
                                    index++;}


                                //parseValue[1] = A ise veri gecerli - V ise gecerli degil
                                if(strcmp(parseValue[1],"A")==0){
                                    latitude=atof(parseValue[2]);
                                    longitude=atof(parseValue[4]);


                                    //latitude hesaplama
                                    degrees=latitude/100;
                                    minutes=latitude-(double)(degrees*100);
                                    seconds=minutes/60.00;
                                    laresult=degrees+seconds;
                                    lat= laresult;
                                   // sprintf(latitudeResult,"%f", laresult);


                                    //longitude hesaplama
                                    degrees=longitude/100;
                                    minutes=longitude-(double)(degrees*100);
                                    seconds=minutes/60.00;
                                    loresult=degrees+seconds;
                                    longt= loresult;
                                    //sprintf(longitudeResult, "%f", loresult);

                                    coordnites[0][u]=laresult;
                                    coordnites[1][u]=loresult;


                                    if(u<=1){
                                        //sprintf(longitudeResult, "%f", coordnites[1][u]);
                                        distanc = 0;
                                        sprintf(latitudeResult, "%f", distanc);
                                        LCD_command(0x1);
                                        LCD_G_rc(0,2);
                                        LCD_string(latitudeResult);
                                        sprintf(longitudeResult, "%d",u);
                                        LCD_G_rc(1,4);
                                        LCD_string(longitudeResult);
                                        printf("%f,%f\n",coordnites[0][u],coordnites[1][u]);

                                    }
                                    else {
                       distanc += distance(coordnites[0][u-2],coordnites[1][u-2],coordnites[0][u-1],coordnites[1][u-1])*1000;


                                    LCD_command(0x1);
                                    LCD_G_rc(0,0);
                                    LCD_string("DISTANCE IN  'm'");
                                    sprintf(latitudeResult, "%f", distanc);
                                    LCD_G_rc(1,3);
                                    LCD_string(latitudeResult);
                                    printf("%f,%f\n",coordnites[0][u],coordnites[1][u]);
                                    if (is_exeeded(distanc)){
                                    GPIO_PORTF_DATA_R |= 0x08;
                                    }
                                    }
                                    u++;

                                    delay(1000000);
                                 //   SysCtlDelay(SysCtlClockGet()/6);
                                }
                                else{
                                    LCD_command(0x1);
                                    LCD_G_rc(0,1);
                                    LCD_string("GPS VOID");}

                                printf("");
                        }}}}}}}
}
