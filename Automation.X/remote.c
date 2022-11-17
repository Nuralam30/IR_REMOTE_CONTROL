#include <xc.h>

#include<pic18f2520.h>

#define _XTAL_FREQ  20000000

#include<string.h>

#include<stdlib.h>


// CONFIG1H
#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bits (Brown-out Reset enabled and controlled by software (SBOREN is enabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)



#define tric PORTBbits.RB1

#define ir PORTBbits.RB2


#define relay1 PORTCbits.RC2

#define relay2 PORTCbits.RC3

#define relay3 PORTCbits.RC4

#define relay4 PORTCbits.RC5


#define rly1LED PORTBbits.RB3

#define rly2LED PORTBbits.RB4

#define rly3LED PORTBbits.RB5

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

   T0PS0=0;

   T0PS1=0;

   T0PS2=0;

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

   TRISB1=0;

   TRISB2=1;

   TRISB3=0;

   TRISB4=0;

   TRISB5=0;

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

            if(dat[j]>1000 && dat[j]<2000)

                cmd<<=1;


            else if(dat[j]>3500 && dat[j]<4500)

            {

                cmd|=0x01;

                cmd<<=1;

            }

        }

        cmd>>=1;



         if(cmd == 0xE3)

         {

             relay1=~relay1;

             rly1LED=~rly1LED;

         }



         else if(cmd == 0xE7)

         {

             relay2=~relay2;

             rly2LED=~rly2LED;

         }



         else if(cmd == 0xE3)

         {

             relay3=~relay3;

             rly3LED=~rly3LED;

         }



         else if(cmd == 0xE3)

         {

             relay4=~relay4;

             rly4LED=~rly4LED;

         }



         else if(cmd == 0xF6)

         {

             speed++;

             if(speed>5)

             {

                 speed=5;

             }

         }



         else if(cmd == 0xEA)

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


void interrupt isr()

{

    if(INT0IF)

    {

        //delay(speed);

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


