/*===============================================
Author 			: Bhupendra Naphade
File 			: Lisa_Scrambling.c
Copyright 		:
Description		: Program to Implement LISA with 
			      data scrambling technique
===============================================*/
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "cr_section_macros.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "lisa.h"


#define tx 3
#define rx 4

#define TICKRATE_HZ1 10
#define rate 500000

char buffer[1000];
char output[500];
int opcounter = 0;
int oplength = 0;

void SetGPIO(uint8_t port, uint32_t pin)
{
    if (port == 0)
    {
        LPC_GPIO0->FIOSET |= (1 << pin);
    }
    else if (port == 1)
    {
        LPC_GPIO1->FIOSET |= (1 << pin);
    }
    else if (port == 2)
    {
        LPC_GPIO2->FIOSET |= (1 << pin);
    }
    else
    {
        printf("Not a valid port!\n");
    }
    return;
}

void ClrGPIO(uint8_t port, uint32_t pin)
{
    if (port == 0)
    {
        LPC_GPIO0->FIOCLR |= (1 << pin);
    }
    else if (port == 1)
    {
        LPC_GPIO1->FIOCLR |= (1 << pin);
    }
    else if (port == 2)
    {
        LPC_GPIO2->FIOCLR |= (1 << pin);
    }
    else
    {
        printf("Not a valid port!\n");
    }
    return;
}

void GPIOInit()
{
    //LPC_PINCON->PINMODE0 = 0x00000010;
    LPC_GPIO2->FIODIR &= ~(1<<rx);
    LPC_GPIO2->FIODIR |= (1<<tx);
    LPC_GPIO2->FIODIR |= (1<<12);
    ClrGPIO(2,tx);
}
//int j;

void delayMs(uint32_t delayInMs)
{
    LPC_TIM0->TCR = 0x02;        /* reset timer */
    LPC_TIM0->PR  = 0x00;        /* set prescaler to zero */
    LPC_TIM0->MR0 = delayInMs * (9000000 / 1000-1);
    LPC_TIM0->IR  = 0xff;        /* reset all interrrupts */
    LPC_TIM0->MCR = 0x04;        /* stop timer on match */
    LPC_TIM0->TCR = 0x01;        /* start timer */

    /* wait until delay time has elapsed */
    while (LPC_TIM0->TCR & 0x01);

  return;
}

void getPayloadASCII(int max, char *p)
{
    int i = 0,cnt=0,temp=0,j=0;
    int ASCIIPayload[max/8];
    ASCIIPayload[0]='\0';
   
    while(i<max)
    {   temp=0;
        for(cnt=0;cnt<8;cnt++)
            {
                temp += (p[i] - '0')*pow(2,(7-cnt));
                ++i;
            }
        ASCIIPayload[j]=temp;
        ++j;
        printf("%c",temp);
        
    }
    
}

int getindex(int *a, char *str)
{
    int indexarr[4];
    int cnt;
    int index=0;
    char x;
   
    for(cnt=0;cnt<4;cnt++)
    {
        x = str[*a];
		indexarr[cnt]= (x - '0');
        *a +=1;
        index += indexarr[cnt]*pow(2,(3-cnt));
    }
    return index;
}

void receive()
{
	int i = 0 ;
	   int j = 0 ;
	   int x,y;
	   unsigned char *payload;
  		bool descramble [700];
  		int del3=0,del7=0;
	   
	   while(i<800)
	   {
	   	if(LPC_GPIO2->FIOPIN & (1<<rx))
	   		{
	   		   SetGPIO(2,12);
	   		   buff[i]=1;
	   		}
	   		else
	   		{
	   			ClrGPIO(2,12);
	   		   buff[i]=0;
	   		}
	   		delayMs(5);
	   		i++;
	   }

	   	if(1){
	   			y=identify(buff);
	   		   printf("Sync index:: %d\n",y);
	   		   fflush(stdout);
	   		   x=payload_start(y,buff);
	   		   printf("Payload Start:: %d\n",x*8+y);
	   		   fflush(stdout);
	   		  j=0;
	   		for(i=(x*8)+y;i<(x*8)+y+256;i++)
	   		{
	   		    if(j>=4)
	   		    {
	   		        del3=buff[i-4];
	   		    }
	   		    else
	   		    {
	   		        del3 = 0;
	   		    }
	   		    if(j>=7)
	   		    {
	   		        del7=buff[i-7];
	   		    }
	   		    else
	   		    {
	   		        del7 = 0;
	   		    }

	   		    descramble[j] =(del3^del7)^buff[i];
	   		    j++;
	   		}
	   		printf("\nDescramble data : ");
	   		for(i=0;i<256;i++)
	   			{printf("%d",descramble[i]);

	   			}
	   		printf("\nReceived Data : ");
	   		   payload = bintochar(descramble,0,256);
	   	  }
}

void transmit(void)
{
    char text[]= {"SJSUCMPE_BHUPENDRA_010703836_F15"};
    char BinText[strlen(text)*8];
    int i =0;
    char SyncField[] = {"1010000010100001101000101010001110100100101001011010011010100111101010001010100110101010101010111010110010101101101011101010111101010000010100010101001001010011010101000101010101010110010101110101100001011001010110100101101101011100010111010101111001011111"};
    oplength = strlen(SyncField)+sizeof(BinText);
    output[0]='\0';
    int temp[8];
    int t = 0, count = 0,j,k;
    for(j=0;j<strlen(text);j++)
        {
            t=text[j];
            for(i=0;i<8;i++)
            {
                temp[i]=t%2;
                t /=2;
            }
            for(k=7;k>=0;k--)
            {
                BinText[count]=temp[k]+'0';
                count++;
            }
        }
        strcat(output,SyncField);
        printf("Data to be Transmitted: %s\n",text);
        printf("BinText : %s\n",BinText);
       //scrambler
        int N;
        bool scramble [300];
        bool ip[300];
        bool del3,del7;


		N = strlen(BinText);
		for(i=0;i<N;i++)
		{
			if(((int)BinText[i])==48)
			{
				ip[i]=0;
			}
			else
			{
				ip[i]=1;
			}
		}
		for(i=0;i<N;i++)
		{
		    if(i>=4)
		    {
		        del3=scramble[i-4];
		    }
		    else
		    {
		        del3 = 0;
		    }
		    if(i>=7)
		    {
		        del7=scramble[i-7];
		    }
		    else
		    {
		        del7 = 0;
		    }

		    scramble[i] =ip[i]^del3^del7;
		}
		for(i=0;i<N;i++)
		{
		    BinText[i]=scramble[i]+'0';
		}
		//end scrambler*/
        strcat(output,BinText);
        for(i=0;i<oplength;i++)
        {
            if (output[i]=='1')
            {
				SetGPIO(2,tx);
                SetGPIO(2,12);
            }
            else if (output[i]=='0')
            {
                ClrGPIO(2,tx);
                ClrGPIO(2,12);
            }
            delayMs(5);
        }
        printf("\nScrambled output  : %s",BinText);
        printf("\nData Transmitted  : %s\n",output);

}

int main(void) {
    
        volatile static int i = 0 ;

        GPIOInit();
        ClrGPIO(2,tx);

#if 1
        printf("Select from below menu\n"
                "1. Transmit Data\n"
                "2. Receive Data\n"
                "Your Selection:");
        scanf("%d\n",&i);

        switch(i)
        {
        case 1:  transmit();
                 break;
        case 2:  receive();
                 break;
        default: printf("Enter Correct Selection\n");
                 break;
        }
#endif

    while(1) {

        }
    return 0 ;
}
