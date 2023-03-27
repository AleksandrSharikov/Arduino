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
int Rft=20;//resistor of devider
float ROmh; // real resistance
int dR;
int DR=30; //gate
byte kb;   //нажатие калибровки
byte fl;    //diod flag
//int k[2][80]; // kalibr table 0-t;1-r
struct call{byte t; int r;};    //srtucture calibr
call k[80];
byte kflag=0; // flag kalibrkvki
int temp; // aproximately temp
int tempset;
byte eeflag=0; //EEPromt calib flag
byte temset=300;// place in eeprom where is seted temp
float TS=60; //seted temp
byte ts1;  // TS=ts1+ts2/10
byte ts2;
byte ts1p;   // previous ts1
unsigned char encoder_A, encoder_B, encoder_A_prev; // valcoder
static bool SW_State = false;
byte num; //number where TS is
float Ttemp;
int Tmax=101; // termination of colibration
long unsigned toe=10000; //timout encoder


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

void mainloop()                              //main loop function
{
 // myOLED.print( "              "    ,   OLED_C, 0); 
 //    myOLED.print( "void mainloop()"    ,   OLED_C, 0);   
  
Rt=analogRead(TRPin);
Ttemp=getTR(Rt);
myOLED.print( Rt , OLED_R, 4); 
myOLED.print(Ttemp, OLED_L, 4);
dR=abs(Ru-Rt);

if(Rt<Ru)                 //pitanie
digitalWrite(UPin,HIGH);
else
digitalWrite(UPin,LOW);

                            //gotov
if(dR<DR)
digitalWrite(TG,HIGH);
else
digitalWrite(TG,LOW);

if(fl==1)
digitalWrite(KL,HIGH);
else
digitalWrite(KL,LOW);
  
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
    myOLED.print( "Т.Н. СОПРОТИВЛЕНИЕ"     , OLED_C, 5);    
     
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
i=1;

while(t1<Tmax&&tim<dtim&&i<80)         //degre complate
{
  tim=millis()-tim1;
  t1=thermocouple.readCelsius();
  t2=floor(t1);
  int termtmp=analogRead(TRPin);
  myOLED.print( "       "     , OLED_C, 2);  
   myOLED.print( t1     , OLED_C, 2, 3);  
    myOLED.print( termtmp     , OLED_C, 6);    
  if(t2>tm)
  {
k[i].t=t2;
k[i].r=analogRead(TRPin);
tim1=millis();  
i++;
tm=t2;


/*Ru=analogRead(TRPin);
ROmh=Rfu*(f1024/Ru-1);*/
     
    myOLED.print( "        " , OLED_C, 4); 
    myOLED.print( i-1 , OLED_R, 4);   
   
  }
   myOLED.print( tim/1000 , OLED_L, 4); 

delay(1500);
//blinc();
  }
  myOLED.clrScr(); 
   myOLED.print( "КАЛИБРОВКА" , OLED_C, 4); 
   myOLED.print( "ЗАКОНЧЕНА" , OLED_C, 5); 
   
//for(i=0;30;i++)
//blinc();
digitalWrite(KL,HIGH);
fl=1;
kflag=1;    
eewrite();
  }

  

 
  byte podbor(float TEM)                                                      //serch
  {            
    myOLED.print( "              "    ,   OLED_C, 0); 
       myOLED.print( "byte podbor(float TEM)"    ,   OLED_C, 0);                                        
  int i=0;
  byte t;
  t=floor(TEM);
  if (t<k[0].t)
    myOLED.print( "!ЗА ТАБЛИЦЕЙ!", OLED_C, 6); 
    else
    while(t>k[i].t)
    i++;    
    return i;
  }

byte rpodbor(int r)                                                    //revers serch
  {          
       myOLED.print( "              "    ,   OLED_C, 0); 
          myOLED.print( "byte rpodbor(int r)"    ,   OLED_C, 0);                                            
  int i=0;
  if (r>k[0].r)
    myOLED.print( "!R ЗА ТАБЛИЦЕЙ!", OLED_C, 6); 
    else
    while(r<k[i].r)
    i++;   
    return i;
  }



void eeread()                                                           //чтение из ЕЕ
{
          
       myOLED.print( "              "    ,   OLED_C, 0); 
          myOLED.print( "void eeread()"    ,   OLED_C, 0);int i=0;
int j=1;
byte rb1;
byte rb2;
//byte flt;   //serching for zerow t

eeflag=EEPROM.read(0);
if(eeflag==1)
{
for(i=0;i<80;i++)
{k[i].t=0;
k[i].r=0;}
i=0;
 while(k[i].t!=0)
{
  k[i].t=EEPROM.read(j);
  i++;
  *((byte*)(&k[i].r))=EEPROM.read(i);
  i++;
  *((byte*)(&k[i].r)+1)=EEPROM.read(i);
  i++;
 // j=j+3;
  }
 
  kflag=1;
}
else
{
  myOLED.clrScr(); 
  myOLED.print( "Ошибка чтения калибровки"     , OLED_C, 3);
delay(10000);}
  }





void eewrite()                                                                  //Запись в ЕЕ
{
          
       myOLED.print( "              "    ,   OLED_C, 0); 
          myOLED.print( "void eewrite()"    ,   OLED_C, 0);
  int i=0;
int j=0;
byte rb1;
byte rb2;
for(i=0;i<temset+2;i++)
EEPROM.write(i,0);
  do{
  rb1=*((byte*)(&k[j].r));
  rb2=*((byte*)(&k[j].r)+1);
 EEPROM.write(i+1,k[j].t);
 EEPROM.write(i+2,rb1);
 EEPROM.write(i+3,rb2); 
 i=i+3;
 j++;
  }
  while(k[j].t!=0);
  EEPROM.write(0,1);
  }
  


  

  void blinc()                                                                         //blinc
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




                                                                                  // encoder
 void encoder()
 {
          
       myOLED.print( "              "    ,   OLED_C, 0); 
          myOLED.print( "void encoder()"    ,   OLED_C, 0);
         long unsigned te0;
         long unsigned te1;
         long unsigned ted;
          
 
  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);

 if(!encoder_A /*&& encoder_A_prev*/)
   {
    // myOLED.clrScr(); 
   // myOLED.print( "JN"    ,   OLED_C, 3);
   // delay(1000);
 te0=millis();
 te1=te0;
  encoder_A_prev = HIGH;

 while(ted<toe)
 {
 
  /* myOLED.print( ted/100    ,   OLED_C, 6);
   myOLED.print( te0/100    ,   OLED_C, 5);
   myOLED.print( te1/100    ,   OLED_C, 4);*/


  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
  te1=millis();
  ted=te1-te0;
  if(!encoder_A && encoder_A_prev)
  {
    //Если уровень сигнала В высокий
    if(encoder_B)
    {
      //Значит вращение происходит по часовой стрелке
      //Здесь можно вставить операцию инкремента
       ts1p=floor(TS);
      TS=TS+0.1;
      myOLED.print( "      "    ,   OLED_L, 2); 
      myOLED.print( TS    ,   OLED_L, 2); 
      
      
  /*     ts1=floor(TS);
       ts2=(TS-ts1)*10;
       if(ts1p!=ts1)
      num=podbor(TS);
       Ru=k[num].r+(k[num].r-k[num+1].r)*ts2/10;*/
      
      te0=millis();
    }
    //Если уровень сигнала В низкий
    else
    {
      //Значит вращение происходит против часовой стрелки
      //Здесь можно вставить операцию декремента
     
      ts1p=floor(TS);
      TS=TS-0.1;
      myOLED.print( "     "    ,   OLED_L, 2); 
       myOLED.print( TS    ,   OLED_L, 2); 
      
       
       
  /*     ts1=floor(TS);
       ts2=(TS-ts1)*10;
       if(ts1p!=ts1)
       num=podbor(TS);
       Ru=k[num].r+(k[num].r-k[num+1].r)*ts2/10;*/
       te0=millis();
      //Здесь можно вставлять какие либо свои 
      //операции по обработке данных в нужном направлении
    }
  }
  //Обязательно нужно сохранить состояние текущего уровня сигнала А
  //для использования этого значения в следующем цикле сканирования программы
  encoder_A_prev = encoder_A;

  //Работаем с центральной осью энкодера - кнопкой
  //Этот кусок кода образует собой как бы перекидной триггер
  //Считываем значение пина 2 на плате Arduino
  //к которому подключен контакт SW энкодера
  //Если центральная ось нажата - то сигнал SW будет иметь низкий уровень
  if(digitalRead(SW))
  {
    //Если переменная SW_State установлена в false то установить её в true 
    if(SW_State)             //if valcoder is pressed write temp to eeprom
    {
      ts1=floor(TS);
      ts2=(TS-ts1)*10;
      EEPROM.write(temset,ts1);
      EEPROM.write(temset+1,ts2);
      te0=millis();
      //И запомнить состояние
     // SW_State = true;
    }
    //И наоборот - если переменная SW_State установлена в true, 
    //то сбросить её в false 
   /* else
    {
      //И запомнить состояние
      SW_State = false;
    }*/
  } 
 } //cicle ends
       ts1=floor(TS);
       ts2=(TS-ts1)*10;
       if(ts1p!=ts1)
       num=podbor(TS);
       Ru=k[num].r+(k[num].r-k[num+1].r)*ts2/10;
        myOLED.print( "      "    ,   OLED_R, 2); 
      myOLED.print( Ru    ,   OLED_R, 2); 
  }
 // encoder_A_prev = encoder_A;
}              


 void checkcall()                                                              //check ct
 {
          
       myOLED.print( "              "    ,   OLED_C, 0); 
          myOLED.print( "void checkcall"    ,   OLED_C, 0);
   eeflag=EEPROM.read(0);
  if(eeflag!=1)
       {
        kb=digitalRead(KB);
        
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
       TS=85;
       myOLED.clrScr(); 
       myOLED.print( "УСТАНОВЛЕНО"    , OLED_C, 1);    
       myOLED.print( TS    , OLED_L, 2);    
       myOLED.print( "ТЕКУЩАЯ" , OLED_C, 3); 
       
       
  }                        


  float getTR(int R)                                                      // ger T from R 
  {
            
       myOLED.print( "              "    ,   OLED_C, 0); 
          myOLED.print( "float getTR(int R)"    ,   OLED_C, 0);
    byte num;
    float T;
    float pose;
    num=rpodbor(R);
   pose=(k[num].r-R)/(k[num].r-k[num+1].r);
   T=k[num].t+(k[num+1].t-k[num].t)*pose;
   return T;
    }
