#include<reg52.h>//52单片机头文件，规定了52单片机的寄存器和IO口等
#include<intrins.h>//_nop_空指令及左右循环移位子函数库 
#define uchar unsigned char//宏定义
#define uint unsigned int//宏定义
sbit lcden=P2^7;//定义下面通过lcden来操作P2^7口,1602液晶使能控制端
sbit lcdwr=P2^5;//定义下面通过lcdrw来操作P2^5口，1602读写选择端
sbit lcdrs=P2^6;//定义选数据寄存器选指令寄存器控制端
sbit sda=P3^4;//定义DS1302数据总线

sbit rst=P3^5;//DS1320复位
sbit sck=P3^6;//定义时钟总线
sbit s1=P1^0;//定义设置按钮
sbit s2=P1^1;//定义调时按钮
sbit s3=P1^2;//定义确定按钮
sbit s4=P1^3;
sbit ACC0=ACC^0;
sbit ACC7=ACC^7;
sbit BELL=P1^5;
char fen,shi,miao,ri,yue,nian,zhou,s1num,s2num,s4num,flag1,flag2,lshi,lfen;
uchar code table[]={" 2012-  -       "};//要写入1602液晶的数据
uchar code table1[]={"   :  :     :   "};//字库中的字可直接以外加""号的形式直接写入
uchar code table2[]={" Welcome To Lcd!"};//欢迎界面
uchar code table3[]={"Happy Every Day!"};	   //欢迎界面
/*******************************************************************************
时函数,z的取值为这个函数的延时ms数，delay(200);大约延时200ms. delay(500);大约延时500ms
*******************************************************************************/
void delay(uint z) 
{               					
	uint x,y;
	for(x=z;x>0;x--)
	for(y=110;y>0;y--);
}
/*********************蜂鸣器函数************************************************/
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

/*******************向1602液晶中写一个指令**************************************/
void write_com(uchar com)
{	
	lcdwr=0;			//lcdwr为读写控制端，lcdwr=0,这里可不写
	lcdrs=0;           //液晶rs接口为0时,写指令，rs为1时写数据
	P0=com;          //将要写的指令赋给P0口，
	delay(5);    //由1602读写操作时序图，先将指令赋给P0口，延时后将使能
	lcden=1;    //  端lcden置高，再延时一段时间，然后将lcden置低，这样指令
	delay(5);	  // 就写入到LCD了	
	lcden=0;
}
/****************************向液晶写数据***************************************/
void write_data(uchar date)//与写指令类似，这里lcdrs设为1
{
	lcdrs=1;
	P0=date;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
}
/*****************************初使化1602液晶************************************/
void init_1602()
{

	lcdwr=0;
	lcden=0;
	write_com(0x38);//设置LCD为16*2显示、5*7点阵、8位数据接口模式
	write_com(0x0c);//开显示、不显示光标
	write_com(0x06);//写一个字符后，地址指针加1
	write_com(0x01);//显示清0
}
/*******************************************************************************/
void gudingtime_1602()
{
    uchar num;
	write_com(0x80);//将指针指向初始位置
	for(num=0;num<16;num++)//循环函数，用于将" 2012-  -     "写入液晶
			write_data(table[num]);
	write_com(0x80+0x40);//将指针指向1602液晶的第二行
	for(num=0;num<16;num++)//功能与上同，用于将"   :  :     :   "写入
			write_data(table1[num]);
}
/**************************显示欢迎界面函数*************************************/
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
/***************************显示时间、日期子函数*********************************/
void write_sfm(uchar add,uchar time)//用于在1602上显示年、月、日、时、分、秒。Add为显示位置，time为要显示的内容
{                              
	uchar shiwei,gewei;						
	shiwei=time/16;   //将从DS1302中读取的BCD码数据转化成10进制个位和10 
	gewei=time%16;   //进制十位
	write_com(add+0x80);//定义显示在液晶的什么位置
	write_data(0x30+shiwei);//由1602液晶字库可知，0~9的数据码分别对应0x30~0x39
	write_data(0x30+gewei);	//初使化中设定了写一个字符后，地址指针加1，因此这里
}                     // 不用重新光标写位
/**************************显示周子函数*****************************************/
void write_zhou(uchar time1)//用于在1602上显示周信息，与显示
{                        //时间日期子函数类似
	uchar gewei;
	gewei=time1%16;//一周七天，因此只需个位
	write_com(0x80+13);
	write_data(0x30+gewei);
}
/***************************写数据字节子函数************************************/
void Input_1byte(uchar TD)//写一字节数据
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


/*************************读数据字节子函数**************************************/
uchar Output_1byte(void)//读一字节数据
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
/***************************写DS1302数据函数***********************************/
void write_1302(uchar DS_ADD,uchar DS_DAT)//写操作
{   
		 rst=0;
		 sck=0;  
		 rst=1;
		 Input_1byte(DS_ADD);
		 Input_1byte(DS_DAT);
		 sck=1;
		 rst=0;
}

/***************************读DS1302数据函数***********************************/
uchar read_1302(uchar DS_ADD)//读操作
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

/*************************初始化DS1302子函数***********************************/
void inital_1302()
{
    write_1302(0x8e,0x00);//禁止写保护
    write_1302(0x90,0xaa);//定义充电
	write_1302(0x80,0x53);//秒   
    write_1302(0x84,0x13);//时     
	write_1302(0x82,0x43);//分
	write_1302(0x8c,0x12);//年
	write_1302(0x88,0x06);//月
	write_1302(0x86,0x03);//日
	write_1302(0x8a,0x06);//星期
    write_1302(0xc0,0x08);//闹钟小时初始化
	write_1302(0xfc,0x00);//闹钟分钟初始化
	write_1302(0x8e,0x80);//开保护

}
/********************************************************************************
                              扫描函数
********************************************************************************/

void keyscan()
{  
	if(s1==0&&s4num==0)	//按键1按下且s4再次之前未曾按过
	{ 
		delay(5);
		if(s1==0&&s4num==0)
		{   
flag1=1;
		    s1num++;
			while(!s1);didi();
			if(s1num==1)
			{				
				write_com(0x80+0x40+8);//光标移动到**位置
				write_com(0x0f);//显示光标
			}

			if(s1num==2)
			{
			   	write_1302(0x8e,0x00);//禁止写保护
				write_1302(0x80,miao);//写入秒信息
				write_1302(0x8e,0x80);//开写保护
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
				write_com(0x0c);//不显示光标
			}
		}
	}	
/*******************************************************************************/	if(s1num!=0&&s4num==0)//按键1按下*次且s4再次之前未曾按过
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
						write_sfm(7+0x40,miao);//将修改的数送到1602显示
						write_com(0x80+0x40+8);//挪回光标
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

	if(s1num!=0||s4num!=0)// 在调整模式下
		{
			if(s3==0)//如果确定键按下
			{
				delay(5); 
				if(s3==0) //延时并重新检测S3是否按下，用于差小误差
				{
				   if(s1num!=0)
				   {
				    while(!s3);didi();//在松手时将调整后的时间、日期及周信息写入DS1302，退出调整模式并隐藏光标，按键次数清0
					write_1302(0x8e,0x00); 
				   	write_1302(0x80,miao);
				    write_1302(0x84,shi);
					write_1302(0x82,fen);
					write_1302(0x8a,zhou);
					write_1302(0x8c,nian);//年
					write_1302(0x88,yue);//月
					write_1302(0x86,ri);
					write_1302(0x8e,0x80);
					flag1=0;
					write_com(0x0c);
					s1num=0;
                   }
				   if(s4num!=0)
				   {
				   	while(!s3);didi();//在松手时将调整后的时间、日期及周信息写入DS1302，退出调整模式并隐藏光标，按键次数清0
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
 
   if(s4==0&&s1num==0)//按键4按下*次且s1再次之前未曾按过
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
					   	write_1302(0x8e,0x00);//禁止写保护
					    write_1302(0xfc,lfen);//闹钟初始化
						write_1302(0x8e,0x80);//开写保护
						write_com(0x80+0x40+11);
					}
					if(s4num==3)
					{   
					    s4num=0;
						flag2=0;
					   	write_1302(0x8e,0x00);//禁止写保护
						write_1302(0xc0,lshi);//闹钟初始化
						write_1302(0x8e,0x80);//开写保护
						write_com(0x0c);//不显示光标
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
   if((shi==lshi)&&(fen==lfen)&&(miao<=10&&miao>=0))//当闹钟的时和分分别与时钟的时和分相等时，闹钟时间到，蜂鸣器鸣响，鸣响10s
   {
       didi();
   }
}
void main()
{   
    init_1602();
	delay(5);
	displaystar();//初始化1602后马上显示欢迎语句
	delay(5000);//让欢迎界面停留一段时间
	gudingtime_1602();//显示液晶固定不变部分
    inital_1302();
	lshi=read_1302(0xc1);//从ds1302里面的RAM读取闹钟时，此处不放在while(1)大循环里面是因为ds1302里面的RAM里的数据是不变的只需读取一次
    lfen=read_1302(0xfd);//从ds1302里面的RAM读取闹钟分
    write_sfm(0x40+10,lshi);//将闹钟时送到1602显示
    write_sfm(0x40+13,lfen);//将闹钟分送到1602显示
	while(1)
		{       
		        keyscan();
				laba();
		        if(flag1==0&&flag2==0)//只有当不在调整的时候才从DS1302里面读取数据并且送到1602上显示
		    	{
				    miao=read_1302(0x81);//先读出，再送到1602显示，不断循环操作，使1602动态显示时间的走动
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

