#include <EEPROM.h>

#include <max6675.h>
//#include "max6675.h"


int Ru; //set mesure
int Rt; //now mesure
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
byte thermoDO = 10; //
byte thermoCS = 11;  //  termocouple pins
byte thermoCLK = 12;  //
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

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
  int t1;
  byte tb;
pinMode(vccPin,OUTPUT); //termocouple on
digitalWrite(vccPin,HIGH);
delay(1000);
t1=thermocouple.readCelsius();
tb=t1;
Serial.print(tb);
Serial.print('\n');

delay(1000);
}

