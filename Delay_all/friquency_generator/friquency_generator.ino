#include <Arduino.h>
#include <TM1637Display.h>
#include <EEPROM.h>

byte out=2;
byte CLK=3;//valcoder
byte DT=4;
byte SW=5;

unsigned char encoder_A, encoder_B, encoder_A_prev; // valcoder
unsigned char SW_State,SW_State_prev;

byte adr=3;
float freq=10;
float mult=1;
int d=2;
unsigned long t1,t2,dt,tee1,tee2,dtee;
float T,dT;
int dTee=10000; //delay before puting to eepromt
//int d[4];

byte u_flag=0;
byte eet_flag=1;

TM1637Display display(7, 6);

void setup() {
//Serial.begin(9600);

    display.setBrightness(0x0f);
EEPROM.get(adr,freq);
DDRD = B00000100;
T=1000000/freq;
t1=micros();
display.showNumberDec(freq*10);
}

void loop() {
  while(1)
 {cycle();
 encoder();}
}
void cycle()
{t2=micros();
dt=t2-t1;
if(dt-T/d<=0)
{if(u_flag==0)
{
  PORTD |=1<<2;
u_flag=1;}

}
else
if(u_flag==1)
{
  PORTD &=~(1<<2);  
u_flag=0;
{
  if(dT<dt-T/d)
  dT=dt-T/d;
}
}
if(dt-T>=0)
{t1=micros()-dt+T-47;
}
  }

  void encoder()
 {              
 
  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
  SW_State = digitalRead(SW);
 if(eet_flag==0)
 {
  tee2=millis();
  dtee=tee2-tee1;
  if(dtee>dTee)
  {EEPROM.put(adr,freq);
  eet_flag=1;}
  }
   if(!encoder_A && encoder_A_prev)
  {
    tee1=millis();
    tee2=tee1;
    eet_flag=0;
     if(encoder_B)
    {
      freq=freq-mult;
      if(freq<0.1)
      freq=0.1;
    }
       else
    {
      freq=freq+mult;
      if(freq>999.9)
      freq=999.9;
      }
      T=1000000/freq;
      display.showNumberDec(freq*10); 
  }
    encoder_A_prev = encoder_A;


 if(!SW_State&&SW_State_prev)
  {
  if(mult==100)
  mult=0.1;
  else
  if(mult==10)
  mult=100;
  else
  if(mult==1)
  mult=10;
  else
  if(mult==0.1)
  mult=1; 

  }
  SW_State_prev=SW_State;
}              
