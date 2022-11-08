
#include <xc.h>

#define _XTAL_FREQ  20000000

#include<string.h>

#include<stdlib.h>

#include "config.h"


#define tric PORTBbits.RB1

#define ir PORTBbits.RB2


#define relay1 PORTCbits.RC2

#define relay2 PORTCbits.RC3

#define relay3 PORTCbits.RC4

#define relay4 PORTCbits.RC5


#define rly1LED PORTBbits.RB3

#define rly2LED PORTBbits.RB4

#define rly3LED PORTBbits.RB5

#define rly5LED PORTBbits.RB6

#define rly4LED PORTCbits.RC1

#define fanLED PORTCbits.RC0


int flag=0;

int cmd=0;

int speed=5;

unsigned int dat[100];

int i=0;

char result[10];

int j=0;

   

void delay(int time)

{

    for(int i=0;i<time;i++)

        for(int j=0;j<800;j++);

}



void timer()           // 10 -> 1us

{

   T0CONbits.T0PS0=0; 

   T0CONbits.T0PS1=0;

   T0CONbits.T0PS2=0;

   PSA=0;      //Timer Clock Source is from Prescaler

   T0CS=0;     //Prescaler gets clock from FCPU (5MHz)

   T08BIT=0;   //16 BIT MODE

   TMR0IE=1;   //Enable TIMER0 Interrupt

   PEIE=1;     //Enable Peripheral Interrupt

   GIE=1;      //Enable INTs globally

   TMR0ON=1;      //Now start the timer!

}


void main(void) 

{

   ADCON1=0b00001111;

   TRISBbits.TRISB1=0;

   TRISBbits.TRISB2=1;

   TRISBbits.TRISB3=0;

   TRISBbits.TRISB4=0;

   TRISBbits.TRISB5=0;

   TRISC=0x00;

   TRISA=0x00;

   PORTA=0xc0;             

   TRISB6=0;

   PORTBbits.RB6=1;

    relay1=0;

    relay2=0;

    relay3=0;

    relay4=0;

    rly1LED=0;

    rly3LED=0;

    rly2LED=0;

    rly4LED=0;

    fanLED=0;

        i=0;

    ir=0;

    tric=0;

    timer();

    INTEDG0 = 0; // Interrupt on falling edge

    INT0IE = 1; // Enable the INT0 external interrupt (RB0) 

    INT0IF = 0; // Clears INT0 External Interrupt Flag bit

    PEIE=1;     //Enable Peripheral Interrupt

    GIE=1;      //Enable INTs globally


   while(1)

   {   

       while(ir == 1);

       INT0IE = 0;

       while(ir == 0);

       TMR0=0;

       while(ir == 1);

       i++;

       dat[i]=TMR0;

       

       if(dat[1] > 5000 && dat[1]<12000)

       {

       }

       else

       {

           i=0;

           INT0IE = 1;

       }


       if(i>=33)

       {

            GIE=0;

            delay(50);

            cmd=0;

            for(j=26;j<34;j++)

        {

            if(dat[j]>1000 && dat[j]<2000){

                cmd<<=1;
            }

            else if(dat[j]>3500 && dat[j]<4500)

            {

                cmd|=0x01;
                cmd<<=1;

            }

        }

        cmd>>=1;

         

         if(cmd == 0xAF)
         {
             relay1=~relay1;
             rly1LED=~rly1LED;
         }

         

         else if(cmd == 0x27)
         {
             relay2=~relay2;
             rly2LED=~rly2LED;
         }

         

         else if(cmd == 0x07)
         {
             relay3=~relay3;
             rly3LED=~rly3LED;
         }

         

         else if(cmd == 0xCF)
         {
             relay4=~relay4;
             rly4LED=~rly4LED;
         }

         

         else if(cmd == 0x5f)
         {
             speed++;

             if(speed>5)
             {
                 speed=5; 
             }
         }

         

         else if(cmd == 0x9f)
         {
             speed--;

             if(speed<=0)
             {
                 speed=0; 
             }
         }

         

        if(speed == 5)        // turned off  5x2= 10ms triger    //speed 0
        {
            PORTA=0xC0;           // display 0
            PORTBbits.RB6=1;
            fanLED=0;             
        }

        

        else if(speed == 4 )    //  8 ms trigger    //speed 1
        { 
            PORTA=0xfc;        // displaying 1
            PORTBbits.RB6=1;
            fanLED=1;
        }

        

        else if(speed == 3)   //   6 ms trigger     // speed 2
        {
            PORTA=0xE4;       // displaying 2                    
            PORTBbits.RB6=0;
            fanLED=1;
        }

        

        else if(speed == 2)   // 4ms trigger  // speed 3
        {
            PORTA=0xF0;       // displaying 3
            PORTBbits.RB6=0;
            fanLED=1;
        }

        

        else if(speed == 1)    // 2ms trigger  // speed 4
        {
            PORTA=0xD9;          // displaying 4
            PORTBbits.RB6=0;
            fanLED=1;
        }

        

        else if(speed == 0)        // 0ms trigger  // speed 5  full power
        {
           PORTA=0xD2;            // displaying 5
           PORTBbits.RB6=0;
           fanLED=1;
        }

        

        else
        {
            PORTBbits.RB6=1;
            PORTA=0xff;         // display off
            fanLED=0;
        }

        

        i=0;
        INT0IE = 1;
        GIE=1;
       }

}

}


void __interrupt() isr(void)
{
    if(INT0IF)

    {
        delay(speed);

        tric=1;

        for(int t=0;t<100;t++);

        tric=0;

        INT0IF=0;

    }

    

   if(TMR0IF)  //Check if it is TMR0 Overflow ISR   
   {
      TMR0IF=0;
   } 

}


