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

LiquidCrystal lcd(RS,E,DB4,DB5,DB6,DB7);

byte CLK=7;   //encoder
byte DT=8;
byte SW=9;

//constants and presets
int exp_del=1500;// delay before exposure
int toe=1000; // encoder opening time
double EXP=10000;//exposure time
unsigned long bt=10000; // base time
byte en=3;// nomber of exposes in the fork
float fs=0.7; //fork size
float sh;//fork shift
int sd=500;//shuter delay
byte qs=3; //quolity of set points




//flags
byte view_flag;
byte encoder_flag=0;
byte s_sh_flag=0;// shift center, or change fork size


         long unsigned te0;
         long unsigned te1;
         long unsigned ted;
         byte mfl=0;

         
//ee adrwss
byte bt_adr=10;



//buttons
byte eb;
byte vb;
byte vb_c;// vb puting counter
byte vb_d=10;// vb delay
byte exp_c;// vb puting counter
byte exp_d=10;// vb delay
byte encoder_A;
byte encoder_B;
byte SW_State;
byte encoder_A_prev;
byte SW_State_prev;
byte SW_c;
byte SW_d;



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

float EV=0;



void setup() {
  // put your setup code here, to run once:
  pinMode(enl,OUTPUT);  
  digitalWrite(enl,LOW);
  pinMode(lamp,OUTPUT); 
  digitalWrite(lamp,LOW);
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
  
  lcd.clear();
  show_quiet();
}

void loop() {
  svitch();
  encoder();
  expose();
}
  void svitch()  //                       lamp/enlager
  {
  vb=digitalRead(view_but);
  if(vb==HIGH)
  vb_c++;
  else
  vb_c=0;
  if(vb_c==vb_d)
    {
 if(view_flag==0)
    {
      digitalWrite(enl,HIGH);
      digitalWrite(lamp,LOW);
      }
 if(view_flag==1)
    {
      digitalWrite(enl,LOW);
      digitalWrite(lamp,HIGH);
      }
       if(view_flag==1)
       view_flag=0; 
       if(view_flag==0)
       view_flag=1;
       vb_c=0;       
    }   
    }
  void expose()  //                       expose
  {
    eb=digitalRead(exp_but);
  if(eb==HIGH)
  exp_c++;
  else
  exp_c=0;
  if(exp_c==exp_d);
      {digitalWrite(enl,LOW);
       digitalWrite(lamp,LOW);
       delay(exp_del);
  enlarge(EXP,1);
  EEPROM.update(bt_adr,bt);
  exp_c=0;
  show_quiet();}
    }
  void encoder() //                                                                encoder
   {
 
  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
  SW_State = digitalRead(SW);
    if(!SW_State)  //                                  pushing encoder axe
  SW_c++;
  else
  SW_c=0;
  if(SW_c==SW_d)
  {lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("base time set");
  bt=bt*pow(2,EV);
  time_print(bt,5,1);
  }

 if(!encoder_A||!SW_State)
   { 


          vb_c=0;

         
 
  encoder_A_prev = HIGH;
te0=millis(); 
lcd.clear(); 
  lcd.setCursor(7,0);
  lcd.print("EV");
 while(ted<toe)
 {

  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
  SW_State = digitalRead(SW);
  vb=digitalRead(view_but);

  te1=millis();
  ted=te1-te0;
  
  if(!SW_State)  //                                  pushing encoder axe
  SW_c++;
  else
  SW_c=0;
  if(SW_c==SW_d)
  {encoder_flag++;
  if(encoder_flag==qs)
  encoder_flag=0;
  lcd.clear(); 
  lcd.setCursor(7,0);
  if(encoder_flag==0)
  lcd.print("EV");
  if(encoder_flag==1)
 { lcd.print(char(1));
 lcd.print("SEC*10");}
  if(encoder_flag==2)
 { lcd.print(char(1));
 lcd.print("SEC/10");}
  SW_c=0;
  te0=millis();
  }

  if(encoder_flag==0)//                                                                         EV
  {
  if(!encoder_A && encoder_A_prev)
  {
     if(encoder_B)
   {
       
      EV=EV-0.1;
      if(EV<-5)
      EV=-5;
     lcd.setCursor(7,1);
     lcd.print(EV);
     
     te0=millis();
    }
       else
   {
       
      EV=EV+0.1;
      if(EV>5)
      EV=5;
     lcd.setCursor(7,1);
     lcd.print(EV);
     
     te0=millis();
    }
  }
    encoder_A_prev = encoder_A;
 }


  if(encoder_flag==1)//                                                                           sec*10
  {
  if(!encoder_A && encoder_A_prev)
  {
     if(encoder_B)
   {
       
      bt=bt-10000;
      if(bt<0)
      bt=0;
     time_print(bt,7,1);
     
     te0=millis();
    }
       else
   {
       
       
      bt=bt+10000;
      time_print(bt,7,1);
     
     te0=millis();
    }
  }
    encoder_A_prev = encoder_A;
 }

  if(encoder_flag==2)//                                                         sec/10
  {
  if(!encoder_A && encoder_A_prev)
  {
     if(encoder_B)
   {
       
      bt=bt-100;
      if(bt<0)
      bt=0;
      time_print(bt,7,1);
     
     te0=millis();
    }
       else
   {     
     
      bt=bt+100;
      time_print(bt,7,1);     
     te0=millis();
    }
  }
    encoder_A_prev = encoder_A;
 }
 fork();
 } //cicle ends
EXP=pow(2,EV)*bt;
show_quiet();
   }     
}   
void enlarge(unsigned long T,byte show_flag)//                 Enlarge
    {
     unsigned long T1,T2,DT;
      T1=millis();
      T2=T1;
            
      digitalWrite(enl,HIGH);
      digitalWrite(lamp,HIGH);
      while(DT<T)
      {
   //     show_exp(show_fag);
        T2=millis();
        DT=T2-T1;
        }
      digitalWrite(enl,LOW);
      digitalWrite(lamp,HIGH);
      view_flag=0;
      }
void fork()//                                              fork
{ 
  vb=digitalRead(view_but);
  if(vb==HIGH)
  vb_c++;
  else
  vb_c=0;
  if(vb_c==vb_d)
  { 
    vb_c=0;
    lcd.clear(); 
      lcd.setCursor(0,0);
      lcd.print("Quontity exposes");
      lcd.setCursor(1,10);
      lcd.print("VB=OK");//                           quontity of exposes   
    while(vb_c<vb_d){
  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
  vb=digitalRead(view_but);

        
  if(!encoder_A && encoder_A_prev)
  {
     if(encoder_B)
   {
       
      en=en-1;
      if(en<1)
      en=4;
      }
       else
   {     
      en=en+1;
      if(en>4)
      en=1;

    }     
    lcd.setCursor(3,1);
    lcd.print(en);
  }
  encoder_A_prev = encoder_A;
  if(vb==HIGH)
  vb_c++;
  else
  vb_c=0;
 }
  vb_c=0;//                                                 fork size
      
       lcd.clear();
      lcd.setCursor(7,0);
      lcd.print("VB=OK"); 
      delay(500);
      if(en!=1)  
    while(vb_c<vb_d){
  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
  SW_State = digitalRead(SW);
  vb=digitalRead(view_but);
if(!SW_State)
{if(s_sh_flag==0)
    {s_sh_flag=1;
    lcd.setCursor(0,0);
    lcd.print(" ");
    lcd.setCursor(15,0);
    lcd.print("Sh");}
if(s_sh_flag=1)
    {s_sh_flag=0;
    lcd.setCursor(0,0);
    lcd.print(char(1));
    lcd.setCursor(15,0);
    lcd.print("  ");}
  }
      
  if(!encoder_A && encoder_A_prev)
  {
    if(s_sh_flag=0)
     {if(encoder_B)
   {       
      fs=fs-0.1;
      if(fs<0.1)
      fs=0.1;
      }
       else
   {  
      fs=fs+0.1;
      if(fs>3)
      fs=3;
     }}
     if(s_sh_flag=1)     
     {if(encoder_B)
   {       
      sh=sh-0.1;
      if(sh<-5)
      sh=-5;
      }
       else
   {       
      sh=sh+0.1;
      if(sh<5)
      sh=5;
     }}
show_fork();
  }
    encoder_A_prev = encoder_A;
  if(vb==HIGH)
  vb_c++;
  else
  vb_c=0;
 }
    fork_exposure(); 
    fork_choice();
    ted=toe+1;
    vb_c=0;}
  }

void show_exp(byte show_flag)
      {}
void fork_exposure()//                                            fork exposure
{ 
  exp_c=0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("First exposure");  
  lcd.setCursor(0,1);
  lcd.print("push exp");
  EXP=pow(2,EV)*bt;
  //d_EXP=bt*(pow(2,EV+sh)-pow(2,EV+sh-fs));
  while(exp_c<exp_d)
  {eb=digitalRead(exp_but);
  if(eb==HIGH)
  exp_c++;
  else
  exp_c=0;}
  exp_c=0;
  delay(sd);
                          if(en==2)
                          {double EXP1,EXP2;
                         EXP1=bt*(pow(2,EV+sh+fs)-pow(2,EV+sh-fs));
                         EXP2=bt*pow(2,EV+sh-fs);
                           enlarge(EXP1,2);
                           lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("Second exposure");  
                            lcd.setCursor(0,1);
                            lcd.print("push exp");
                            while(exp_c<exp_d)
                            {eb=digitalRead(exp_but);
                            if(eb==HIGH)
                            exp_c++;
                            else
                            exp_c=0;}
                            exp_c=0;
                            delay(sd);
                           enlarge(EXP2,2);                           
                            }
                          if(en==3)
                          {double EXP1,EXP2,EXP3;                          
                         EXP1=bt*(pow(2,EV+sh+fs)-pow(2,EV+sh));
                         EXP2=bt*(pow(2,EV+sh)-pow(2,EV+sh-fs));
                         EXP3=bt*pow(2,EV+sh-fs);
                           enlarge(EXP1,2);
                           lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("Second exposure");  
                            lcd.setCursor(0,1);
                            lcd.print("push exp");
                            while(exp_c<exp_d)
                            {eb=digitalRead(exp_but);
                            if(eb==HIGH)
                            exp_c++;
                            else
                            exp_c=0;}
                            exp_c=0;
                            delay(sd);
                           enlarge(EXP2,2);                           
                            
                           lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("Third exposure");  
                            lcd.setCursor(0,1);
                            lcd.print("push exp");
                            while(exp_c<exp_d)
                            {eb=digitalRead(exp_but);
                            if(eb==HIGH)
                            exp_c++;
                            else
                            exp_c=0;}
                            exp_c=0;
                            delay(sd);
                           enlarge(EXP3,2);
                            }
                          if(en==4)
                          {double EXP1,EXP2,EXP3,EXP4;                                 
                         EXP1=bt*(pow(2,EV+sh+2*fs)-pow(2,EV+sh+fs));                   
                         EXP2=bt*(pow(2,EV+sh+fs)-pow(2,EV+sh-fs));
                         EXP3=bt*(pow(2,EV+sh-fs)-pow(2,EV+sh-2*fs));
                         EXP4=bt*pow(2,EV+sh-2*fs);
                           enlarge(EXP1,2);
                           lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("Second exposure");  
                            lcd.setCursor(0,1);
                            lcd.print("push exp");
                            while(exp_c<exp_d)
                            {eb=digitalRead(exp_but);
                            if(eb==HIGH)
                            exp_c++;
                            else
                            exp_c=0;}
                            exp_c=0;
                            delay(sd);
                           enlarge(EXP2,2);                           
                            
                           lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("Third exposure");  
                            lcd.setCursor(0,1);
                            lcd.print("push exp");
                            while(exp_c<exp_d)
                            {eb=digitalRead(exp_but);
                            if(eb==HIGH)
                            exp_c++;
                            else
                            exp_c=0;}
                            exp_c=0;
                            delay(sd);
                           enlarge(EXP3,2);
                           
                           lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("Fourth exposure");  
                            lcd.setCursor(0,1);
                            lcd.print("push exp");
                            while(exp_c<exp_d)
                            {eb=digitalRead(exp_but);
                            if(eb==HIGH)
                            exp_c++;
                            else
                            exp_c=0;}
                            exp_c=0;
                            delay(sd);
                           enlarge(EXP3,2);}
  }
void fork_choice()
{
  lcd.clear();
   if(en=2)
     {lcd.setCursor(0,0);
     lcd.print("-");
     //lcd.print(fs-sh);
     lcd.setCursor(10,0);
     lcd.print("+");
     //lcd.print(fs+sh);

     lcd.setCursor(0,1);
     lcd.print('\2');
     //lcd.print(fs-sh);
     lcd.setCursor(10,1);
     lcd.print('\2');
     //lcd.print(fs+sh);
     lcd.setCursor(12,0);
     lcd.print("EV");
     }
     
     if(en=3)
     {
     lcd.setCursor(5,0);
     lcd.print("0");
     
     lcd.setCursor(0,0);
     lcd.print("-");
     //lcd.print(fs-sh);
     lcd.setCursor(10,1);
     lcd.print("+");
     //lcd.print(fs+sh);
     
     lcd.setCursor(0,1);
     lcd.print('\2');
     //lcd.print(fs-sh);
     lcd.setCursor(10,1);
     lcd.print('\2');
     //lcd.print(fs+sh);
     lcd.setCursor(12,0);
     lcd.print("EV");
     lcd.setCursor(5,1);
     lcd.print('\2');
     }
     if(en=4)
     {
     lcd.setCursor(0,0);
     lcd.print("-2");
     //lcd.print(2*fs-sh);
     lcd.setCursor(11,0);
     lcd.print("+2");
     //lcd.print(2*fs+sh);
     
     lcd.setCursor(4,0);
     lcd.print("-");
     //lcd.print(fs-sh);
     lcd.setCursor(8,0);
     lcd.print("+");
     //lcd.print(fs+sh);

     
     lcd.setCursor(0,1);
     lcd.print('\2');
     lcd.setCursor(4,1);
     lcd.print('\2');
     lcd.setCursor(8,1);
     lcd.print('\2');
     lcd.setCursor(12,1);
     lcd.print('\2');
     
     lcd.setCursor(13,0);
     lcd.print("EV");
     }
  
  }  
void show_quiet()//                                              LCD0
      {
        lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("EV");
      lcd.setCursor(6,0);
      lcd.print("Time");
      lcd.setCursor(10,0);
      lcd.print("BT");

      
      lcd.setCursor(0,1);
      lcd.print(EV);
      time_print(EXP,6,1);      
      time_print(bt,10,1);
      }
void show_fork()
{     
  if(en=2)
     {lcd.setCursor(3,1);
     lcd.print("-");
     lcd.print(fs-sh);
     lcd.setCursor(10,1);
     lcd.print("+");
     lcd.print(fs+sh);
     }
     
     if(en=3)
     {
     lcd.setCursor(5,1);
     lcd.print(sh);
     
     lcd.setCursor(4,1);
     lcd.print("-");
     lcd.print(fs-sh);
     lcd.setCursor(8,1);
     lcd.print("+");
     lcd.print(fs+sh);
     }
     if(en=4)
     {
     lcd.setCursor(0,1);
     lcd.print("-");
     lcd.print(2*fs-sh);
     lcd.setCursor(12,1);
     lcd.print("+");
     lcd.print(2*fs+sh);
     
     lcd.setCursor(4,1);
     lcd.print("-");
     lcd.print(fs-sh);
     lcd.setCursor(8,1);
     lcd.print("+");
     lcd.print(fs+sh);
     }}

void time_print(double TIM,byte row, byte line)
     {double sec;
     byte mint;
      lcd.setCursor(row,line);      
      if(TIM>=60000)
      {
        mint=floor(TIM/60000);
        sec=(TIM-mint*60000)*1000;
        lcd.print(mint);
        lcd.print(":");
        lcd.print(sec);}
        else
        lcd.print(TIM*1000);
      }
