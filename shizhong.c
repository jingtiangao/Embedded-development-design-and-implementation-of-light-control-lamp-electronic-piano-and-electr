#include<reg52.h>
sbit RS=P2^6;
sbit RW=P2^5;
sbit check=P3^0;
sbit zuo=P3^1;
sbit you=P3^2;
sbit LCDEN=P2^7;
#define uchar unsigned char
#define uint unsigned int
char miao =0,shi=0,fen=0;
uchar num=0,chnum=0;
int i,j=0;
uchar code table[]="2013-11-5";
uchar code table1[]="  :  :00";


void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}
void write_com(uchar z)
{
RS=0;
RW=0;
delay(5);
LCDEN=1;
delay(1);
P0=z;
LCDEN=0;

}
void write_date(uchar z)
{
RS=1;
RW=0;
delay(5);
LCDEN=1;
delay(1);
P0=z;
LCDEN=0;

}
void write_byte(uchar add,uchar date)
{
write_com(0x80+add);
write_date(date);
}
void init()
{
write_com(0x01);
write_com(0x02);
write_com(0x06);
write_com(0x0F);
write_com(0x38);
TMOD=0x01;
EA=1;
ET0=1;
TR0=1;
for(i=0;i<9;i++)
{
write_byte(i,table[i]);
}
for(i=0;i<8;i++)
{

write_byte(i+40,table1[i]);

}
}

void keycheck()
{
				check=1;
				delay(5);
			
				if(check==0)
				{
					delay(5);
					if(check==0)
					{
							chnum++;
							while(!check);
								if(chnum==1)
								{
								TR0=0;
								write_com(0x0f);
								write_com(0x80+0x40+7);
								
								}
								if(chnum==2)
								{
								write_com(0x0f);
								write_com(0x80+0x40+4);
								}
								if(chnum==3)
								{
								write_com(0x0f);
								write_com(0x80+0x40+1);
								}
								if(chnum==4)
								{
								write_com(0x0f);
								chnum=0;
								write_com(0x0c);
								TR0=1;
								}					
					}
					
				}

			if(chnum!=0)
			{
				if(zuo==0)
				{
					delay(5);
					if(zuo==0)
					{
									while(!zuo);
									if(chnum==1)
									{
										miao++;
										if(miao==60)
										miao=0;
										write_byte(0x40+6,0x30+miao/10);
										write_byte(0x40+7,0x30+miao%10);
										write_com(0x80+0x40+7);
									}
									if(chnum==2)
									{
										fen++;
										if(fen==60)
										fen=0;
										write_byte(0x40+3,0x30+fen/10);
										write_byte(0x40+4,0x30+fen%10);
										write_com(0x80+0x40+4);
									}
									if(chnum==3)
									{
											shi++;
											if(shi==24)
											shi=0;
											write_byte(0x40+0,0x30+shi/10);
											write_byte(0x40+1,0x30+shi%10);
											write_com(0x80+0x40+1);
									}
					}
				 }



											
											
											
											
															if(you==0)
															{
																delay(5);
																if(you==0)
																{
																				while(!you);
																				if(chnum==1)
																				{
																					miao--;
																					if(miao==-1)
																					miao=59;
																					write_byte(0x40+6,0x30+miao/10);
																					write_byte(0x40+7,0x30+miao%10);
																					write_com(0x80+0x40+7);
																				}
																				if(chnum==2)
																				{
																					fen--;
																					if(fen==-1)
																					fen=59;
																					write_byte(0x40+3,0x30+fen/10);
																					write_byte(0x40+4,0x30+fen%10);
																					write_com(0x80+0x40+4);
																				}
																				if(chnum==3)
																				{
																						shi--;
																						if(shi==-1)
																						shi=23;
																						write_byte(0x40+0,0x30+shi/10);
																						write_byte(0x40+1,0x30+shi%10);
																						write_com(0x80+0x40+1);
																				}
																}
															 }

			 }
									
				

			











}



void main()
{
init();


while(1)
{
keycheck();
}
}
void timer_0() interrupt 1
{
TH0=(65536-50000)/256;
TL0=(65536-50000)%256;
num++;
	if(num==20)
	{
	miao++;
				if(miao==60)
				{
				miao=0;
				fen++;
						if(fen==60)
						{
						fen=0;
						shi++;
					
							if(shi==24)
							{
							shi=0;
							}
							write_byte(0x40+0,0x30+shi/10);
							write_byte(0x40+1,0x30+shi%10);
						}
				write_byte(0x40+3,0x30+fen/10);
				write_byte(0x40+4,0x30+fen%10);
				}

		write_byte(0x40+6,0x30+miao/10);
		write_byte(0x40+7,0x30+miao%10);
		num=0;
	}

}

