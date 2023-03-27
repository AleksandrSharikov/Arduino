#include <EEPROM.h>
//#include <LiquidCrystal.h>
#include <max6675.h>
//#include "max6675.h"
#include <iarduino_OLED_txt.h>                         // Подключаем библиотеку iarduino_OLED_txt.
iarduino_OLED_txt myOLED(0x78);
extern uint8_t SmallFontRus[];   // Подключаем шрифт SmallFontRus.


int Ru; //set mesure
int Rt; //now mesure
float f1024=1024;
int Rfu=20;
int Rft=24;//resistor of devider
float ROmh; // real resistance
int dR;
int DR=3; //gate
byte kb;   //нажатие калибровки
byte fl;    //diod flag
//int k[2][80]; // kalibr table 0-t;1-r
struct call{byte t; int r;};    //srtucture calibr
call k[100];
byte kflag=0; // flag kalibrkvki
int temp; // aproximately temp
int tempset;
byte eeflag=0; //EEPromt calib flag
byte temset=5;// place in eeprom where is seted temp
float TS=60; //seted temp
byte ts1;  // TS=ts1+ts2/10
byte ts2;
byte ts1p;   // previous ts1
unsigned char encoder_A, encoder_B, encoder_A_prev; // valcoder
unsigned char SW_State;
byte num; //number where TS is
float Ttemp;
int Tmax=101; // termination of colibration
long unsigned toe=10000; //timout encoder
int beg=10; //table start
byte wid=14; //box's wide 
byte boxnum=4; //number of boxes
int test;// test ratio
int test2;


//pins
byte TG=2;// gotov
byte TRPin=17;// termoresistor analog
//byte RPin=17; // reostat       anflog
byte UPin=5; // pit
byte KB=6; // kalibrovka
byte KL=7; // Light kalibr
byte thermoDO = 12; //
byte thermoCS = 11;  //  termocouple pins
byte thermoCLK = 10;  //
byte vccPin = 9; // termocouple power
byte CLK=3;//valcoder
byte DT=4;
byte SW=2;

//int gndPin = 8;
/*byte RS=14;     // LCD pins
byte E=15;
byte D4=16;
byte D5=17;
byte D6=18;
byte D7=19;

LiquidCrystal lcd(RS,E,D4,D5,D6,D7);*/


void setup() {
  // put your setup code here, to run once:
//lcd.begin(16,2);      //lcd start
  
//Serial.begin(9600);

    myOLED.begin();                                    // Инициируем работу с дисплеем.
    myOLED.setFont(SmallFontRus);                      // Указываем шрифт который требуется использовать для вывода цифр и текста.
    myOLED.clrScr(); 
     myOLED.print( "void setup()"    ,   OLED_C, 0);   
  pinMode(TG,OUTPUT);  //Gotov
  digitalWrite(TG,LOW);
  pinMode(UPin,OUTPUT); //Pitanie
  digitalWrite(UPin,LOW);
  pinMode(KB,INPUT);  //Calibrovka
  digitalWrite(KB,LOW);
  pinMode(KL,OUTPUT); //Lampa
  digitalWrite(KL,LOW);
  fl=0;
  
  // Check if there is a calibration table
 checkcall();
 

}






void loop() 
{
//  myOLED.print( "              "    ,   OLED_C, 0); 
//  myOLED.print( "void loop()"    ,   OLED_C, 0);   
encoder();
mainloop();


kb=digitalRead(KB);
if(kb==HIGH)              //if kb pressed
{
  delay(500);
  if(kb==HIGH)
  {digitalWrite(UPin,LOW);
  digitalWrite(TG,LOW);
 // Kalibrovka(); 
  checkcall();}  
  }
}



        //---------------------------------------------------------FUNCTIONS-------------------------------------------------------------

void mainloop()                                                               //main loop function
{
 // myOLED.print( "              "    ,   OLED_C, 0); 
 //    myOLED.print( "void mainloop()"    ,   OLED_C, 0);   
  
Rt=analogRead(TRPin);
Ttemp=getTR(Rt);
myOLED.print( Rt , OLED_R, 4); 
myOLED.print(Ttemp, OLED_L, 4);
dR=abs(Ru-Rt);

if(Rt>Ru)                 //pitanie
{digitalWrite(UPin,HIGH);
myOLED.print("+",((wid+1)*2+5),0);}
else
{digitalWrite(UPin,LOW);
myOLED.print("-",((wid+1)*2+5),0);}

                            //gotov
if(dR<DR)
{digitalWrite(TG,HIGH);
myOLED.print("OK",((wid+1)*3+1),0);}
else
{digitalWrite(TG,LOW);
myOLED.print("!!",((wid+1)*3+1),0);}

if(fl==1)
digitalWrite(KL,HIGH);
else
digitalWrite(KL,LOW);

testTP();


  
  }




void Kalibrovka()                                                                     //kalibrovka
{
  myOLED.print( "              "    ,   OLED_C, 0); 
   myOLED.print( "void Kalibrovka()"    ,   OLED_C, 0);   

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

float t1=0;
byte t2=0;
byte tm=0;
long unsigned tim;
long unsigned tim1;
byte i=0;
long unsigned dtim=180000;      // не повысилось на градус за 3 минуты

myOLED.clrScr();                                   // Чистим экран.
    myOLED.print( "КАЛИБРОВКА"    ,   OLED_C, 0);   
    myOLED.print( "ТЕМПЕРАТУРА"     , OLED_C, 1);    
    myOLED.print( "ВРЕМЯ" , 0, 3); 
    myOLED.print( "НОМЕР" , OLED_R, 3);   
    myOLED.print( "СОПРОТИВЛЕНИЕ"     , OLED_C, 5);    
     
    delay(3000);     

EEPROM.write(0,0);
for(i=0;i<80;i++)   //обнуление массива
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
  int termtmp=analogRead(TRPin);
  myOLED.print( "       "     , OLED_C, 2);  
   myOLED.print( t1     , OLED_C, 2, 3);  
    myOLED.print( ROmh/(1024/termtmp-1)     , OLED_C, 6);    
  if(t2>tm)
  {
k[i].t=t2;
k[i].r=analogRead(TRPin);
tim1=millis();  
i++;
tm=t2;


/*Ru=analogRead(TRPin);
ROmh=Rfu*(f1024/Ru-1);*/
     
    myOLED.print( "        " , OLED_R, 4); 
    myOLED.print( i-1 , OLED_R, 4);   
    myOLED.print( "        " , OLED_L, 4); 
   
  }
  
   myOLED.print( tim/1000 , OLED_L, 4); 

delay(1500);
//blinc();
  }
  myOLED.clrScr(); 
   myOLED.print( "КАЛИБРОВКА" , OLED_C, 1); 
   myOLED.print( "ЗАКОНЧЕНА" , OLED_C, 2); 
   myOLED.print("i",OLED_L,3); 
   myOLED.print("tim",OLED_C,3);
   myOLED.print("t1",OLED_R,3);
   myOLED.print(i,OLED_L,4); 
   myOLED.print(tim,OLED_C,4);
   myOLED.print(t1,OLED_R,4);
   if (i>99)
   myOLED.print("^^^^^",OLED_L,5);
   if (tim>=dtim)
   myOLED.print("^^^^^",OLED_C,5);
   if (t1>=Tmax)
   myOLED.print("^^^^^",OLED_R,5);
   
   
//for(i=0;30;i++)
//blinc();
digitalWrite(KL,HIGH);
fl=1;
kflag=1;    
eewrite();
while(!digitalRead(KB))
{}
  }

  

 
  byte podbor(float TEM)                                                               //serch
  {            
  //  myOLED.print( "              "    ,   OLED_C, 0); 
  //    myOLED.print( "byte podbor(float TEM)"    ,   OLED_C, 0);                                        
  int i=0;
  byte t;
  t=floor(TEM);
  if (t<k[0].t)
    myOLED.print( "!ЗА ТАБЛИЦЕЙ!", OLED_C, 6); 
    else
    while(t>k[i].t)
    i++;     
  //  myOLED.print( "+"    ,   OLED_L, 6); 
    return i;
  }
  
byte rpodbor(int r)                                                                //revers serch
  {          
    //   myOLED.print( "              "    ,   OLED_C, 0); 
     //     myOLED.print( "byte rpodbor(int r)"    ,   OLED_C, 0);                                            
  int i=0;
  if (r>k[0].r)
    myOLED.print( "!R ЗА ТАБЛИЦЕЙ!", OLED_C, 6); 
    else
    while(r<k[i].r)
    i++;   
    return i;
  }



void eeread()                                                                  //чтение из ЕЕ
{
          
      // myOLED.print( "              "    ,   OLED_C, 0); 
      //    myOLED.print( "void eeread()"    ,   OLED_C, 0);
//int j=1;
//byte rb1;
//byte rb2;
//byte flt;   //serching for zerow t
int i=0;
eeflag=EEPROM.read(0);
if(eeflag==1)
{
for(i=0;i<80;i++)
{k[i].t=0;
k[i].r=0;}
i=0;
 EEPROM.get(beg,k);
 
  kflag=1;
}
else
{
  myOLED.clrScr(); 
  myOLED.print( "Ошибка чтения калибровки"     , OLED_C, 3);
delay(10000);}
  }





void eewrite()                                                                      //Запись в ЕЕ
{
          
 //      myOLED.print( "              "    ,   OLED_C, 0); 
 //         myOLED.print( "void eewrite()"    ,   OLED_C, 0);
 // int i=0;
//int j=0;
//byte rb1;
//byte rb2;

 /* do{
  rb1=*((byte*)(&k[j].r));
  rb2=*((byte*)(&k[j].r)+1);
 EEPROM.write(i+1,k[j].t);
 EEPROM.write(i+2,rb1);
 EEPROM.write(i+3,rb2); 
 i=i+3;
 j++;
  }
  
  while(k[i].t!=0);*/
  EEPROM.put(beg,k);
  EEPROM.write(0,1);
  }
  


  

 /* void blinc()                                                                         //blinc
  {
            
       myOLED.print( "              "    ,   OLED_C, 0); 
          myOLED.print( "void blinc"    ,   OLED_C, 0);
    byte fl1;
  
    if(fl==0)
    {digitalWrite(KL,HIGH);
    fl1=1;}
    if(fl==1)
    {digitalWrite(KL,LOW);
    fl1=0;}
    fl=fl1;
    
    }
*/



                                                                                        // encoder
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

 if(!encoder_A)
   {
    // myOLED.clrScr(); 
   // myOLED.print( "JN"    ,   OLED_C, 3);
   // delay(1000);
 te0=millis();
 te1=te0;
  encoder_A_prev = HIGH;
  myOLED.print("VK",(wid+4),0);

 while(ted<toe)
 {
 
  /* myOLED.print( ted/100    ,   OLED_C, 6);
   myOLED.print( te0/100    ,   OLED_C, 5);
   myOLED.print( te1/100    ,   OLED_C, 4);*/


  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
/*  SW_State=digitalRead(2);
 myOLED.print(SW_State,OLED_L,6);
  //if(!SW_State) myOLED.print("SW=-",OLED_L,6);
   if(mfl==0) myOLED.print("mfl=0",OLED_C,6); 
   if(mfl!=0) myOLED.print("mfl!=0",OLED_C,6);*/
  te1=millis();
  ted=te1-te0;
  if(!encoder_A && encoder_A_prev)
  {
   
    if(encoder_B)
    {
       ts1p=floor(TS);
      TS=TS-0.2;
      myOLED.print( "      "    ,   OLED_L, 2); 
      myOLED.print( TS    ,   OLED_L, 2); 
      te0=millis();
    }
       else
    {
      ts1p=floor(TS);
      TS=TS+0.2;
      myOLED.print( "     "    ,   OLED_L, 2); 
       myOLED.print( TS    ,   OLED_L, 2); 
       te0=millis();
      }
  }
    encoder_A_prev = encoder_A;

  //Работаем с центральной осью энкодера - кнопкой
  //Этот кусок кода образует собой как бы перекидной триггер
  //Считываем значение пина 2 на плате Arduino
  //к которому подключен контакт SW энкодера
  //Если центральная ось нажата - то сигнал SW будет иметь низкий уровень
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
       ts1=floor(TS);
       ts2=TS-ts1;
      // if(ts1p!=ts1)  
     // myOLED.print( "      "    ,   OLED_L, 6); 
       num=podbor(TS);
       Ru=k[num].r-(k[num].r-k[num+1].r)*ts2;
        myOLED.print( "      "    ,   OLED_R, 2); 
      myOLED.print( Ru    ,   OLED_R, 2);  
      myOLED.print( "      "    ,   OLED_C, 2); 
      myOLED.print( num    ,   OLED_C, 2); 
       EEPROM.put(temset,TS);
       myOLED.print("  ",((wid+1)+3),0);
       myOLED.print("*",((wid+1)+5),0);
     
  }
 // encoder_A_prev = encoder_A;
}              


 void checkcall()                                                                           //check ct
 { 
          
   //    myOLED.print( "              "    ,   OLED_C, 0); 
     //     myOLED.print( "void checkcall"    ,   OLED_C, 0);

byte i;
     
   eeflag=EEPROM.read(0);
   if(eeflag!=1)
       {
        kb=digitalRead(KB);
        myOLED.print( "  " , 0, 0);
        
        while(kb==LOW)  
        {   
          myOLED.clrScr(); 
        myOLED.print( "КАЛИБРОВОЧНАЯ" , OLED_C, 3); 
        myOLED.print( "ТАБЛИЦА" , OLED_C, 4); 
        myOLED.print( "НЕДОСТУПНА" , OLED_C, 5); 
        delay(1000);
        myOLED.clrScr(); 
        myOLED.print( "ПОДГОТОВЬТЕСЬ" , OLED_C, 3); 
        myOLED.print( "И НАЖМИТЕ KB" , OLED_C, 5); 
        delay(1000);
        kb=digitalRead(KB);        
        }
        Kalibrovka();
}


    eeread();
    /*ts1=EEPROM.read(temset);
    ts2=EEPROM.read(temset+1);
    TS=ts1+ts2/10;
    num=podbor(TS);
       ts1=floor(TS);
       ts2=(TS-ts1)*10;
       Ru=k[num].r+(k[num].r-k[num+1].r)*ts2/10;*/
       
       EEPROM.get(temset,TS);
       if(TS<20||TS>120)
       TS=85;

       ts1=floor(TS);
       ts2=TS-ts1;
      // if(ts1p!=ts1)  
    //  myOLED.print( "      "    ,   OLED_L, 6); 
       num=podbor(TS);
       Ru=k[num].r-(k[num].r-k[num+1].r)*ts2;
       
       
       myOLED.clrScr(); 
       myOLED.print( "      "    ,   OLED_R, 2); 
      myOLED.print( Ru    ,   OLED_R, 2);  
      myOLED.print( "      "    ,   OLED_C, 2); 
      myOLED.print( num    ,   OLED_C, 2); 
       myOLED.print( "УСТАНОВЛЕНО"    , OLED_C, 1);    
       myOLED.print( TS    , OLED_L, 2);    
       myOLED.print( "ТЕКУЩАЯ" , OLED_C, 3);
       for(i=0;i<boxnum;i++) 
       myOLED.print( "|" , i*wid, 0);
       myOLED.print( "КТ" , 0, 0);
       
       
       
  }                        


  float getTR(int R)                                                      // ger T from R 
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

void testTP()                                                                //termocouple test
{
if(test==1000)
{float testtemp;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
delay(2000);
testtemp=thermocouple.readCelsius();
  myOLED.print("           ",OLED_R,5);
    myOLED.print("    ",OLED_L,5);
myOLED.print(testtemp,OLED_R,5,4);
myOLED.print("testtemp",OLED_C,5);
myOLED.print(test2,OLED_L,5);
test=0;
test2++;
  }
  test++;
  
}
