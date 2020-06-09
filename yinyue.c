/************************************************************************  
[�ļ���]  C51���ֳ���(������)  
[����]    ͨ����Ƭ����������  
  
/**********************************************************************/   
#include <REG52.H>    
#include <INTRINS.H>    

sbit Beep =  P1^5 ;
sbit s1=P2^7;
sbit s2=P2^6;
sbit s3=P2^5;
sbit s4=P2^4;
sbit s5=P2^3;
sbit s6=P2^2;
sbit s7=P2^1;
sbit s8=P2^0;
unsigned char p,m;   //mΪƵ�ʳ�������
   
unsigned char n=0;  //nΪ���ĳ�������    
unsigned char code music_tab[] ={0x60,0x20,0x55,0x20,0x4c,0x20,0x48,0x20,0x40,0x20,0x39,0x20,0x33,0x20,
0x30,0x20, 0x30,0x20};//��ʽΪ: Ƶ�ʳ���, ���ĳ���, Ƶ�ʳ���, ���ĳ���,    

     
void int0()  interrupt 1  
{  TH0=0xd8;   
   TL0=0xef;   
   n--;   
}   
   
void delay (unsigned char m)   //����Ƶ����ʱ    
{   
 unsigned i=3*m;   
 while(--i);   
}   
 

void xiang(char i)
{
            m=music_tab[i];
				n=music_tab[i+1]; //ȡƵ�ʳ��� �� ���ĳ���    
            TR0=1;                                             //����ʱ��1    
           	while(n!=0) 
				{
				Beep=~Beep,delay(m);//�ȴ��������, ͨ��P1�������Ƶ(�ɶ�����Ŷ!)    
       	                                         //�ض�ʱ��1    
   			} 
			    TR0=0;  
} 
  
void key()
{
	if(s1==0)
		{
		delay(25);
		if(s1==0)
			{
			
			xiang(0);
			}
		 }
		if(s2==0)
		{
		delay(25);
			if(s2==0)
			{
				xiang(2);
			}
		 }
	if(s3==0)
		{
		delay(25);
			if(s3==0)
			{
				xiang(4);
			}
		
		}
	if(s4==0)
		{
		delay(25);
			if(s4==0)
			{
			
				xiang(6);
			}
		
		}
	if(s5==0)
		{
		delay(25);
			if(s5==0)
			{
			
				xiang(8);
			}
		
		}
	if(s6==0)
		{
		delay(25);
			if(s6==0)
			{
			
				xiang(10);
			}
		
		}
	if(s7==0)
		{
		delay(25);
			if(s7==0)
			{
			
				xiang(12);
			}
		
		}
	if(s8==0)
		{
		delay(25);
			if(s8==0)
			{
				xiang(14);
			}
		
		}
}
void main()   
{ 
    
  unsigned char i=0;   
  TMOD&=0x0f;   
  TMOD|=0x01;   
  TH0=0xd8;TL0=0xef;  
  IE=0x82;
 

while(1)
{
key();
}




}  