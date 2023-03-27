#include <EEPROM.h>

#include <max6675.h>
//#include "max6675.h"


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

//pins
byte TG=2;// gotov
byte TRPin=7;// termoresistor analog
byte RPin=6; // reostat       anflog
byte UPin=4; // pit
byte KB=6; // kalibrovka
byte KL=7; // Light kalibr
byte thermoDO = 12; //
byte thermoCS = 11;  //  termocouple pins
byte thermoCLK = 10;  //
int vccPin = 9; // termocouple power
//int gndPin = 8;



void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  
  pinMode(TG,OUTPUT);  //Gotov
  digitalWrite(TG,LOW);
  pinMode(UPin,OUTPUT); //Pitanie
  digitalWrite(UPin,LOW);
  pinMode(KB,INPUT);  //Calibrovka
  digitalWrite(KB,LOW);
  pinMode(KL,OUTPUT); //Lampa
  digitalWrite(KL,LOW);
  fl=0;
  
  

}

void loop() {
   // put your main code here, to run repeatedly:
Ru=analogRead(RPin);
Rt=analogRead(TRPin);
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

kb=digitalRead(KB);
if(kb==HIGH)              //if kb pressed
{
  delay(500);
  if(kb==HIGH)
  {digitalWrite(UPin,LOW);
  digitalWrite(TG,LOW);
  Kalibrovka();  }  
  }

  if(kflag==0)      // read from ee
  {
    eeflag=EEPROM.read(0);
    if(eeflag==1)
    eeread();
    }

if(kflag==1)              //serch in calibr table
  {temp=podbor(Rt);
  tempset=podbor(Ru);
  }
  Serial.print("R set = ");
  Serial.print('\t');
  ROmh=Rfu*(f1024/Ru-1);
  Serial.print(ROmh);
  Serial.print('\t');
  if(kflag==1)
  {Serial.print("apr");
  Serial.print('\t');
  Serial.print(tempset);
  Serial.print("/");
  Serial.print(tempset+1);
  Serial.print("C");
  Serial.print('\t'); 
  }
   Serial.print("R = ");
  Serial.print("\t");
    ROmh=Rfu*(f1024/Rt-1);
  Serial.print(ROmh);
  Serial.print("\t");
  if(kflag==1)
  {Serial.print("apr");
  Serial.print("\t");
  Serial.print(temp);
  Serial.print("/");
  Serial.print(temp+1);
  Serial.print("C");
  Serial.print("\t"); }
  Serial.print("\n");

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
  float t1;
pinMode(vccPin,OUTPUT); //termocouple on
digitalWrite(vccPin,HIGH);
delay(1000);
t1=thermocouple.readCelsius();
Serial.print(t1);
//Serial.print(dR);
Serial.print('\n');


delay(1000);
}



//FUNCTIONS

void Kalibrovka()                                                 //kalibrovka
{


MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

float t1;
int t2;
long unsigned tim;
long unsigned tim1;
byte i=0;
long unsigned dtim=180000;      // не повысилось на градус за 3 минуты

EEPROM.write(0,0);
for(i=0;i<80;i++)   //обнуление массива
{k[i].t=0;
k[i].r=0;}
digitalWrite(KL,LOW);
fl=0;
pinMode(vccPin,OUTPUT); //termocouple on
digitalWrite(vccPin,HIGH);
delay(1000);
t1=thermocouple.readCelsius();
t2=round(t1);
digitalWrite(UPin,HIGH);
tim=millis();
tim1=tim;

while(t1<101&&tim<dtim)         //degre complate
{
  tim=abs(millis()-tim1);
  t2=round(t1);
k[i].t=t2;
k[i].r=analogRead(TRPin);
tim1=millis();  
i++;
  

{Serial.print(i);
  Serial.print('\t');
  Serial.print(k[i].t);
  Serial.print("/");
  Serial.print(k[i].r);
  Serial.print("C");
  Serial.print('\n'); 
  }

delay(1500);
blinc();
  }
for(i=0;30;i++)
blinc();
digitalWrite(KL,HIGH);
fl=1;
kflag=1;    
eewrite();
  }


  int podbor(int R)                                  //serch
  {                                                 
  int i=0;
  int R1=k[0].r;
  if(R>R1);
  Serial.print("OWERLIMIT");
    while(R>R1&&R1>0&&i<80)
    {i++;
    R1=k[i].r; }
    if(R1==0||i==79)
     Serial.print("OWERLIMIT");
     return(R1);
  }



void eeread()         //чтение из ЕЕ
{int i=0;
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
do
{
  k[i].t=EEPROM.read(j);
  *((byte*)(&k[i].r))=EEPROM.read(j+1);
  *((byte*)(&k[i].r)+1)=EEPROM.read(j+2);
  i++;
  j=j+3;
  }
  while(k[i-1].t!=0);
  kflag=1;
}
else
{Serial.print("eeflag error");
delay(10000);}
  }

void eewrite()      //Запись в ЕЕ
{int i=0;
int j=0;
byte rb1;
byte rb2;

  do{
  rb1=*((byte*)(&k[j].r));
  rb2=*((byte*)(&k[j].r)+1);
 EEPROM.write(i+1,k[j].t);
 EEPROM.write(i+2,rb1);
 EEPROM.write(i+3,rb2); 
 i=i+3;
 j++;
  }
  while(k[i].t!=0);
  EEPROM.write(0,1);
  }
  


  

  void blinc()                          //blinc
  {
    byte fl1;
  
    if(fl==0)
    {digitalWrite(KL,HIGH);
    fl1=1;}
    if(fl==1)
    {digitalWrite(KL,LOW);
    fl1=0;}
    fl=fl1;
    
    }
