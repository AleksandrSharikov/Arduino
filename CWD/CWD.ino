#include <LiquidCrystal.h>
#include <EEPROM.h>



byte b1=2; //dash pin
byte b2=3; //dot pin
byte o=5; //output pin
byte s=12; //sound pin
byte reo=0; // reostat pin in cercit 0 analog
byte m1=15;//memory pins
byte m2=16;
byte m3=17;
byte m4=18;
byte RS=6;//display pins
byte E=7;
byte DB4=8;
byte DB5=9;
byte DB6=10;
byte DB7=11;


byte k1=0; //button1 flag
byte k2=0; //button2 flag
byte sf=0; //spase flag
byte nf=0; //next flag
byte mem1=0; //
byte mem2=0; // mem button flag
byte mem3=0; //
byte mem4=0; //

float fc=500; //reostat
//byte simb; //simbol


int fr=1200; //sound friquancy
int dotL=80; //dot lenth
int dashL; //dash lenth
unsigned long t2=0;
unsigned long t1=0;
unsigned long dt;

int bd=400;     // button delay

byte simb[6]={0,0,0,0,0,0};
byte leti=0;
char let;
char D1[16];  //strings of displey
char D2[13];
byte ci=0;//index cursor
byte ci2=0;
byte eefr1=0; // eeprom cel of fr
byte eefr2=1;
byte efr1=0;  // parts of int fr
byte efr2=0;
byte eesp1=2; // eeprom cel of sp
byte eesp2=3;
byte esp1=0;  // parts of int sp
byte esp2=0;

byte full[8]={
  0b00000,
  0b00000,
  0b11011,
  0b11011,
  0b11011,
  0b11011,
  0b00000,
  0b00000};
  byte half[8]={
  0b00000,
  0b00000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b00000,
  0b00000};
  byte bfull[8]={
  0b00010,
  0b00010,
  0b11011,
  0b11011,
  0b11011,
  0b11011,
  0b00010,
  0b00010};
 


  
byte dotch[8]={
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b01110,
  0b01110,
  0b00000,
  0b00000};
  byte dashch[8]={
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b00000,
  0b00000};


LiquidCrystal lcd(RS,E,DB4,DB5,DB6,DB7);

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

lcd.begin(16,2);
//lcd.autoscroll();
pinMode(b1,INPUT);
pinMode(b2,INPUT);
pinMode(o,OUTPUT);
pinMode(m1,INPUT);
pinMode(m2,INPUT);
pinMode(m3,INPUT);
pinMode(m4,INPUT);

lcd.createChar(1,full);
lcd.createChar(2,half);
lcd.createChar(3,dotch);
lcd.createChar(4,dashch);
lcd.createChar(5,bfull);

efr1=EEPROM.read(eefr1);        // checking fr in eeprom
efr2=EEPROM.read(eefr2);
if(efr1!=0||efr2!=0)
 {*((byte*)(&fr))=EEPROM.read(eefr1);
  *((byte*)(&fr)+1)=EEPROM.read(eefr2);}

  esp1=EEPROM.read(eesp1);        // checking sp in eeprom
esp2=EEPROM.read(eesp2);
if(esp1!=0||esp2!=0)
 {*((byte*)(&dotL))=EEPROM.read(eesp1);
  *((byte*)(&dotL)+1)=EEPROM.read(eesp2);}
}

void loop() {
  noTone(s);
  digitalWrite(o,LOW);
  // put your main code here, to run repeatedly:
 
//  fc=analogRead(reo);
//  dotL=(190-150*fc/1000); //dot lenth
  dashL=dotL*3;
  k1=digitalRead(b1);
  k2=digitalRead(b2);
  mem1=digitalRead(m1);
  mem2=digitalRead(m2);
  mem3=digitalRead(m3);
  mem4=digitalRead(m4);

if(k1==HIGH && k2==LOW) //dash
dash();
if(k1==LOW && k2==HIGH) //dot
dot();
if(k1==HIGH && k2==HIGH) //-.
yamb();



if(k1==LOW && k2==LOW && mem1==HIGH)          // memory buttons
{delay(bd);
mem1=digitalRead(m1);
if(k1==LOW && k2==LOW && mem1==HIGH)
spset();
else
M1();}

if(k1==LOW && k2==LOW && mem1==LOW && mem2==HIGH)
{delay(bd);
mem2=digitalRead(m2);
if(k1==LOW && k2==LOW && mem1==LOW && mem2==HIGH)
frset();
else
M2();  }

if(k1==LOW && k2==LOW && mem1==LOW&&mem2==LOW&&mem3==HIGH)
M3();
if(k1==LOW && k2==LOW && mem1==LOW&&mem2==LOW&&mem3==LOW&&mem4==HIGH)
{delay(bd);
mem4=digitalRead(m4);
if(k1==LOW && k2==LOW && mem1==LOW&&mem2==LOW&&mem3==LOW&&mem4==HIGH)
cl();
  else
M4();
}
//lcd.setCursor (12,1);
//lcd.print("S");
//lcd.print(dotL);
//
//lcd.setCursor (5,1);
//lcd.print("fr");
//lcd.print(fr);

t2=millis();
dt=t2-t1;
if(dt>3*dotL&&nf==0)
next();
if(dt>7*dotL&&sf==0)
spase();

if(sf==0)
{
if(dt<dotL)        //time roll
{lcd.setCursor(13,1);
lcd.print(char(1));
lcd.print(char(5));
lcd.print(char(1));}
else
{
if(dt<2*dotL)        
{lcd.setCursor(13,1);
lcd.print(char(1));
lcd.print(char(5));
lcd.print(char(2));}
else
{
if(dt<3*dotL)        
{lcd.setCursor(13,1);
lcd.print(char(1));
lcd.print(char(5));
lcd.print(" ");}
else
{
if(dt<4*dotL)        
{lcd.setCursor(13,1);
lcd.print(char(1));
lcd.print(char(2));
lcd.print(" ");}
else
{
if(dt<5*dotL)        
{lcd.setCursor(13,1);
lcd.print(char(1));
lcd.print(" ");
lcd.print(" ");}
else
{
if(dt<6*dotL)        
{lcd.setCursor(13,1);
lcd.print(char(2));
lcd.print(" ");
lcd.print(" ");}
else

if(dt<7*dotL)        
{lcd.setCursor(13,1);
lcd.print(" ");
lcd.print(" ");
lcd.print(" ");}

}
}
}
}
}
}
}

    void dash()
{
tone(s,fr);
digitalWrite(o,HIGH);
delay(dashL);
if(leti<5)
simb[leti]=2;
  noTone(s);
  digitalWrite(o,LOW);
//  Serial.print('_');
//disp2('_');
disp2(char(4));
delay(dotL);
t1=millis();
leti=leti+1;

sf=0;
nf=0;
}

    void dot()
{
tone(s,fr);
digitalWrite(o,HIGH);
delay(dotL);
if(leti<5)
simb[leti]=1;
  noTone(s);
  digitalWrite(o,LOW);
 // disp2('.');
//Serial.print(".");
disp2(char(3));
delay(dotL);
t1=millis();
leti=leti+1;
sf=0;
nf=0;
}

   void yamb()
{
tone(s,fr);
digitalWrite(o,HIGH);
delay(dashL);

  noTone(s);
    digitalWrite(o,LOW);
  delay(dotL);

tone(s,fr);
digitalWrite(o,HIGH);
delay(dotL);
  noTone(s);
  digitalWrite(o,LOW);
  delay(dotL);
 // Serial.print("-.");
  t1=millis();
  sf=0;
  nf=0;
  if(leti<4)
  {
simb[leti]=2;
simb[leti+1]=1;
leti=leti+2;
  }
}


void spase()
{
// Serial.print(" ");
char sps=(' ');
 disp(sps);
 //simb=0;
 sf=1; 
  }

  void next()
  {
    int i;
    byte flag;
  //  Serial.print("|");
    leti=0;
    nf=1;
    flag=leter();
    if(flag==1)
{ //Serial.print('[');
//    for(i=0;i<6;i++)
//   {Serial.print(simb[i]);
//   Serial.print(',');
//   }
//   Serial.print("]=>");
//    Serial.print(let);
    disp2('|');
    disp(let);}
       //else
  // { Serial.print("!!mis!!");
    //Serial.print(let);}
   
  for(i=0;i<6;i++)
   simb[i]=0;
    }

    void frset()        //tone friquency set
    {
       byte spfl=0;
      lcd.clear();
      ci=0;
      ci2=0;
      while(k1==LOW&&k2==LOW)
      { k1=digitalRead(b1);
        k2=digitalRead(b2);
        fc=analogRead(reo);
        fr=(2000-1700*fc/1000);
        lcd.setCursor(0,0);
        lcd.print("tone fr =");
        lcd.print(round(fr));
        
        if(spfl==0)
        dot();
        if(spfl==1)
        dash();
        spfl=abs(spfl-1);
       
        }
        
        efr1=*((byte*)(&fr));
        efr2=*((byte*)(&fr)+1);
 EEPROM.write(eefr1,efr1);
 EEPROM.write(eefr2,efr2); 
        lcd.clear();
              }

               void spset()        // speed set
    {
      byte spfl=0;
      lcd.clear();
      ci=0;
      ci2=0;
      while(k1==LOW&&k2==LOW)
      { 
        k1=digitalRead(b1);
        k2=digitalRead(b2);
        fc=analogRead(reo);
         dotL=(190-150*fc/1000); //dot lenth
        lcd.setCursor(0,0);
        lcd.print("input temp =");
        lcd.print(round(dotL));

      //  if(spfl==0)
        dot();
      //  if(spfl==1)
       // dash();
       // spfl=abs(spfl-1);
        }
        
        esp1=*((byte*)(&dotL));
        esp2=*((byte*)(&dotL)+1);
 EEPROM.write(eesp1,esp1);
 EEPROM.write(eesp2,esp2); 
        lcd.clear();
              }
              void cl()         // clean
              {lcd.clear();
              ci=0;
              ci2=0;
                
                }
  
    byte leter()        //determination
    {
      byte i;
      byte flag=0;
      if(simb[5]==0)
      {
      if(simb[0]==1)                    //[1xxxx]
        {
          if(simb[1]==0)
          {let='e';flag=1;}
          if(simb[1]==1)                   //[11xxx]
            {
              if(simb[2]==0)
              {let='i';flag=1;}
              if(simb[2]==1)              //[111xx]
                {
                  if(simb[3]==0)
                  {let='s';flag=1;}
                  if(simb[3]==1)
                      {
                        if(simb[4]==0)
                        {let='h';flag=1;}
                        if(simb[4]==1)
                        {let='5';flag=1;}
                        if(simb[4]==2)
                        {let='4';flag=1;}
                        }
                    if(simb[3]==2)
                        {
                        if(simb[4]==0)
                        {let='v';flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {let='3';flag=1;}
                          }
                }
                          if(simb[2]==2)      //[112XX]
                {
                  if(simb[3]==0)
                  {let='u';flag=1;}
                  if(simb[3]==1)
                      {
                        if(simb[4]==0)
                        {let='f';flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {flag=0;}
                        }
                    if(simb[3]==2)
                        {
                        if(simb[4]==0)
                        {flag=0;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {let='2';flag=1;}
                          }
                     }


             
              }
          
                    if(simb[1]==2)                   //[12xxx]
            {
              if(simb[2]==0)
              {let='a';flag=1;}
              if(simb[2]==1)              //[121xx]
                {
                  if(simb[3]==0)
                  {let='r';flag=1;}
                  if(simb[3]==1)
                      {
                        if(simb[4]==0)
                        {let='l';flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {flag=0;}
                        }
                    if(simb[3]==2)
                     /*   {
                        if(simb[4]==0)
                        {let='v';flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {let='3';flag=1;}
                          } */
                          flag=0;
                }
                          if(simb[2]==2)      //[122XX]
                {
                  if(simb[3]==0)
                  {let='w';flag=1;}
                  if(simb[3]==1)
                      {
                        if(simb[4]==0)
                        {let='p';flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {flag=0;}
                        }
                    if(simb[3]==2)
                        {
                        if(simb[4]==0)
                        {let='j'; flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {let='1';flag=1;}
                          }
                     }


             
              
          }
      }

            if(simb[0]==2)                    //[2xxxx]
        {
          if(simb[1]==0)
          {let='t';flag=1;}
          if(simb[1]==1)                   //[21xxx]
            {
              if(simb[2]==0)
              {let='n';flag=1;}
              if(simb[2]==1)              //[211xx]
                {
                  if(simb[3]==0)
                  {let='d';flag=1;}
                  if(simb[3]==1)
                      {
                        if(simb[4]==0)    //[2111x]
                        {let='b';flag=1;}
                        if(simb[4]==1)
                        {let='6';flag=1;}
                        if(simb[4]==2)
                        {flag=0;}
                        }
                    if(simb[3]==2)       //[2112]
                        {
                        if(simb[4]==0)
                        {let='x';flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {flag=0;}
                          }
                }
                          if(simb[2]==2)      //[212XX]
                {
                  if(simb[3]==0)
                  {let='k';flag=1;}
                  if(simb[3]==1)
                      {
                        if(simb[4]==0)    //[2121x]
                        {let='c';flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {flag=0;}
                        }
                    if(simb[3]==2)     //[2122x]
                        {
                        if(simb[4]==0)
                        {let='y';flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {flag=0;}
                          }
                     }


             
              }
          
                    if(simb[1]==2)                   //[22xxx]
            {
              if(simb[2]==0)
              {let='m';flag=1;}
              if(simb[2]==1)              //[221xx]
                {
                  if(simb[3]==0)
                  {let='g';flag=1;}
                  if(simb[3]==1)    //[2211x]
                      {
                        if(simb[4]==0)
                        {let='z';flag=1;}
                        if(simb[4]==1)
                        {let='7';flag=1;}
                        if(simb[4]==2)
                        {flag=0;}
                        }
                    if(simb[3]==2) //[2212x]
                       {
                        if(simb[4]==0)
                       {let='q';flag=1;}
                        if(simb[4]==1)
                       {flag=0;}
                        if(simb[4]==2)
                      {flag=0;}
                          } 
                }
                

                          if(simb[2]==2)      //[222XX]
                {
                  if(simb[3]==0)
                  {let='o';flag=1;}
                     if(simb[3]==1)              //[2221x]
                      {
                        if(simb[4]==0)
                        {flag=0;}
                        if(simb[4]==1)
                        {let='8';flag=1;}
                        if(simb[4]==2)
                        {flag=0;}
                        }
                    if(simb[3]==2)         //[2222x]
                        {
                        if(simb[4]==0)
                        {flag=0;}
                        if(simb[4]==1)
                        {let='9';flag=1;}
                        if(simb[4]==2)
                        {let='0';flag=1;}
                          }
                     }


             
              }
        }
      }
      else
      if(simb[0]==1 &&simb[1]==1 &&simb[2]==2 &&simb[3]==2 &&simb[4]==1 &&simb[5]==1)
      {let='?';flag=1;}
      
     
      return(flag);
    }
    void M1()  //R2DHS
    {
      dot();  //R
      dash();
      dot();
      next();
      
      dot();  //2
      dot();
      dash();
      dash();
      dash();
      next();

      dash(); //d
      dot();
      dot();
      next();

      dot();  //h
      dot();
      dot();
      dot();
      next();

      dot();  //s
      dot();
      dot();
      next();

      }

      void M2()     //QRS
      {
        dash();    //Q
       dash();
        dot();
        dash();
        next();

dot();            //R
dash();
dot();
next();

       dot();     //S
       dot();
       dot(); 
       next();        
        }

        void M3()  //QSM 
{
   dash();    //Q
       dash();
        dot();
        dash();
        next();

               dot();     //S
       dot();
       dot(); 
       next();  

       dash();          //M
       dash();
       next();
  }
void M4()
{}

  void disp(char a)        //print first string
  {

    char Dt[16];
    int i;
   
  
    if (ci<16)
    {
    lcd.setCursor(ci,0);
    lcd.print(a);
    D1[ci]=a;
    ci=ci+1;
    }
    else
    {
      for(i=0;i<16;i++)     
      Dt[i]=D1[i]; 
      for(i=0;i<15;i++)     
      D1[i]=Dt[i+1];
      D1[15]=a;
      for(i=0;i<16;i++)     
      {lcd.setCursor(i,0);
       lcd.print(D1[i]);
      }
      }
//    { 
//      lcd.scrollDisplayLeft();
//    lcd.print(a);
//   
//    }
//   
    }


  void disp2(char a)              // down row print
   {

    char Dt[12];
    int i;
   
  
    if (ci2<12)
    {
    lcd.setCursor(ci2,1);
    lcd.print(a);
    D2[ci2]=a;
    ci2=ci2+1;
    }
    else
    {
      for(i=0;i<12;i++)     
      Dt[i]=D2[i]; 
      for(i=0;i<11;i++)     
      D2[i]=Dt[i+1];
      D2[11]=a;
      for(i=0;i<12;i++)     
      {lcd.setCursor(i,1);
       lcd.print(D2[i]);
      }
      }

    }

