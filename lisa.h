/*===============================================
Author 			: Bhupendra Naphade
File 			: Lisa_Scrambling.c
Copyright 		:
Description		: Program to Implement LISA with 
			      data scrambling technique
===============================================*/
#ifndef LISA_H_
#define LISA_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define sync_overhead_bits 256
#define payload_overhead_bits 256
#define sync_overhead_bytes 32
#define payload_size_bytes 32
#define buffer_size_bytes 64
#define buffer_size 1024

char bintocharbyte(bool *,int,int);
bool buff[1024];
bool findindex(bool * buffer,int current_index,int next_index)
{
    if(bintocharbyte(buffer,next_index,next_index+8)-bintocharbyte(buffer,current_index,current_index+8)==1)
        return true;
    else
        return false;


}



int identify (bool * buffer)
{
    int i;
    int index1=0, index2=0;
    for(i=0;i<(buffer_size/2);i++)
    {
        if((((buffer[i])^0)+((buffer[i+1])^1)+((buffer[i+2])^0)+((buffer[i+3]^1)))==4)
        {
            index1=i;
            if((((buffer[i+8])^0)+((buffer[i+9])^1)+((buffer[i+10])^0)+((buffer[i+11]^1)))==4)
            {
                if(findindex(buffer,index1,i+8))
                {
                    return i;
                }
            }
        }
        if((((buffer[i])^1)+((buffer[i+1])^0)+((buffer[i+2])^1)+((buffer[i+3]^0)))==4)
        {
            index2=i;

            if((((buffer[i+8])^1)+((buffer[i+9])^0)+((buffer[i+10])^1)+((buffer[i+11]^0)))==4)
            {

                if(findindex(buffer,index2,i+8))
                    {
                        return i;
                    }
            }
        }
    }
    return 0;
}


void chartobin(char x,bool *a,int j)
{
    int i,k;
    for(i=0,k=7;i<8,k>=0;i++,k--)
    {
        a[i+j]=x&(1<<k);
    }
    k=0;
}

char bintocharbyte(bool *a,int index,int end)
{
    unsigned char character=NULL;
    int i,k=7;
    for (i = index; i < end; i++) {

        character|=a[i]<<k;
        k--;
    }
    return character;
}

unsigned char * bintochar(bool *a,int index,int end)
{
	int j=0,temp=0,cnt=0;
	unsigned char *ASCIIPayload[32];
	    ASCIIPayload[0]='\0';
	    while(index<end)
	    {   temp=0;
	        for(cnt=0;cnt<8;cnt++)
	            {
	                temp += a[index]*pow(2,(7-cnt));
	                ++index;
	            }
	        ASCIIPayload[j]=temp;
	        ++j;
	        printf("%c",temp);
	      }
	    return ASCIIPayload;

}

int payload_start(int sync_info,bool *buffer)
{
    unsigned char x= bintocharbyte(buffer,sync_info,sync_info+8);
    switch(x)
    {
        case 0xA0:
            return 32;
        case 0xA1:
            return 31;
        case 0xA2:
            return 30;
        case 0xA3:
            return 29;
        case 0xA4:
            return 28;
        case 0xA5:
            return 27;
        case 0xA6:
            return 26;
        case 0xA7:
            return 25;
        case 0xA8:
            return 24;
        case 0xA9:
            return 23;
        case 0xAA:
            return 22;
        case 0xAB:
            return 21;
        case 0xAC:
            return 20;
        case 0xAD:
            return 19;
        case 0xAE:
            return 18;
        case 0xAF:
            return 17;
        case 0x50:
            return 16;
        case 0x51:
            return 15;
        case 0x52:
            return 14;
        case 0x53:
            return 13;
        case 0x54:
            return 12;
        case 0x55:
            return 11;
        case 0x56:
            return 10;
        case 0x57:
            return 9;
        case 0x58:
            return 8;
        case 0x59:
            return 7;
        case 0x5A:
            return 6;
        case 0x5B:
            return 5;
        case 0x5C:
            return 4;
        case 0x5D:
            return 3;
        case 0x5E:
            return 2;
        case 0x5F:
            return 1;
    }

    return 0;
}


#endif /* LISA_H_ */
