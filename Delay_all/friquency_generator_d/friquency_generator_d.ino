#include <Arduino.h>
#include <TM1637Display.h>
#include <EEPROM.h>

byte out=2;
byte CLK=3;//valcoder
byte DT=4;
byte SW=5;

unsigned char encoder_A, encoder_B, encoder_A_prev; // valcoder
unsigned char SW_State,SW_State_prev;


int sw_counter;
int sw_delay=5000;
byte adr=3;
byte adr_d=7;
float freq=10;
float mult=1;
int d=2;
unsigned long t1,t2,dt,tee1,tee2,dtee;
unsigned long td1,td2,dtd;
float T,dT;
int dTee=10000; //delay before puting to eepromt
long dTd=10000;//delay for d set
//int d[4];

byte u_flag=0;
byte eet_flag=1;
byte d_flag=0;

TM1637Display display(7, 6);

void setup() {
//Serial.begin(9600);

    display.setBrightness(0x0f);
EEPROM.get(adr,freq);
EEPROM.get(adr_d,d);
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
{
  t2=micros();
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
      if(d_flag==1)
    {td2=millis();
    if(td2-td1>1000)
    if(td2-td1>dTd||!SW_State)
    {d_flag=0;
    display.clear(); 
    display.showNumberDec(freq*10); 
    EEPROM.put(adr_d,d);}}
    
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
      if(d_flag==0)
      {
      if(mult>freq&&mult!=0.1)
      mult=mult/10;  
      freq=freq-mult;
      if(freq<0.1)
      freq=0.1;
      }
      if(d_flag==1)
      {d=d-1;
      if(d<2)
      d=2;}
    }
       else
    {
      if(d_flag==0)
      {
      freq=freq+mult;
      if(freq>999.9)
      freq=999.9;
      }
      if(d_flag==1)
      {d=d+1;
      if(d>20)
      d=20;}
      }
      if(d_flag==0)
      {
      T=1000000/freq;
      display.showNumberDec(freq*10); 
      }
      if(d_flag==1)
      {    display.clear();  
            char data1[4];
data1[1]=display.encodeDigit(1)| 0b10000000; // для получения символа с двоеточием
if(d<10)
{data1[2]=display.encodeDigit(d);
data1[3]=0;}
if(d>=10)
{data1[2]=display.encodeDigit(floor(d/10));
data1[3]=display.encodeDigit(d-floor(d/10)*10);}
display.setSegments(data1);
td1=millis();
        }
  }
    encoder_A_prev = encoder_A;

 if(!SW_State)
 {
 if(SW_State_prev)
  {
  sw_counter=0;
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
  if(!SW_State_prev)
  sw_counter++;}
  if(sw_counter==sw_delay)
  if(d_flag==0)
  {
    sw_counter=0;
    display.clear();  
    char data1[4];
data1[0]=0;    
data1[1]=display.encodeDigit(1)| 0b10000000; // для получения символа с двоеточием
if(d<10)
{data1[2]=display.encodeDigit(d);
data1[3]=0;}
if(d>=10)
{data1[2]=display.encodeDigit(floor(d/10));
data1[3]=display.encodeDigit(d-floor(d/10)*10);}
display.setSegments(data1);
td1=millis();
td2=td1;
d_flag=1;

 }
  SW_State_prev=SW_State;
}              
