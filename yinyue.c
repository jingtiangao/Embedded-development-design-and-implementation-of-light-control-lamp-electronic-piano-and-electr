/************************************************************************  
[文件名]  C51音乐程序(电子琴)  
[功能]    通过单片机演奏音乐  
  
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
unsigned char p,m;   //m为频率常数变量
   
unsigned char n=0;  //n为节拍常数变量    
unsigned char code music_tab[] ={0x60,0x20,0x55,0x20,0x4c,0x20,0x48,0x20,0x40,0x20,0x39,0x20,0x33,0x20,
0x30,0x20, 0x30,0x20};//格式为: 频率常数, 节拍常数, 频率常数, 节拍常数,    

     
void int0()  interrupt 1  
{  TH0=0xd8;   
   TL0=0xef;   
   n--;   
}   
   
void delay (unsigned char m)   //控制频率延时    
{   
 unsigned i=3*m;   
 while(--i);   
}   
 

void xiang(char i)
{
            m=music_tab[i];
				n=music_tab[i+1]; //取频率常数 和 节拍常数    
            TR0=1;                                             //开定时器1    
           	while(n!=0) 
				{
				Beep=~Beep,delay(m);//等待节拍完成, 通过P1口输出音频(可多声道哦!)    
       	                                         //关定时器1    
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