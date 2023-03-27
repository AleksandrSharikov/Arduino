#include <EEPROM.h>
//#include <LiquidCrystal.h>
#include <max6675.h>
//#include "max6675.h"
#include <iarduino_OLED_txt.h>                         // Подключаем библиотеку iarduino_OLED_txt.
iarduino_OLED_txt myOLED(0x78);
extern uint8_t MediumFontRus[];   
extern uint8_t SmallFontRus[];// Подключаем шрифт SmallFontRus.

//-----------------------------------------------------------variables--------------------------------------------------------------------
//pins
byte TG=16;// gotov
byte TRPin=17;// termoresistor analog
byte UPin=15; // pit
byte KB=5; // kalibrovka
byte KL=14; // Light kalibr
byte thermoDO = 8; //
byte thermoCS = 7;  //  termocouple pins
byte thermoCLK = 6;  //
byte control=9; // controller connected
byte CLK=4;//valcoder
byte DT=3;
byte SW=2;

//constants
float TS=60; //seted temp
byte Tmax=121; // termination of colibration
long unsigned toe=10000; //timout encoder
byte Rft=24;//resistor of devider
byte DR=3; //gate
float f1024=1024;
byte kb_delay=200; //holding kb time
byte Ru_off=100; // overheat
int dtimer=1000;//frequency of switch

//EEPROM adresses
byte shift_adr=3; //adres shift storage
byte temset=5;// place in eeprom where is seted temp
byte Ru_adr=8;// place for Ru without calibr table
byte beg=30; //table start



//flags
byte rpodbor_fl=0, podbor_fl=0; //search and reverse serch error flags
byte kl_flag=0; // flag kalibrkvki
byte fl;    //diod flagbyte 
byte ee_flag=0; //EEPromt calib flag

//buttons
byte CT=0;//control connected
unsigned char encoder_A, encoder_B, encoder_A_prev; // valcoder
unsigned char SW_State;
byte kb;   //нажатие калибровки
byte kb_hold=0; // holding kb

// boxes map
byte boxnum=5; //number of boxes
byte wid=18; //box's wide 
byte KT_box=0;
byte TS_box=1;
byte SHIFT_box=2;
byte U_box=3;
byte TG_box=4;

//unsorted
int Ru; //set mesure
int Rt; //now mesure
//int Rfu=20;
//float ROmh; // resistance of balance resistor
int dR;
//int k[2][80]; // kalibr table 0-t;1-r
struct call{byte t; int r;};    //srtucture calibr
call k[100];
int temp; // aproximately temp
int tempset;
byte ts1;  // TS=ts1+ts2/10
byte ts2;
byte ts1p;   // previous ts1
byte num; //number where TS is
float Ttemp;
int atest; //shift test
int test;// test ratio
int test2;
long timer1,timer2;//main counter


//tmp
int err2=0;


//-------------------------------------------------------------SETUP--------------------------------------------------------
void setup() {
  
    
  pinMode(TG,OUTPUT);  //Gotov
  digitalWrite(TG,LOW);
  pinMode(UPin,OUTPUT); //Pitanie
  digitalWrite(UPin,LOW);
  pinMode(KB,INPUT);  //Calibrovka
  digitalWrite(KB,LOW);
  pinMode(control,INPUT);  //Control
  digitalWrite(control,LOW);
  pinMode(KL,OUTPUT); //Lampa
  digitalWrite(KL,LOW);
  pinMode(TRPin,INPUT);  //Calibrovka
  digitalWrite(TRPin,LOW);

//CT=HIGH;
CT=digitalRead(control);
if(CT==HIGH)
{
  myOLED.begin();                                  
    myOLED.clrScr(); 
 }
  fl=0;
  // Check if there is a calibration table
 checkcall();
  myOLED.clrScr(); 
  }

//-------------------------------------------------------------LOOP---------------------------------------------

void loop()                                                                    //+
{ 
  timer2=millis();
  
if(CT==HIGH)
{
  if(kl_flag==1)
    {encoder();
    if(timer2-timer1>dtimer)
    {mainloopview();
    timer1=millis();}}
  if(kl_flag==2)
    {if(timer2-timer1>dtimer)
    {mainloopview_bk();
    timer1=millis();} 
    encoder_bk(); }  

kb_push();
}  
else
    if(timer2-timer1>dtimer)
    {mainloop();
    timer1=millis();} 

  //delay(1000);
  /*if(Ru<Ru_off)
  while(1<2)
  {
    digitalWrite(UPin,LOW);
    blinc(1,10);
    blinc(5,2);}*/
}


        //---------------------------------------------------------FUNCTIONS-------------------------------------------------------------

void mainloop()                                                       //main loop function     +
{

  
Rt=analogRead(TRPin);
Ttemp=getTR(Rt);
dR=abs(Ru-Rt);

if(Rt>Ru)                 //pitanie
digitalWrite(UPin,HIGH);
else
digitalWrite(UPin,LOW);
                            //gotov
if(dR<DR)
digitalWrite(TG,HIGH);
else
digitalWrite(TG,LOW);
/*
if(fl==1)
digitalWrite(KL,HIGH);
else
digitalWrite(KL,LOW);*/

  }
 
void mainloopview()                                    //main loop with control block attached function  +
{
   
Rt=analogRead(TRPin);
LCD(Rt,'R');
Ttemp=getTR(Rt);
LCD(Ttemp,'C');

dR=abs(Ru-Rt);

if(Rt>Ru)                 //pitanie
{digitalWrite(UPin,HIGH);
LCD(1,'U');}
else
{digitalWrite(UPin,LOW);
LCD(0,'U');}

                            //gotov
if(dR<DR)
{digitalWrite(TG,HIGH);
LCD(1,'G');}
else
{digitalWrite(TG,LOW);
LCD(0,'G');}

if(fl==1)
digitalWrite(KL,HIGH);
else
digitalWrite(KL,LOW);

testTP();  
  }

void mainloopview_bk()                                           //main loop without calibration  +
{

Rt=analogRead(TRPin);
LCD(Rt,'R');

LCD(Rt,'C');

dR=abs(Ru-Rt);

if(Rt>Ru)                 //pitanie
{digitalWrite(UPin,HIGH);
LCD(1,'U');}
else
{digitalWrite(UPin,LOW);
LCD(0,'U');}

                            //gotov
if(dR<DR)
{digitalWrite(TG,HIGH);
LCD(1,'G');}
else
{digitalWrite(TG,LOW);
LCD(0,'G');}

if(fl==1)
digitalWrite(KL,HIGH);
else
digitalWrite(KL,LOW);

testTP();  
  }

void Kalibrovka()                                                       //kalibrovka  + -
{
 
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

float t1=0;
 byte unsigned t2=0;
 byte unsigned tm=0;
int termtmp;
long unsigned tim;
long unsigned tim1;
byte i=0;
long unsigned dtim=540000;      // не повысилось на градус за 3 минуты

     LCD(1,'K');
    delay(3000);     

//EEPROM.update(0,0);
for(i=0;i<100;i++)   //обнуление массива
{k[i].t=0;
k[i].r=0;}
digitalWrite(KL,LOW);
fl=0;
//pinMode(vccPin,OUTPUT); //termocouple on
//digitalWrite(vccPin,HIGH);
delay(1000);
t1=thermocouple.readCelsius();
t2=floor(t1);
digitalWrite(UPin,HIGH);
tim=millis();
tim1=tim;
i=0;

while(t1<Tmax&&tim<dtim&&i<101)         //degre complate
{
  tim=millis()-tim1;
  t1=thermocouple.readCelsius();
  t2=floor(t1);
  termtmp=analogRead(TRPin);
  LCD(t1,'c');
  LCD(termtmp,'O');
  
  if(t2>tm)
  {
k[i].t=t2;
k[i].r=analogRead(TRPin);
tim1=millis();  
i++;
tm=t2;
LCD(i,'i');
  }
  LCD(tim,'q');
  

delay(1500);
  }


  
LCD(0,'K'); 

           myOLED.print(i,OLED_L,4); 
           myOLED.print(tim,OLED_C,4);
           myOLED.print(t1,OLED_R,4);  
           if (i>99)
           myOLED.print("^^^^^",OLED_L,5);
           if (tim>=dtim)
           myOLED.print("^^^^^",OLED_C,5);
           if (t1>=Tmax)
           myOLED.print("^^^^^",OLED_R,5);
digitalWrite(KL,HIGH);
fl=1;
kl_flag=1;    
eewrite();
while(!digitalRead(KB))
{}
myOLED.clrScr(); 
  }
 
byte podbor(float TEM)                                                  //serch     +
  {            
   
                                      
  int i=0;
  byte t;
  if(podbor_fl==1)
  //myOLED.print("               ",OLED_C,6);
  podbor_fl=0;
  t=floor(TEM);
  if (t<k[0].t)
    {LCD(2,'E'); 
    podbor_fl=1;}
    else
    while(t>k[i].t)
    i++;     
  //  myOLED.print( "+"    ,   OLED_L, 6); 
    return i;
  }
  
byte rpodbor(int r)                                                    //revers serch   +
  {                                                
  int i=0;
 // if(rpodbor_fl==1)
 // myOLED.print("               ",OLED_C,6);
  rpodbor_fl=0;
  if (r>k[0].r)
  {LCD(3,'E');
       rpodbor_fl=1;}
    else
    while(r<k[i].r)
    i++;   
    return i;
  }

void eeread()                                                         //чтение из ЕЕ    +
{

byte i=0;
int sh=0;
ee_flag=EEPROM.read(0);
if(ee_flag==1)
{
for(i=0;i<100;i++)
{k[i].t=0;
k[i].r=0;}
i=0;
 EEPROM.get(beg,k);
 EEPROM.get(shift_adr,sh);
 if(sh>-15&&sh<15)
 {shift(sh);
LCD(2,'A');}

else
LCD(0,'A');
  kl_flag=1;
}
else
{
    LCD(1,'E');
while(1<2)
blinc(5,100);
}
  }

void eewrite()                                                        //Запись в ЕЕ   +
{      
  EEPROM.put(beg,k);
  EEPROM.update(0,1);
  }
   

void blinc(int sec, int times)                                         //blink   +
  {
    int i;
   for (i=0;i<=times;i++)
   {
    digitalWrite(KL,HIGH);
    delay(sec*100);
    digitalWrite(KL,LOW);
    delay(sec*100);
   }
    
    }

                                                                     // encoder + -
void encoder()
 {
          
   //    myOLED.print( "              "    ,   OLED_C, 0); 
   //       myOLED.print( "void encoder()"    ,   OLED_C, 0);
         long unsigned te0;
         long unsigned te1;
         long unsigned ted;
         byte mfl=0;
          
 
  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
  SW_State = digitalRead(SW);

 if(!encoder_A)
   {
 
  encoder_A_prev = HIGH;
 myOLED.print("VK",(wid+4),0);
te0=millis();
 while(ted<toe)
 {

  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
  kb=digitalRead(KB);
  

  te1=millis();
  ted=te1-te0;
  if(!encoder_A && encoder_A_prev)
  {
   
    if(encoder_B)
    {
       ts1p=floor(TS);
      TS=TS-0.2;
      if(kb==HIGH)
      TS=TS-9.8;
      if(TS<0)
      TS=0;
      LCD(TS,'S');
     te0=millis();
    }
       else
    {
      ts1p=floor(TS);
      TS=TS+0.2;
      if(kb==HIGH)
      TS=TS+9.8;
      if(TS>Tmax)
      TS=Tmax;
     LCD(TS,'S');
       te0=millis();
      }
  }
    encoder_A_prev = encoder_A;


/*  if(!SW_State)
  if(mfl==0)
  {
    myOLED.print("+",OLED_R,6);
    //Если переменная SW_State установлена в false то установить её в true 
    //if(SW_State)             //if valcoder is pressed write temp to eeprom
    {
      //ts1=floor(TS);
     // ts2=(TS-ts1)*10;
      EEPROM.put(temset,TS);
     // EEPROM.write(temset+1,ts2);
    myOLED.print( "*" , (wid+1)+3, 0);
      te0=millis();
      mfl=1;
      //И запомнить состояние
     // SW_State = true;
    }
    //И наоборот - если переменная SW_State установлена в true, 
    //то сбросить её в false 
  
  } */
 } //cicle ends
        LCD(1258,'0');
       ts1=floor(TS);
       ts2=TS-ts1;
   
       num=podbor(TS);
       Ru=k[num].r-(k[num].r-k[num+1].r)*ts2;
       LCD(Ru,'r');
   
       EEPROM.put(temset,TS);
       LCD(1,'1');
       
     
  }
 if (SW_State==0)
 shift_now();
}              
                                                              //encoder without calibration table
void encoder_bk()
 {
         long unsigned te0;
         long unsigned te1;
         long unsigned ted;
         byte mfl=0;
          
 
  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
  
  //SW_State = digitalRead(SW);

    
 
 if(!encoder_A)
   {
    
  encoder_A_prev = HIGH;
 myOLED.print("NT",(wid+4),0);
te0=millis();
 while(ted<toe)
 {
  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
  kb=digitalRead(KB);

  te1=millis();
  ted=te1-te0;
  if(!encoder_A && encoder_A_prev)
  {
    if(encoder_B)
    {
      Ru=Ru-1;
      if(kb==HIGH)
      Ru=Ru-99;
      if(Ru<0)
      Ru=1024;
     /*  ts1p=floor(TS);
      TS=TS-0.2;*/
      LCD(Ru,'b');
     te0=millis();
    }
       else
    {
      Ru=Ru+1;
      if(kb==HIGH)
      Ru=Ru+99;
      if(Ru>1024)
      Ru=0;
    /*  ts1p=floor(TS);
      TS=TS+0.2;*/
     LCD(Ru,'b');
       te0=millis();
      }
  }
    encoder_A_prev = encoder_A;

 } //cicle ends
        LCD(1258,'0');
      LCD(Ru,'r');
       LCD(Ru,'S');
   
       EEPROM.put(Ru_adr,Ru);
       EEPROM.update(0,2);
       LCD(2,'1');
       
     
  }
 /*if (SW_State==0)
 shift_now();*/
}              

 void checkcall()                                                  //check ct     +
 {
  //byte i;
   ee_flag=EEPROM.read(0);  
  /* if(CT==HIGH)   
   {SW_State=digitalRead(SW);
           if(!SW_State)
          {}
          // kl_flag=4;
           else
           kl_flag==ee_flag;}
   else        */
   kl_flag=ee_flag;
   if(kl_flag==1) 
  {
    eeread();        
       EEPROM.get(temset,TS);
       if(TS<20||TS>120)
       TS=85;
       ts1=floor(TS);
       ts2=TS-ts1;
      // if(ts1p!=ts1)  
    //  myOLED.print( "      "    ,   OLED_L, 6); 
       num=podbor(TS);
       Ru=k[num].r-(k[num].r-k[num+1].r)*ts2;
             
       if(CT==HIGH)
       {
        LCD(0,'0');
        LCD(TS,'S');
        LCD(Ru,'r');
        LCD(1,'B');           
       } 
  }    
  if(kl_flag==2)
  {
    
    EEPROM.get(Ru_adr,Ru);
    if(Ru<2||Ru>1020)
    Ru=500;
    if(CT==HIGH)
   { myOLED.clrScr(); 
    LCD(Ru,'S');}
    }
       if(kl_flag!=1&&kl_flag!=2)
        {
              while(CT==LOW)
              {
        blinc(2,50);
        CT=digitalRead(control);}
        if(CT==HIGH)        
         while(kl_flag!=1&&kl_flag!=2)  //                
        {   
        LCD(3,'K');
        kb=digitalRead(KB); 
        //kb_push();
        SW_State = digitalRead(SW);     
        if(!SW_State)  
        kl_flag=2;
        if(kb==HIGH)
        delay(500); 
        kb=digitalRead(KB); 
        if(kb==HIGH)
        delay(500);
        kb=digitalRead(KB); 
        if(kb==HIGH)
        Kalibrovka();
        }             
}
}                     

  float getTR(int R)                                             // ger T from R    +
  {
            
      // myOLED.print( "              "    ,   OLED_C, 0); 
      //    myOLED.print( "float getTR(int R)"    ,   OLED_C, 0);
    byte num;
    float T;
    float pose;
    num=rpodbor(R);
   pose=(k[num].r-R)/(k[num].r-k[num+1].r);
   T=k[num].t+(k[num+1].t-k[num].t)*pose;
   return T;
    }

void testTP()                                                //termocouple test  +
{
if(test==10)
{float testtemp;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
delay(2000);
testtemp=thermocouple.readCelsius();
LCD(testtemp,'T');
 
test=0;
//test2++;
  }
  test++;
  
}

void shift(int shift)                                       //shift calibtation table   +
{
  int i;
  for(i=0;i<100;i++)
  k[i].t=k[i].t+shift;
  }

void shift_now()                                      // align temperature to current one    +
  {
    float TR;
    float TT;
    int DT;
    int R;
    MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
    delay(2000);
TT=thermocouple.readCelsius();
R=analogRead(TRPin);
TR=getTR(R);
DT=TT-TR;
shift(DT);
EEPROM.put(shift_adr,DT);
LCD(1,'A');
   ts1=floor(TS);
       ts2=TS-ts1;
   
       num=podbor(TS);
       Ru=k[num].r-(k[num].r-k[num+1].r)*ts2;
       LCD(Ru,'r');
   
       EEPROM.put(temset,TS);
       LCD(1,'1');
   
    //myOLED.print(atest,OLED_L,6);
    //myOLED.print(SW_State,OLED_R,6);
   // atest++;
    
    }

void kb_push() //                                              pushing KB
    {
      //if(kl_flag!=1)
      //kb_delay=100;
    kb=digitalRead(KB);
    encoder_A = digitalRead(CLK);
    if(kb_hold!=0)
    if(kb==LOW||!encoder_A)
    {kb_hold=0;
    LCD(22,'0');}
    if(kb==HIGH)   //if kb pressed
    {kb_hold++;
    LCD(kb_hold,'k');}
    if(kb_hold==kb_delay)
      {kb_hold=0;
        digitalWrite(UPin,LOW);
      digitalWrite(TG,LOW);         
      Kalibrovka(); 
     checkcall();
      }
     }

void LCD(float dat,char n)//                                 displey on LCD     +
    {
      /*
       n=0 Table and voids
       n=S Set temp
       n=C Current temp  //by R
       n=r Set R
       n=R Current R
       n=T Current temp // by Termocouple
       n=U Supply flag
       n=G Gotov flag
       n=A Align flag
       n=E error
       n=K calibration
       n=i calibration number
       n=q time in calibration
       n=c Calibration termocouple
       n=O Romh in calibration
       n=B KT_flag
       n=1 TS mamory flag
       n=k dackcount for calibration
       n=b Set resistance
       */
      if(CT==HIGH)
      {
if(n=='S')
        {
          myOLED.setFont(MediumFontRus);
          myOLED.print("     ",OLED_L,4);
          myOLED.print("     ",OLED_R,4);
          myOLED.print(dat,OLED_L,4,1);
          }
if(n=='C')
        {
          myOLED.setFont(MediumFontRus);
          myOLED.print(dat,OLED_R,4);
          }
if(n=='T')
        {
          myOLED.setFont(SmallFontRus);
          myOLED.print("     ",OLED_C,7);
          myOLED.print(dat,OLED_C,7);
          }
if(n=='r')
        {
          myOLED.setFont(SmallFontRus);
          myOLED.print("     ",OLED_L,7);
           myOLED.print("     ",OLED_R,7);
          myOLED.print(dat,OLED_L,7);
          }
if(n=='R')
        {
          myOLED.setFont(SmallFontRus);
          myOLED.print(dat,OLED_R,7);
          }
if(n=='0')
        {
          myOLED.setFont(SmallFontRus);
                 // myOLED.clrScr(); 
          myOLED.print("     ",OLED_R,0);
          myOLED.print("     ",OLED_C,1);
          myOLED.print("        ",OLED_L,1);
          myOLED.print("Установл",OLED_L,1);
          myOLED.print("        ",OLED_R,1);
          myOLED.print("Текущее",OLED_R,1);
          myOLED.print("     ",OLED_L,6);
          myOLED.print("Rуст",OLED_L,6);
          myOLED.print("     ",OLED_C,6);
          myOLED.print("Tтп",OLED_C,6);
          myOLED.print("     ",OLED_R,6);
          myOLED.print("Rтек",OLED_R,6);
          int i;
          for(i=0;i<boxnum;i++) 
          myOLED.print( "|" , i*wid, 0);
          }
if(n=='1')
          {
          myOLED.setFont(SmallFontRus);
          myOLED.print("  ",((wid+1)*TS_box+3),0);
          if(dat==1)
          myOLED.print("*",((wid+1)*TS_box+5),0);
          if(dat==2)
          myOLED.print("+",((wid+1)*TS_box+5),0);
            }
if(n=='U')
      {
          myOLED.setFont(SmallFontRus);
          if(dat==1)
          myOLED.print("+",((wid+1)*U_box+5),0);
          if(dat==0)
          myOLED.print("-",((wid+1)*U_box+5),0);
}
if(n=='G')
      {
          myOLED.setFont(SmallFontRus);
          if(dat==1)
          myOLED.print("OK",((wid+1)*TG_box+1),0);
          if(dat==0)
          myOLED.print("!!",((wid+1)*TG_box+1),0);
}

if(n=='A')
      {
          myOLED.setFont(SmallFontRus);
          if(dat==1)
          myOLED.print("SN",(wid+1)*SHIFT_box+3,0);
          if(dat==2)
          myOLED.print("SE",(wid+1)*SHIFT_box+3,0);
          if(dat==0)
          myOLED.print("S0",(wid+1)*SHIFT_box+3,0);
      }
if(n=='B')
      { myOLED.print( "КТ" , (wid+1)*KT_box+5, 0);}
if(n=='E')
      {
        if(dat==1)
            {
        myOLED.setFont(MediumFontRus);
        myOLED.clrScr(); 
        myOLED.print( "Ошибка чтения"     , OLED_C, 2);
        myOLED.print( "калибровки"     , OLED_C, 3);
        }
         if(dat==2)
        {
           myOLED.setFont(SmallFontRus);
            myOLED.print( "     " , OLED_R, 1); 
            myOLED.print( "     " , OLED_L, 1); 
           myOLED.print( "!ЗА ТАБЛИЦЕЙ!", OLED_C, 1); }
      
        if(dat==3)
        {
           myOLED.setFont(SmallFontRus);
            myOLED.print( "    " , OLED_R, 1); 
            myOLED.print( "    " , OLED_L, 1); 
           myOLED.print( "!R ЗА ТАБЛИЦЕЙ!", OLED_C, 1); 
           
           }
      }
if(n=='K')
      {
        if(dat==0)
          {  myOLED.clrScr(); 
          // myOLED.setFont(SmallFontRus);
           myOLED.print( "КАЛИБРОВКА" , OLED_C, 1); 
           myOLED.print( "ЗАКОНЧЕНА" , OLED_C, 2); 
           myOLED.print("i",OLED_L,3); 
           myOLED.print("tim",OLED_C,3);
           myOLED.print("t1",OLED_R,3);
           }
            
            if(dat==1)
            {
              myOLED.setFont(SmallFontRus);
            myOLED.clrScr();                                   
            myOLED.print( "КАЛИБРОВКА"    ,   OLED_C, 0);   
            myOLED.print( "ТЕМПЕРАТУРА"     , OLED_C, 1);    
            myOLED.print( "ВРЕМЯ" , OLED_L, 3); 
            myOLED.print( "НОМЕР" , OLED_R, 3);   
            myOLED.print( "СОПРОТИВЛЕНИЕ(Ом/уе)"     , OLED_C, 5);    
            }
            if(dat==3)
          {            
             myOLED.setFont(SmallFontRus);
          myOLED.clrScr(); 
        myOLED.print( "КАЛИБРОВОЧНАЯ" , OLED_C, 3); 
        myOLED.print( "ТАБЛИЦА" , OLED_C, 4); 
        myOLED.print( "НЕДОСТУПНА" , OLED_C, 5); 
        delay(2000);
        myOLED.clrScr(); 
        myOLED.print( "УДЕРЖИВАЙТЕ КБ" , OLED_C, 3); 
        myOLED.print( "ДЛЯ КАЛИБРОВКИ" , OLED_C, 5); 
        delay(2000);
        myOLED.clrScr(); 
        myOLED.print( "УДЕРЖИВАЙТЕ" , OLED_C, 3); 
        myOLED.print( "ЦЕНТР ВАЛЬКОДЕРА" , OLED_C, 4); 
        myOLED.print( "ДЛЯ РАБОТЫ В АЕ" , OLED_C, 5); 
        delay(2000);
          }
                }
if(n=='i')
              {
            // myOLED.setFont(SmallFontRus);
            myOLED.print( "       " , OLED_R, 4); 
            myOLED.print( dat-1 , OLED_R, 4);   
            myOLED.print( "       " , OLED_L, 4); 
          }
if(n=='q')
        {
           myOLED.print( dat/1000 , OLED_L, 4); }
if(n=='c')
           {   myOLED.print( "       "     , OLED_C, 2);  
               myOLED.print( dat     , OLED_C, 2);  }
if(n=='O')
               {  
             // myOLED.print( ROmh/(1024/termtmp-1)     , OLED_C, 6);    
              myOLED.print( Rft/(1024/dat-1)      , OLED_L, 6); 
               myOLED.print( dat      , OLED_R, 6);
      }
if(n=='k')
      {     myOLED.setFont(SmallFontRus);
          //  myOLED.clrScr(); 
            myOLED.print( (kb_delay-dat)/kb_delay*10     , OLED_R, 0);
        }
if(n=='b')
        {
          myOLED.setFont(MediumFontRus);
          myOLED.print("     ",OLED_L,4);
          myOLED.print("     ",OLED_R,4);
          myOLED.print(dat,OLED_L,4,0);
          }
      }
       }
      
