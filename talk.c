#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define SW_INT 0x60

//change Destination MAC address before compiling
unsigned char dest_mac[] = "\x08\x00\x27\x52\xbf\x0f";

unsigned char data[64];

unsigned char c[2];
int handle,packet_len,i;
unsigned char e[200];
unsigned char from_mac[6];
unsigned char type[] = "\xff\xff";

unsigned char broadcastAdd[] = "\xff\xff\xff\xff\xff\xff";
unsigned char buffer[100];
int bufferCounter;

unsigned char temp;
int exitCounter=0;

void get_mac_address();
void fill_headers();
void fill_data(unsigned char *msg, int length);
void send_packet();
int getHandle();
void releaseType();

void interrupt receiver(bp, di, si, ds, es, dx, cx, bx, ax, ip, cs, flags)
unsigned short bp, di, si, ds, es, dx, cx, bx, ax, ip, cs, flags;
{
        //printf("Reciever ax=%d\n",ax);
        if ( ax == 0)
        {
        	//discard packets of size greater than 200
        	if(cx>200)
        	{
        		es = 0;
        		di = 0;
        		return;
        	}
            es=FP_SEG(e);
            di=FP_OFF(e);
            packet_len=cx;
        }
        if(ax==1)
        {
        	if(memcmp(e,broadcastAdd,6) == 0)
        	{
        		//broadcast packet received discard it
        		return;
        	}
			for(i=6;i<12;i++)
			{
				cprintf("%02x:",e[i]);
			}

        	cprintf(" > ");
			//skip type

		    for (i=14;i<packet_len;i++)
		    {
		    	if(e[i] == 0x00)
		    	{
		    		break;
		    	}
				putch(e[i]);
		    }
		    putch('\r');
			putch('\n');
       }
}

int main()
{
        get_mac_address();
        fill_headers();
        if(getHandle() != 0)
        	printf("Error getting Handle, Exitting\r\n");

        bufferCounter = 0;
        
        while(1)
        {
        	temp = getchar();
        	if(temp==0x0D || temp ==0x0A)
        	{
        		if(exitCounter >= 1)
        		{
        			break;
        		}
        		if(bufferCounter ==0)
        			exitCounter++;
        		fill_data(buffer,bufferCounter);
        		bufferCounter = 0;
        		send_packet();
        	}
        	else
        	{
        		exitCounter = 0;
	        	buffer[bufferCounter] = temp;
	        	bufferCounter++;
	        }
        }

		releaseType();

		printf("Bye Bye\n");
		return 0;
}

int getHandle()
{
	union REGS inregs,outregs;
	struct SREGS segregs;
	inregs.h.al=1;
	inregs.x.bx=-1;
	inregs.h.dl=0;
	inregs.x.cx=0;
	inregs.h.ah=2;
	segregs.es=FP_SEG(receiver);
	inregs.x.di=FP_OFF(receiver);
	c[0]=0xff;
	c[1]=0xff;
	segregs.ds=FP_SEG(c);
	inregs.x.si=FP_OFF(c);
	int86x(SW_INT,&inregs,&outregs,&segregs);
	printf("Carry Flag Access Type %d\n",outregs.x.cflag);
	printf("Handle %d\n",outregs.x.ax);
	handle = outregs.x.ax;
	return outregs.x.cflag;
}

void releaseType()
{
	union REGS inregs,outregs;
	struct SREGS segregs;
	inregs.h.ah=3;
	inregs.x.bx=handle;
	int86x(SW_INT,&inregs,&outregs,&segregs);
	printf("Carry Flag Release Type %d\n",outregs.x.cflag);
}

void printReceiveMode()
{
	union REGS inregs,outregs;
	struct SREGS segregs;
	inregs.h.ah=21;
	inregs.x.bx=handle;
	int86x(SW_INT,&inregs,&outregs,&segregs);
	printf("Carry Flag Get Recieve Mode %d\n",outregs.x.cflag);
	printf("Get Recieve Mode %d\n",outregs.x.ax);
}

int setReceiveMode(int mode)
{
	union REGS inregs,outregs;
	struct SREGS segregs;
	inregs.h.ah=20;
	inregs.x.bx=handle;
	inregs.x.cx=mode;
	int86x(SW_INT,&inregs,&outregs,&segregs);
	printf("Carry Flag Set Recieve Mode %d\n",outregs.x.cflag);
	return outregs.x.cflag;
}



void get_mac_address()
{
	union REGS inregs,outregs;
	struct SREGS segregs;
	char far *bufptr;
	segread(&segregs);
	bufptr = (char far *)from_mac;
	segregs.es = FP_SEG(bufptr);
	inregs.x.di = FP_OFF(from_mac);
	inregs.x.cx = 6;
	inregs.h.ah = 6;
	int86x(SW_INT, &inregs, &outregs, &segregs);
}

//To be called after calling the get_mac method
void fill_headers()
{
	memcpy(data, dest_mac, 6); //set the destination mac
	memcpy(data+6, from_mac, 6); //set the source mac
	memcpy(data+12, type, 2); //set the type
}

void fill_data(unsigned char *msg, int length)
{
	if(length+14 > 64)
	        length = 64-14;
	memcpy(data+14, msg, length);

	for(i=length+14; i<64; i++)
	{
		data[i] = 0;
	}
}

void send_packet()
{
	union REGS inregs,outregs;
	struct SREGS segregs;
	segread(&segregs);
	inregs.h.ah = 4;
	segregs.ds = FP_SEG(data);
	inregs.x.si = FP_OFF(data);
	inregs.x.cx = 64;
	int86x(SW_INT,&inregs,&outregs,&segregs);
}
