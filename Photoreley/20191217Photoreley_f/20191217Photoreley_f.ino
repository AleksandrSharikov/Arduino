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
byte A=10;//displey light

//buttons
byte encoder_A;
byte encoder_B;
byte SW_State;
byte encoder_A_prev;
byte SW_State_prev;
float SW_counter;
float SW_delay=50000;
byte be;                //exposure and view buttons
byte bv;
byte be_counter;
float bv_counter;
byte be_delay=100;
float bv_delay=100000;
byte m_max=3; // quantity of encoder sets

//eeprom
byte EV_adr=3;
byte bt_adr=7;
byte fev_adr=12;

//flags
byte view_flag;
byte encoder_flag;// encoder mode
byte en_flag=0;// encoder on
byte ae_flag;//afterexpose flag
byte fe_flag;// forck encoder flag
byte begf_flag;// firsr touch forck encoder flag
byte f_i; // index of chosen forck set
//constants

int exp_del=1000;//exposure delay
float en_del=10000; //encoder delay

//variables
byte m; //vhat change by encoder
byte bt_a; //bt aquracy
float EV=0;
float forck_EV;//
float forck_show;
float EXP; // exposure time
float T1,T2;
float T_last,T_now;
float bt=10000;//base time
float T1_e,T2_e;



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

byte DARK[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte NORM[8] = {
  0b11111,
  0b11011,
  0b10101,
  0b11011,
  0b10101,
  0b11011,
  0b10101,
  0b11111
};
byte BRIGHT[8] = {
  0b11111,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b11111
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
  pinMode(A,OUTPUT);  
  digitalWrite(A,HIGH);

lcd.begin(16,2);
lcd.createChar(1,PM);
lcd.createChar(2,DARK);
lcd.createChar(3,NORM);
lcd.createChar(4,BRIGHT);
//lcd.createChar(5,UVR);
//lcd.createChar(6,C);
//lcd.createChar(7,L);
//lcd.createChar(8,R);

EEPROM.get(bt_adr,bt);
EEPROM.get(EV_adr,EV);
EEPROM.get(fev_adr,forck_EV);
forck_EV=1;
  if(bt>1000)
  bt_a=1;
  EXP=bt*pow(2,EV);
  
  lcd.clear();
  lcd.print("Base time");
  lcd.setCursor(14,0);
  lcd.print("EV");
  lcd.setCursor(0,1);
  showtime(bt);
  if(EV==0)
  lcd.setCursor(14,1);
  else
  lcd.setCursor(11,1);
  if(EV>0)
  lcd.print("+");
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
  T2_e=millis();

  if(!SW_State)                   // axis pushed
  {T1_e=T2_e;
  en_flag=1;
    while(!SW_State)
{   SW_State = digitalRead(SW);
    if(SW_counter<SW_delay);
    SW_counter++;
    if(SW_counter==SW_delay)         // if encoder axes was been holdedd
   EV_bt();
    }
    

  if(SW_counter<SW_delay)
  {m++;
  if(m==m_max)
  m=0;
  if(m==1)
        {
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Base time SEC");        
        lcd.setCursor(6,1);
        showtime(bt);           
        }
    if(m==0)
          {
          lcd.clear();
          lcd.setCursor(4,0);
          lcd.print("EV=");
          lcd.setCursor(6,1);
          if(EV>0)
          lcd.print("+");
          lcd.print(EV);  
          }
     if(m==2)
     {     
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Base time MIN");        
        lcd.setCursor(6,1);
        showtime(bt); 
      }     
    }
    SW_counter=0;
  }
  
if(!encoder_A&&encoder_A_prev)
{T1_e=T2_e;
en_flag=1;
  if(!encoder_B)
      {if(m==0)
      {EV=EV-0.3333333;
        lcd.clear();
        lcd.setCursor(4,0);       
        lcd.print("EV=");
        lcd.setCursor(6,1);
        if(EV>0)
        lcd.print("+");
        lcd.print(EV);}

              if(m==1)
              {if(bt>3000)
                bt=bt-1000;
                else
                bt=bt-100;
                 if(bt<0)
                bt=0;
                
                      lcd.clear();
                      lcd.setCursor(2,0);
                      lcd.print("Base time");
                      lcd.setCursor(6,1);
                      showtime(bt);}

                      if(m==2)
              {if(bt>60000)
                bt=bt-60000;
                else
                m=1;
                if(bt<0)
                bt=0;
                
                      lcd.clear();
                      lcd.setCursor(2,0);
                      lcd.print("Base time");
                      lcd.setCursor(6,1);
                      showtime(bt);}
                }
                else
                      {if(m==0)
      {EV=EV+0.3333333;
        lcd.clear();
        lcd.setCursor(4,0);
        lcd.print("EV=");
        lcd.setCursor(6,1);
        if(EV>0)
        lcd.print("+");
        lcd.print(EV);}

              if(m==1)
              {if(bt>=5000)
                bt=bt+1000;
                else
                bt=bt+100;
                
                      lcd.clear();
                      lcd.setCursor(2,0);
                      lcd.print("Base time");
                      lcd.setCursor(6,1);
                      showtime(bt);}
                      
                      if(m==2)
              {      bt=bt+60000;
                                            
                      lcd.clear();
                      lcd.setCursor(2,0);
                      lcd.print("Base time");
                      lcd.setCursor(6,1);
                      showtime(bt);}
                }
  }
              if(T2_e-T1_e>en_del&&en_flag==1)
              {  lcd.clear();
                  lcd.print("Base time");
                  lcd.setCursor(14,0);
                  lcd.print("EV");
                  lcd.setCursor(0,1);
                  showtime(bt);
                  if(EV==0)
                  lcd.setCursor(14,1);
                  else
                  lcd.setCursor(11,1);
                  if(EV>0)
                  lcd.print("+");
                  lcd.print(EV);
                  en_flag=0;
                  m=0;}
    encoder_A_prev = encoder_A;

  }
void expose() { 
    be= digitalRead(exp_but);
    if(be)
    be_counter++;
    else 
    be_counter=0;
    if(be_counter>be_delay)
                  { 
                    
                  digitalWrite(lamp,LOW);
                   delay(exp_del);
                   EXP=bt*pow(2,EV);

                  
                  lcd.clear();
                  lcd.setCursor(0,0);
                  lcd.print("(");
                  showtime(bt);
                  lcd.print(")");
                  if(EV>=0)
                  lcd.print("+");
                  lcd.print(EV);
                  lcd.print("EV");                  
                  lcd.print("=");
                  lcd.setCursor(0,1);
                  showtime(EXP);
                  digitalWrite(enl,HIGH);                  
                  digitalWrite(lamp,HIGH);
                  T1=millis();
                  T2=T1;
                            while(T1+EXP>T2)
                            {T2=millis();
                             T_now=(EXP+T1-T2);
                            if(T_now<9500&&T_last>=9500||T_now<60000&&T_last>60000||T_now<600000&&T_last>600000)
                            {lcd.setCursor(11,1);
                              lcd.print("      ");}
                            T_last=T_now;
                            lcd.setCursor(11,1);
                            showtime(T_last);}
                  digitalWrite(enl,LOW);
                  digitalWrite(A,LOW);
                  digitalWrite(lamp,LOW);
                  delay(2000);
                  digitalWrite(A,HIGH);
                  digitalWrite(lamp,HIGH);
                   
                  EEPROM.put(bt_adr,bt);
                  EEPROM.put(EV_adr,EV);
                  afterexpose();
                  be_counter=0;
                    }     
}
void lamp_flag(){
  bv=digitalRead(view_but);
if(bv){
              while(bv)
              {bv=digitalRead(view_but);
               be=digitalRead(exp_but);
               if(bv_counter<bv_delay)
               bv_counter++;
               if(be)
               forck();
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
void EV_bt() {
  lcd.setCursor(0,0);
  lcd.print("bt*2^EV=>bt");
  lcd.setCursor(0,1);
  showtime(bt);
  lcd.setCursor(6,1);
  lcd.print("=>");
  lcd.setCursor(8,1);
bt=bt*pow(2,EV);
if(bt<5000)
bt=round(bt/100)*100;
else
bt=round(bt/1000)*1000;
showtime(bt);
EV=0;
delay(2000);
  lcd.clear();
  lcd.print("Base time");
  lcd.setCursor(14,0);
  lcd.print("EV");
  lcd.setCursor(0,1);
  showtime(bt);
  if(EV==0)
  lcd.setCursor(14,1);
  else
  lcd.setCursor(11,1);
  if(EV>0)
  lcd.print("+");
  lcd.print(EV);
}
void showtime(float t){
    if(t>60000)
  {   lcd.print(floor(t/60000),0);
      lcd.print(":");
      if((t-floor(t/60000)*60000)/1000<9.5)
      lcd.print("0");
      lcd.print((t-floor(t/60000)*60000)/1000,0);  
  }
  else
  {
  if(t>5000)
  lcd.print(t/1000,0);
  else
  lcd.print(t/1000,1);
  //lcd.print(" sec"); 
  }
} 
void afterexpose(){
  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("after exp");
                  lcd.setCursor(0,1);
               //   lcd.print("(");
                  showtime(bt);
              //    lcd.print(")");
                  if(EV>=0)
                  lcd.print("+");
                  lcd.print(EV);
                  lcd.print("EV");                  
                  lcd.print("=");
               //   lcd.setCursor(0,1);
                  showtime(EXP);
  T1=millis();
while(ae_flag==0)
{  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
  SW_State = digitalRead(SW);
  be= digitalRead(exp_but);
  bv=digitalRead(view_but);
  
  T2=millis();
  T_now=T2-T1+1000;
                            if(T_now>7000&&T_last<=7000)
                            {lcd.setCursor(0,0);
                              lcd.print("      ");}
                            T_last=T_now;
                            lcd.setCursor(0,0);
                            showtime(T_last);
                            if(!SW_State||!encoder_A||be||bv)
                            ae_flag=1;                          
  }
  ae_flag=0;
  delay(2000);
  lcd.clear();
  lcd.print("Base time");
  lcd.setCursor(14,0);
  lcd.print("EV");
  lcd.setCursor(0,1);
  showtime(bt);
  lcd.setCursor(11,1);
  if(EV>0)
  lcd.print("+");
  lcd.print(EV);
}
void forck(){
  fe_flag=0;
  lcd.clear();
   lcd.setCursor(6,0);
  lcd.print("forck");
   lcd.setCursor(7,1);
  lcd.print("\1");
  lcd.print(forck_EV);
  while(be){be=digitalRead(exp_but);}
  delay(100);
  while(fe_flag==0){
  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
  SW_State = digitalRead(SW);
  be= digitalRead(exp_but);
  bv=digitalRead(view_but);

if(!encoder_A&&encoder_A_prev)
{if(begf_flag==0)
{lcd.clear();
lcd.setCursor(5,0);
lcd.print("\1"); 
lcd.setCursor(6,1);
showtime(bt);  
//lcd.setCursor(12,0);
//lcd.print(EV);
begf_flag=1;}
  if(!encoder_B)
  {forck_EV=forck_EV-0.3333333333333;
  if(forck_EV<0)
  forck_EV=0;
   }
  else
    forck_EV=forck_EV+0.3333333333333;
 forck_show=EV-forck_EV;
  lcd.setCursor(0,1);
  if(forck_show>0)
  lcd.print("+");
  lcd.print(forck_show); 
  lcd.print("EV"); 

  forck_show=EV+forck_EV;
  lcd.setCursor(11,1);
  if(forck_show>0)
  lcd.print("+");
  lcd.print(forck_show);
  lcd.print("EV"); 

  forck_show=abs(forck_EV);
  lcd.setCursor(6,0);
  lcd.print(forck_show);
  lcd.print("EV"); 
     
  }  
  encoder_A_prev = encoder_A;
  if(be)
  {fe_flag=1;
  forck_exposure();
    }
  }
  }
void forck_exposure(){
  while(be){be=digitalRead(exp_but);}
  delay(100);
  EEPROM.put(fev_adr,forck_EV);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("first exposure");
  lcd.setCursor(2,1);
  lcd.print("add time");
  do{be=digitalRead(exp_but);}while(!be);
  
  lcd.setCursor(2,1);
  lcd.print("             ");
                     
                   delay(exp_del);
                   EXP=bt*(pow(2,EV+forck_EV)-pow(2,EV));

                  

                  lcd.setCursor(0,1);
                  showtime(EXP);
                  digitalWrite(enl,HIGH);
                  T1=millis();
                  T2=T1;
                            while(T1+EXP>T2)
                            {T2=millis();
                             T_now=(EXP+T1-T2);
                            if(T_now<9500&&T_last>=9500||T_now<60000&&T_last>60000||T_now<600000&&T_last>600000)
                            {lcd.setCursor(11,1);
                              lcd.print("      ");}
                            T_last=T_now;
                            lcd.setCursor(11,1);
                            showtime(T_last);}
                  digitalWrite(enl,LOW);
                 // digitalWrite(A,LOW);
                  delay(2000);
                 // digitalWrite(A,HIGH);
                  digitalWrite(lamp,HIGH);

lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("second exposure");
  lcd.setCursor(2,1);
  lcd.print("normal time");
  do{be=digitalRead(exp_but);}while(!be);
 
  lcd.setCursor(2,1);
  lcd.print("             ");
                    
                   delay(exp_del);
                   EXP=bt*(pow(2,EV)-pow(2,EV-forck_EV));

                  

                  lcd.setCursor(0,1);
                  showtime(EXP);
                  digitalWrite(enl,HIGH);
                  T1=millis();
                  T2=T1;
                            while(T1+EXP>T2)
                            {T2=millis();
                             T_now=(EXP+T1-T2);
                            if(T_now<9500&&T_last>=9500||T_now<60000&&T_last>60000||T_now<600000&&T_last>600000)
                            {lcd.setCursor(11,1);
                              lcd.print("      ");}
                            T_last=T_now;
                            lcd.setCursor(11,1);
                            showtime(T_last);}
                  digitalWrite(enl,LOW);
                 // digitalWrite(A,LOW);
                  delay(2000);
                 // digitalWrite(A,HIGH);
                  digitalWrite(lamp,HIGH);

                   lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("third exposure");
  lcd.setCursor(2,1);
  lcd.print("incr time");
  do{be=digitalRead(exp_but);}while(!be);
  
  lcd.setCursor(2,1);
  lcd.print("             ");
                    
                   delay(exp_del);
                   EXP=bt*pow(2,EV-forck_EV);

                  

                  lcd.setCursor(0,1);
                  showtime(EXP);
                  digitalWrite(enl,HIGH);
                  T1=millis();
                  T2=T1;
                            while(T1+EXP>T2)
                            {T2=millis();
                             T_now=(EXP+T1-T2);
                            if(T_now<9500&&T_last>=9500||T_now<60000&&T_last>60000||T_now<600000&&T_last>600000)
                            {lcd.setCursor(11,1);
                              lcd.print("      ");}
                            T_last=T_now;
                            lcd.setCursor(11,1);
                            showtime(T_last);}
                  digitalWrite(enl,LOW);
                //  digitalWrite(A,LOW);
                  delay(2000);
                //  digitalWrite(A,HIGH);
                  digitalWrite(lamp,HIGH);


                   afterforck();                      
                    
  }
void afterforck(){
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("-");
    lcd.setCursor(7,0);
    lcd.print("0");
    lcd.setCursor(14,0);
    lcd.print("+");

    lcd.setCursor(1,1);
    lcd.print("\4");
    lcd.setCursor(7,1);
    lcd.print("\3");
    lcd.setCursor(14,1);
    lcd.print("\2");
    f_i=1;
    lcd.setCursor(0,0);
    lcd.print(" ");
    lcd.setCursor(6,0);
    lcd.print("(");
    lcd.setCursor(13,0);
    lcd.print(" ");
    lcd.setCursor(2,0);
    lcd.print(" ");
    lcd.setCursor(8,0);
    lcd.print(")");
    lcd.setCursor(15,0);
    lcd.print(" ");
    
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.setCursor(6,1);
    lcd.print("(");
    lcd.setCursor(13,1);
    lcd.print(" ");
    lcd.setCursor(2,1);
    lcd.print(" ");
    lcd.setCursor(8,1);
    lcd.print(")");
    lcd.setCursor(15,1);
    lcd.print(" ");
    fe_flag=0;
    while(fe_flag==0)
{  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
  SW_State = digitalRead(SW);
  be= digitalRead(exp_but);
  bv=digitalRead(view_but);
  if(!encoder_A&&encoder_A_prev)
{if(encoder_B)
{f_i++;
if(f_i==3)
f_i=0;}
else
{f_i=f_i-1;
if(f_i==255)
f_i=2;}

if(f_i==0){
            lcd.setCursor(0,0);
    lcd.print("(");
    lcd.setCursor(6,0);
    lcd.print(" ");
    lcd.setCursor(13,0);
    lcd.print(" ");
    lcd.setCursor(2,0);
    lcd.print(")");
    lcd.setCursor(8,0);
    lcd.print(" ");
    lcd.setCursor(15,0);
    lcd.print(" ");
    
    lcd.setCursor(0,1);
    lcd.print("(");
    lcd.setCursor(6,1);
    lcd.print(" ");
    lcd.setCursor(13,1);
    lcd.print(" ");
    lcd.setCursor(2,1);
    lcd.print(")");
    lcd.setCursor(8,1);
    lcd.print(" ");
    lcd.setCursor(15,1);
    lcd.print(" ");}

    
if(f_i==1){
            lcd.setCursor(0,0);
    lcd.print(" ");
    lcd.setCursor(6,0);
    lcd.print("(");
    lcd.setCursor(13,0);
    lcd.print(" ");
    lcd.setCursor(2,0);
    lcd.print(" ");
    lcd.setCursor(8,0);
    lcd.print(")");
    lcd.setCursor(15,0);
    lcd.print(" ");
    
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.setCursor(6,1);
    lcd.print("(");
    lcd.setCursor(13,1);
    lcd.print(" ");
    lcd.setCursor(2,1);
    lcd.print(" ");
    lcd.setCursor(8,1);
    lcd.print(")");
    lcd.setCursor(15,1);
    lcd.print(" ");}

    
if(f_i==2){
            lcd.setCursor(0,0);
    lcd.print(" ");
    lcd.setCursor(6,0);
    lcd.print(" ");
    lcd.setCursor(13,0);
    lcd.print("(");
    lcd.setCursor(2,0);
    lcd.print(" ");
    lcd.setCursor(8,0);
    lcd.print(" ");
    lcd.setCursor(15,0);
    lcd.print(")");
    
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.setCursor(6,1);
    lcd.print(" ");
    lcd.setCursor(13,1);
    lcd.print("(");
    lcd.setCursor(2,1);
    lcd.print(" ");
    lcd.setCursor(8,1);
    lcd.print(" ");
    lcd.setCursor(15,1);
    lcd.print(")");}
  }  
  if(be)
  {if(f_i==0)
   EV=EV-forck_EV;
   if(f_i==2)
   EV=EV+forck_EV;
   fe_flag=1;
   while(be){be=digitalRead(exp_but);}
   delay(100);}
  encoder_A_prev=encoder_A;}
  afterexpose();
  }
  

