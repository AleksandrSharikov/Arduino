#include <EEPROM.h>
#include <LiquidCrystal.h>

//pin
byte exp_but=5;
byte view_but=6;
byte enl=12;
byte lamp=11;
byte RS=19;//display pins
byte E=18;
byte DB4=17;
byte DB5=16;
byte DB6=15;
byte DB7=14;
byte CLK=7;   //encoder
byte DT=8;
byte SW=9;

//buttons
byte encoder_A;
byte encoder_B;
byte SW_State;
byte encoder_A_prev;
byte SW_State_prev;
int SW_counter;
int SW_delay=30000;
byte be;                //exposure and view buttons
byte bv;
byte be_counter;
float bv_counter;
byte be_delay=100;
float bv_delay=100000;
byte m_max=2; // quantity of encoder sets

byte EV_adr=3;//eeprom
byte bt_adr=7;
//flags
byte view_flag;

//constants
float bt=10000;//base time
int exp_del=1000;//exposure delay

//variables
byte m; //vhat change by encoder
byte bt_a; //bt aquracy
float EV=0;
float EXP; // exposure time
float T1,T2;
int T_last,T_now;

//chars

byte PM[8] = {//+-
  0b00000,
  0b00000,
  0b00100,
  0b01110,
  0b00100,
  0b00000,
  0b01110,
  0b00000
};
byte UV[8] = {//Under value
  0b01010,
  0b01010,
  0b01010,
  0b00000,
  0b00000,
  0b01010,
  0b01010,
  0b01010
};
byte UVC[8] = {
  0b01110,
  0b01110,
  0b01110,
  0b00100,
  0b00100,
  0b01110,
  0b01110,
  0b01110
};
byte UVL[8] = {
  0b11010,
  0b11010,
  0b11010,
  0b10000,
  0b10000,
  0b11010,
  0b11010,
  0b11010
};
byte UVR[8] = {
  0b01011,
  0b01011,
  0b01011,
  0b00001,
  0b00001,
  0b01011,
  0b01011,
  0b01011
};

byte L[8] = {
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001
};
byte C[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};
byte R[8] = {
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000
};

LiquidCrystal lcd(RS,E,DB4,DB5,DB6,DB7);

void setup() {
  // put your setup code here, to run once:
  pinMode(enl,OUTPUT);  
  digitalWrite(enl,LOW);
  pinMode(lamp,OUTPUT); 
  digitalWrite(lamp,HIGH);
  pinMode(exp_but,INPUT);  
  digitalWrite(exp_but,LOW);
  pinMode(exp_but,INPUT);  
  digitalWrite(exp_but,LOW);

lcd.begin(16,2);
lcd.createChar(1,PM);
lcd.createChar(2,UV);
lcd.createChar(3,UVC);
lcd.createChar(4,UVL);
lcd.createChar(5,UVR);
lcd.createChar(6,C);
lcd.createChar(7,L);
lcd.createChar(8,R);

EEPROM.get(bt_adr,bt);
EEPROM.get(EV_adr,EV);
  if(bt>1000)
  bt_a=1;
  EXP=bt*pow(2,EV);
  
  lcd.clear();
  lcd.print("Base time");
  lcd.setCursor(14,0);
  lcd.print("EV");
  lcd.setCursor(0,1);
  lcd.print(bt/1000,bt_a);
  lcd.setCursor(5,1);
  lcd.print("Sec");
  if(EV>=0)
  lcd.setCursor(12,1);
  else
  lcd.setCursor(11,1);
  lcd.print(EV);
  


}
void loop() {
  // put your main code here, to run repeatedly:
//lamps();
encoder();
expose();
lamp_flag();

}
void encoder(){ 
  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
  SW_State = digitalRead(SW);

  if(!SW_State)                   // axis pushed
  {while(!SW_State)
{   SW_State = digitalRead(SW);
    SW_counter++;}
  if(SW_counter>SW_delay)         // if encoder axes was been holdedd
  {lcd.clear();
    lcd.print(SW_counter);
    SW_counter=0;}
  else
  {m++;
  if(m==m_max)
  m=0;
  if(m==1)
        {
          lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Base time   Sec");
        if(bt<60000){
        lcd.setCursor(0,1);
        lcd.print(bt/1000,bt_a);   
              lcd.setCursor(5,1);
              lcd.print("SEC");}
           
        }
    if(m==0)
          {
            lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("EV=");
          lcd.setCursor(0,1);
          lcd.print(EV);  
          }
    }
  }
  
if(!encoder_A&&encoder_A_prev)
{if(!encoder_B)
      {if(m==0)
      {EV=EV-0.3333333;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("EV=");
        lcd.setCursor(0,1);
        lcd.print(EV);}

              if(m==1)
              {if(bt>1000)
                bt=bt-1000;
                else
                {bt=bt-100;
                bt_a=1;}
                if(bt<0)
                bt=0;
                
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Base time");
                      lcd.setCursor(0,1);
                      lcd.print(bt/1000,bt_a);
                      lcd.setCursor(5,1);
                      lcd.print("SEC");}
                }
                else
                      {if(m==0)
      {EV=EV+0.3333333;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("EV=");
        lcd.setCursor(0,1);
        lcd.print(EV);}

              if(m==1)
              {if(bt>=1000)
                {bt=bt+1000;
                 bt_a=0;}
                else
                bt=bt+100;
                
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Base time");
                      lcd.setCursor(0,1);
                      lcd.print(bt/1000,bt_a);
                      lcd.setCursor(5,1);
                      lcd.print("SEC");}
                }
  }

    encoder_A_prev = encoder_A;

  }
void expose() { 
    be= digitalRead(exp_but);
    if(be)
    be_counter++;
    else 
    be_counter=0;
    if(be_counter>be_delay)
                  {delay(exp_del);
                   EXP=bt*pow(2,EV);
                  T1=millis();
                  T2=T1;
                  digitalWrite(enl,HIGH);
                  lcd.clear();
                            while(T1+EXP>T2)
                            {T2=millis();
                             T_now=(EXP+T1-T2)/1000;
                            if(T_now<100&&T_last>=100||T_now<10&&T_last>=10||T_now<10&&T_last>=10)
                            lcd.clear();
                            T_last=T_now;
                            lcd.setCursor(8,1);
                            lcd.print(T_last);}
                  digitalWrite(enl,LOW);
                  delay(100);
                  digitalWrite(lamp,HIGH);
                   lcd.clear();
  lcd.print("Base time");
  lcd.setCursor(14,0);
  lcd.print("EV");
  lcd.setCursor(0,1);
  lcd.print(bt/1000,bt_a);
  lcd.setCursor(5,1);
  lcd.print("Sec");
  if(EV>=0)
  lcd.setCursor(12,1);
  else
  lcd.setCursor(11,1);
  lcd.print(EV);
                  EEPROM.put(bt_adr,bt);
                  EEPROM.put(EV_adr,EV);
                  be_counter=0;
                    }
     
}
void lamp_flag(){
  bv=digitalRead(view_but);
if(bv){
              while(bv)
              {bv=digitalRead(view_but);
               if(bv_counter<bv_delay)
               bv_counter++;
               if(bv_counter==bv_delay)
               {view_flag=2;
               lamp_io();}}
                    if(bv_counter<bv_delay)                    
                    {if(view_flag==1)
                    view_flag=0;
                    else
                    view_flag=1;}
              bv_counter=0;
              lamp_io();
              delay(100);}
  }
  void lamp_io()  {
 if(view_flag==0)
         {digitalWrite(enl,LOW);
          digitalWrite(lamp,HIGH);}
 if(view_flag==1)
         {digitalWrite(enl,HIGH);
          digitalWrite(lamp,LOW);}
 if(view_flag==2)
         {digitalWrite(enl,LOW);
          digitalWrite(lamp,LOW);}
         
  }
