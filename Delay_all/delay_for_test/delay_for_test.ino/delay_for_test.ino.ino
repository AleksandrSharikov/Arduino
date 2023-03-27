#include <Arduino.h>
#include <TM1637Display.h>
#include <EEPROM.h>

//PINS
byte out1=2;
byte out2=11;
byte CLK1=3;//valcoder1
byte DT1=4;
byte SW1=5;
byte CLK2=10;//valcoder2
byte DT2=9;
byte SW2=8;

unsigned char encoder_A1, encoder_B1, encoder_A_prev1; // valcoder1
unsigned char SW_State1,SW_State_prev1;
unsigned char encoder_A2, encoder_B2, encoder_A_prev2; // valcoder2
unsigned char SW_State2,SW_State_prev2;


int sw_counter;
int sw1_counter;
int sw_delay=5000;
byte adr_freq=3;
byte adr_del=7;
byte freq_mass[3]={10,25,50};  //frequences
byte freq;
byte freq_i; //frequency nomber
int T_cor=70;
//float mult=1;
//int d=2;
int del;//delay
int del_mult=10;
byte pulse_wide=25;
unsigned long t1,t2,dt;//,tee1,tee2,dtee;
unsigned long i,a,b;
float T,dT;
float k=1.331;// recount mks to nop
int d=4;
//int dTee=10000; //delay before puting to eepromt
//long dTd=10000;//delay for d set
//int d[4];

byte u1_flag=0;
byte u2_flag=0;
byte eet_flag=1;
byte d_flag=0;
byte beg_flag=1;
byte work_flag=0;

TM1637Display display(7, 6);

void setup() {
//Serial.begin(9600);

    display.setBrightness(0x0f);
EEPROM.get(adr_freq,freq_i);
EEPROM.get(adr_del,del);
DDRD = B00000100;
DDRB = B00001000;
//pinMode(out1,OUTPUT);
//pinMode(out2,OUTPUT);
freq=freq_mass[freq_i];
T=1000000/freq;
a=k*pulse_wide-d;
b=del*k-d;
//t1=micros();
display.showNumberDec(freq);
delay(1500);
display.showNumberDec(del);
delay(1500);
display.setBrightness(0);
display.showNumberDec(work_flag);
delay(1500);
display.showNumberDec(freq);

}

void loop() {
  while(1)
 {if(work_flag==1)
  cycle();
 encoder1();
 encoder2();
 }
}


void cycle()
{

  
  if(beg_flag==1)
  {
    t1=micros();
  u1_flag=1;
        noInterrupts();
      PORTD |=1<<2;
      for(i=0;i<a;i++)
      asm("nop");
      //PORTD &=~(1<<2);
      for(i=0;i<abs(b-a);i++)
      asm("nop");
      //PORTD |=1<<2;
      PORTB |=1<<3;
      for(i=0;i<a;i++)
      asm("nop");
      PORTB &=~(1<<3);
      interrupts();
  u2_flag=1;
  beg_flag=0;
  }
  t2=micros();
  dt=t2-t1;

if(dt>T/2)
PORTD &=~(1<<2);
  if(dt>=T-T_cor)
  beg_flag=1;
}
  

  void encoder1()
 {              
 
  encoder_A1 = digitalRead(CLK1);
  encoder_B1 = digitalRead(DT1);
  SW_State1 = digitalRead(SW1);

   if(!encoder_A1 && encoder_A_prev1)
  {
     if(encoder_B1)
    {
freq_i=freq_i-1;

    }
       else
    {
freq_i++;

      }
if(freq_i<0||freq_i>250)
freq_i=0;
if(freq_i>1)
freq_i=2;

      freq=freq_mass[freq_i];
      T=1000000/freq;
      display.showNumberDec(freq);       
    }
    encoder_A_prev1 = encoder_A1;

 if(!SW_State1)
 {
 if(SW_State_prev1)
  {
work_flag=0;
EEPROM.put(adr_freq,freq_i);
EEPROM.put(adr_del,del);
display.setBrightness(0);
display.showNumberDec(work_flag); 
sw1_counter=0;
  }
  if(!SW_State_prev1)
  sw1_counter++;
  if(sw1_counter==sw_delay*10)
  {
    sw1_counter=0;
    
    display.setBrightness(7);
    work_flag=1;
    display.showNumberDec(freq); 
    delay(500);
EEPROM.put(adr_freq,freq_i);
EEPROM.put(adr_del,del);
 }

}  
 if(sw1_counter!=0)
 if(SW_State1)
 sw1_counter=0;
  SW_State_prev1=SW_State1;            
 }
  void encoder2()
 {              
 
  encoder_A2 = digitalRead(CLK2);
  encoder_B2 = digitalRead(DT2);
  SW_State2 = digitalRead(SW2);

   if(!encoder_A2 && encoder_A_prev2)
  {
     if(encoder_B2)
    {
      while(del_mult>del)
      del_mult=del_mult/10;
      if(del_mult<1)
      del_mult=1;
      del=del-del_mult;
      if(del<0)
      del=0;
    }
       else
    {
      del=del+del_mult;
      if(del>9999)
      del=9999;
      if(del<pulse_wide)
      del=pulse_wide;
      }
b=del*k-d;
      display.showNumberDec(del);       
    }
    encoder_A_prev2 = encoder_A2;

 if(!SW_State2)
 sw_counter++;
 {
 if(sw_counter==sw_delay)
  {
del_mult=del_mult*10;
if(del_mult>1000)
del_mult=1;
sw_counter=0;
display.showNumberDec(del_mult); 
  }  
 } 
 if(sw_counter!=0)
 if(SW_State2)
 sw_counter=0;
 //SW_State_prev2=SW_State2;
 }            
