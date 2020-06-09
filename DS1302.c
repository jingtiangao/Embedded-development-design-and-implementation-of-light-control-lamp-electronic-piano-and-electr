#include<reg52.h>//52��Ƭ��ͷ�ļ����涨��52��Ƭ���ļĴ�����IO�ڵ�
#include<intrins.h>//_nop_��ָ�����ѭ����λ�Ӻ����� 
#define uchar unsigned char//�궨��
#define uint unsigned int//�궨��
sbit lcden=P2^7;//��������ͨ��lcden������P2^7��,1602Һ��ʹ�ܿ��ƶ�
sbit lcdwr=P2^5;//��������ͨ��lcdrw������P2^5�ڣ�1602��дѡ���
sbit lcdrs=P2^6;//����ѡ���ݼĴ���ѡָ��Ĵ������ƶ�
sbit sda=P3^4;//����DS1302��������

sbit rst=P3^5;//DS1320��λ
sbit sck=P3^6;//����ʱ������
sbit s1=P1^0;//�������ð�ť
sbit s2=P1^1;//�����ʱ��ť
sbit s3=P1^2;//����ȷ����ť
sbit s4=P1^3;
sbit ACC0=ACC^0;
sbit ACC7=ACC^7;
sbit BELL=P1^5;
char fen,shi,miao,ri,yue,nian,zhou,s1num,s2num,s4num,flag1,flag2,lshi,lfen;
uchar code table[]={" 2012-  -       "};//Ҫд��1602Һ��������
uchar code table1[]={"   :  :     :   "};//�ֿ��е��ֿ�ֱ�������""�ŵ���ʽֱ��д��
uchar code table2[]={" Welcome To Lcd!"};//��ӭ����
uchar code table3[]={"Happy Every Day!"};	   //��ӭ����
/*******************************************************************************
ʱ����,z��ȡֵΪ�����������ʱms����delay(200);��Լ��ʱ200ms. delay(500);��Լ��ʱ500ms
*******************************************************************************/
void delay(uint z) 
{               					
	uint x,y;
	for(x=z;x>0;x--)
	for(y=110;y>0;y--);
}
/*********************����������************************************************/
void didi() 
{  	
    uchar i;
     for(i=0;i<60;i++)
      {
        BELL = 0;
		delay(1);
     	BELL = 1;
		delay(1);
       }
}

/*******************��1602Һ����дһ��ָ��**************************************/
void write_com(uchar com)
{	
	lcdwr=0;			//lcdwrΪ��д���ƶˣ�lcdwr=0,����ɲ�д
	lcdrs=0;           //Һ��rs�ӿ�Ϊ0ʱ,дָ�rsΪ1ʱд����
	P0=com;          //��Ҫд��ָ���P0�ڣ�
	delay(5);    //��1602��д����ʱ��ͼ���Ƚ�ָ���P0�ڣ���ʱ��ʹ��
	lcden=1;    //  ��lcden�øߣ�����ʱһ��ʱ�䣬Ȼ��lcden�õͣ�����ָ��
	delay(5);	  // ��д�뵽LCD��	
	lcden=0;
}
/****************************��Һ��д����***************************************/
void write_data(uchar date)//��дָ�����ƣ�����lcdrs��Ϊ1
{
	lcdrs=1;
	P0=date;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
}
/*****************************��ʹ��1602Һ��************************************/
void init_1602()
{

	lcdwr=0;
	lcden=0;
	write_com(0x38);//����LCDΪ16*2��ʾ��5*7����8λ���ݽӿ�ģʽ
	write_com(0x0c);//����ʾ������ʾ���
	write_com(0x06);//дһ���ַ��󣬵�ַָ���1
	write_com(0x01);//��ʾ��0
}
/*******************************************************************************/
void gudingtime_1602()
{
    uchar num;
	write_com(0x80);//��ָ��ָ���ʼλ��
	for(num=0;num<16;num++)//ѭ�����������ڽ�" 2012-  -     "д��Һ��
			write_data(table[num]);
	write_com(0x80+0x40);//��ָ��ָ��1602Һ���ĵڶ���
	for(num=0;num<16;num++)//��������ͬ�����ڽ�"   :  :     :   "д��
			write_data(table1[num]);
}
/**************************��ʾ��ӭ���溯��*************************************/
void displaystar(void) 
{
	 uchar i;
	 write_com(0x80);
	 for(i=0;i<16;i++)
	 write_data(table2[i]);
	 write_com(0x80+0x40);
	 for(i=0;i<16;i++)
 	 write_data(table3[i]);
}
/***************************��ʾʱ�䡢�����Ӻ���*********************************/
void write_sfm(uchar add,uchar time)//������1602����ʾ�ꡢ�¡��ա�ʱ���֡��롣AddΪ��ʾλ�ã�timeΪҪ��ʾ������
{                              
	uchar shiwei,gewei;						
	shiwei=time/16;   //����DS1302�ж�ȡ��BCD������ת����10���Ƹ�λ��10 
	gewei=time%16;   //����ʮλ
	write_com(add+0x80);//������ʾ��Һ����ʲôλ��
	write_data(0x30+shiwei);//��1602Һ���ֿ��֪��0~9��������ֱ��Ӧ0x30~0x39
	write_data(0x30+gewei);	//��ʹ�����趨��дһ���ַ��󣬵�ַָ���1���������
}                     // �������¹��дλ
/**************************��ʾ���Ӻ���*****************************************/
void write_zhou(uchar time1)//������1602����ʾ����Ϣ������ʾ
{                        //ʱ�������Ӻ�������
	uchar gewei;
	gewei=time1%16;//һ�����죬���ֻ���λ
	write_com(0x80+13);
	write_data(0x30+gewei);
}
/***************************д�����ֽ��Ӻ���************************************/
void Input_1byte(uchar TD)//дһ�ֽ�����
{
    uchar i;
    ACC=TD;
    for(i=8;i>0;i--)
    {
       sda=ACC0;
       sck=1;
       sck=0;
       ACC=ACC>>1; 
    }
}


/*************************�������ֽ��Ӻ���**************************************/
uchar Output_1byte(void)//��һ�ֽ�����
{
    uchar i;
    for(i=8;i>0;i--)
     {
         ACC=ACC>>1;
         ACC7=sda;
         sck=1;
         sck=0;
     }

     return(ACC);

}
/***************************дDS1302���ݺ���***********************************/
void write_1302(uchar DS_ADD,uchar DS_DAT)//д����
{   
		 rst=0;
		 sck=0;  
		 rst=1;
		 Input_1byte(DS_ADD);
		 Input_1byte(DS_DAT);
		 sck=1;
		 rst=0;
}

/***************************��DS1302���ݺ���***********************************/
uchar read_1302(uchar DS_ADD)//������
{   
	  uchar DS_INF;
	  rst=0;
	  sck=0;  
	  rst=1;
	  Input_1byte(DS_ADD);
	  DS_INF=Output_1byte();
	  sck=1;
	  rst=0;
	  return(DS_INF);
}

/*************************��ʼ��DS1302�Ӻ���***********************************/
void inital_1302()
{
    write_1302(0x8e,0x00);//��ֹд����
    write_1302(0x90,0xaa);//������
	write_1302(0x80,0x53);//��   
    write_1302(0x84,0x13);//ʱ     
	write_1302(0x82,0x43);//��
	write_1302(0x8c,0x12);//��
	write_1302(0x88,0x06);//��
	write_1302(0x86,0x03);//��
	write_1302(0x8a,0x06);//����
    write_1302(0xc0,0x08);//����Сʱ��ʼ��
	write_1302(0xfc,0x00);//���ӷ��ӳ�ʼ��
	write_1302(0x8e,0x80);//������

}
/********************************************************************************
                              ɨ�躯��
********************************************************************************/

void keyscan()
{  
	if(s1==0&&s4num==0)	//����1������s4�ٴ�֮ǰδ������
	{ 
		delay(5);
		if(s1==0&&s4num==0)
		{   
flag1=1;
		    s1num++;
			while(!s1);didi();
			if(s1num==1)
			{				
				write_com(0x80+0x40+8);//����ƶ���**λ��
				write_com(0x0f);//��ʾ���
			}

			if(s1num==2)
			{
			   	write_1302(0x8e,0x00);//��ֹд����
				write_1302(0x80,miao);//д������Ϣ
				write_1302(0x8e,0x80);//��д����
				write_com(0x80+0x40+5);
			}
			if(s1num==3)
			{
			   	write_1302(0x8e,0x00);
				write_1302(0x82,fen);
				write_1302(0x8e,0x80);
				write_com(0x80+0x40+2);								
			}
			if(s1num==4)
			{
				write_1302(0x8e,0x00);
				write_1302(0x88,yue);
				write_1302(0x8e,0x80);
			    write_com(0x80+13);
			}
			if(s1num==5)
			{
				write_1302(0x8e,0x00);
				write_1302(0x84,shi);
				write_1302(0x8e,0x80);
				write_com(0x80+10);
			}
			if(s1num==6)
			{
				write_1302(0x8e,0x00);
				write_1302(0x86,ri);
				write_1302(0x8e,0x80);
				write_com(0x80+7);
			}
			if(s1num==7)
			{
				write_1302(0x8e,0x00);
				write_1302(0x8c,nian);
				write_1302(0x8e,0x80);
				write_com(0x80+4);
			}
            if(s1num==8)
			{  
		        flag1=0;
				s1num=0;
				write_1302(0x8e,0x00);
				write_1302(0x8a,zhou);
				write_1302(0x8e,0x80);
				write_com(0x0c);//����ʾ���
			}
		}
	}	
/*******************************************************************************/	if(s1num!=0&&s4num==0)//����1����*����s4�ٴ�֮ǰδ������
		{
			if(s2==0)
			{
				delay(5);
				if(s2==0)
				{   
					while(!s2);didi();
					if(s1num==1)
					{
						int x1,x2;
						x1=miao%16;
						x2=miao/16;
						x1++;
						if(x1==10)
						{
							x1=0;
							x2++;
							if(x2>=6)
								x2=0;
						}
						miao=x1+x2*16;
						write_sfm(7+0x40,miao);//���޸ĵ����͵�1602��ʾ
						write_com(0x80+0x40+8);//Ų�ع��
					}
					if(s1num==2)
					{
						int x3,x4;
						x3=fen%16;
						x4=fen/16;
						x3++;
						if(x3==10)
						{
							x3=0;
							x4++;
							if(x4>=6)
								x4=0;
						}
						fen=x3+x4*16;
						write_sfm(4+0x40,fen);
						write_com(0x80+0x40+5);
					}	
					if(s1num==3)
					{
						int x5,x6;
						x5=shi%16;
						x6=shi/16;
						x5++;
						 if(x6>=2&&x5>=4)	
						{
							x5=0;
							x6=0;
						}
						if(x5==10)
						{
							x5=0;
							x6++;
						}
						shi=x5+x6*16;
						write_sfm(1+0x40,shi);
						write_com(0x80+0x40+2);
					}
					if(s1num==4)
					{
						 zhou++;
						 if(zhou==8)
						 zhou=1;
						 write_zhou(zhou);
						 write_com(0x80+13);
						 
			        }
					if(s1num==5)
					{
						int x5,x6;
						x5=ri%16;
						x6=ri/16;
						x5++;
						if(x6>=3&&x5>=1)	
						{
							x5=1;
							x6=0;
						}
						if(x5==10)
						{
							x5=0;
							x6++;
						}
						ri=x5+x6*16;
						write_sfm(9,ri);
						write_com(0x80+10);
					}
					if(s1num==6)
					{
						int x5,x6;
						x5=yue%16;
						x6=yue/16;
						x5++;
						if(x6>=1&&x5>=3)	
						{
							x5=1;
							x6=0;
						}
						if(x5==10)
						{
							x5=0;
							x6++;
						}
						yue=x5+x6*16;
						write_sfm(6,yue);
						write_com(0x80+7);
			        }
					if(s1num==7)
					{
						int x5,x6;
						x5=nian%16;
						x6=nian/16;
						x5++;
						if(x6>=2&&x5>=1)	
						{
							x5=1;
							x6=0;
						}
						if(x5==10)
						{
							x5=0;
							x6++;
						}
						nian=x5+x6*16;
						write_sfm(3,nian);
						write_com(0x80+4);
			        }
				}
			}
	    } 
/*******************************************************************************/

	if(s1num!=0||s4num!=0)// �ڵ���ģʽ��
		{
			if(s3==0)//���ȷ��������
			{
				delay(5); 
				if(s3==0) //��ʱ�����¼��S3�Ƿ��£����ڲ�С���
				{
				   if(s1num!=0)
				   {
				    while(!s3);didi();//������ʱ���������ʱ�䡢���ڼ�����Ϣд��DS1302���˳�����ģʽ�����ع�꣬����������0
					write_1302(0x8e,0x00); 
				   	write_1302(0x80,miao);
				    write_1302(0x84,shi);
					write_1302(0x82,fen);
					write_1302(0x8a,zhou);
					write_1302(0x8c,nian);//��
					write_1302(0x88,yue);//��
					write_1302(0x86,ri);
					write_1302(0x8e,0x80);
					flag1=0;
					write_com(0x0c);
					s1num=0;
                   }
				   if(s4num!=0)
				   {
				   	while(!s3);didi();//������ʱ���������ʱ�䡢���ڼ�����Ϣд��DS1302���˳�����ģʽ�����ع�꣬����������0
                    write_1302(0x8e,0x00); 
   				    write_1302(0xc0,lshi);
                    write_1302(0xfc,lfen);
					write_1302(0x8e,0x80);
					flag2=0;
					s4num=0;
					write_com(0x0c);
                   }

				}
			}	
        }
 
   if(s4==0&&s1num==0)//����4����*����s1�ٴ�֮ǰδ������
        {  
		     
              delay(5);
  			  if(s4==0&&s1num==0)
			  {    
		            s4num++;
					flag2=1;
                    while(!s4);didi();
			      	if(s4num==1)
					{				
										
						write_com(0x80+0x40+14);
						write_com(0x0f);
					}
		   			  
		  			if(s4num==2)
					{
					   	write_1302(0x8e,0x00);//��ֹд����
					    write_1302(0xfc,lfen);//���ӳ�ʼ��
						write_1302(0x8e,0x80);//��д����
						write_com(0x80+0x40+11);
					}
					if(s4num==3)
					{   
					    s4num=0;
						flag2=0;
					   	write_1302(0x8e,0x00);//��ֹд����
						write_1302(0xc0,lshi);//���ӳ�ʼ��
						write_1302(0x8e,0x80);//��д����
						write_com(0x0c);//����ʾ���
					}
			  }
		}
	
			if(s4num!=0&&s1num==0)
			{
				if(s2==0)
				{
					delay(5);
					if(s2==0)
					{   
						while(!s2);didi();
						if(s4num==1)
						{
							int x1,x2;
							x1=lfen%16;
							x2=lfen/16;
							x1++;
							if(x1==10)
							{
								x1=0;
								x2++;
								if(x2>=6)
									x2=0;
							}
							lfen=x1+x2*16;
							write_sfm(13+0x40,lfen);
							write_com(0x80+0x40+14);
	                    }
						if(s4num==2)
						{
							int x5,x6;
							x5=lshi%16;
							x6=lshi/16;
							x5++;
							 if(x6>=2&&x5>=4)	
							{
								x5=0;
								x6=0;
							}
							if(x5==10)
							{
								x5=0;
								x6++;
							}
							lshi=x5+x6*16;
							write_sfm(10+0x40,lshi);
							write_com(0x80+0x40+11);
						}
	                }
	            }
	        }
} 
void laba()
{  
   if((shi==lshi)&&(fen==lfen)&&(miao<=10&&miao>=0))//�����ӵ�ʱ�ͷֱַ���ʱ�ӵ�ʱ�ͷ����ʱ������ʱ�䵽�����������죬����10s
   {
       didi();
   }
}
void main()
{   
    init_1602();
	delay(5);
	displaystar();//��ʼ��1602��������ʾ��ӭ���
	delay(5000);//�û�ӭ����ͣ��һ��ʱ��
	gudingtime_1602();//��ʾҺ���̶����䲿��
    inital_1302();
	lshi=read_1302(0xc1);//��ds1302�����RAM��ȡ����ʱ���˴�������while(1)��ѭ����������Ϊds1302�����RAM��������ǲ����ֻ���ȡһ��
    lfen=read_1302(0xfd);//��ds1302�����RAM��ȡ���ӷ�
    write_sfm(0x40+10,lshi);//������ʱ�͵�1602��ʾ
    write_sfm(0x40+13,lfen);//�����ӷ��͵�1602��ʾ
	while(1)
		{       
		        keyscan();
				laba();
		        if(flag1==0&&flag2==0)//ֻ�е����ڵ�����ʱ��Ŵ�DS1302�����ȡ���ݲ����͵�1602����ʾ
		    	{
				    miao=read_1302(0x81);//�ȶ��������͵�1602��ʾ������ѭ��������ʹ1602��̬��ʾʱ����߶�
					fen=read_1302(0x83);
					shi=read_1302(0x85);
					ri=read_1302(0x87);
					yue=read_1302(0x89);
					zhou=read_1302(0x8b);
		         	nian=read_1302(0x8d);
				    write_sfm(7+0x40,miao);
					write_sfm(4+0x40,fen);
					write_sfm(1+0x40,shi);
					write_sfm(9,ri);
					write_sfm(6,yue);
					write_sfm(3,nian);
					write_zhou(zhou);
		        }
}
}

