#include <max6675.h>
//#include "max6675.h"
#include <iarduino_OLED_txt.h>                         // Подключаем библиотеку iarduino_OLED_txt.
iarduino_OLED_txt myOLED(0x78);
extern uint8_t SmallFontRus[];   // Подключаем шрифт SmallFontRus.

//pins
byte TG=8;// gotov
byte TRPin=17;// termoresistor analog
byte UPin=5; // pit
byte KB=6; // kalibrovka
byte KL=7; // Light kalibr
byte thermoDO = 12; //
byte thermoCS = 11;  //  termocouple pins
byte thermoCLK = 10;  //
//byte vccPin = 9; // termocouple power
byte CLK=3;//valcoder
byte DT=4;
byte SW=2;


//constants
int Roff=300;// Max temp resistance
int T=5000;//full period
int Tms=30000;  // friqancy of mesurinh
long unsigned toe=10000; //timout encoder
byte add[4]={10,5,2,1};// add to power
byte PD=20;//Precission delat

//flags
byte chenge_fl=0; //changefl
byte tpon_fl=0;
byte reach_fl=0; //reaching Ru flag




byte sh=24;
//byte SK=50;// скважность
double TC,TP,TD; //time counters
double TS1,TS0; //time counters set
double Tmn, Tmp, Tmd; //mine cickle timer
float Tcheck;// thermocouple
int Rc; //courent R
unsigned char encoder_A, encoder_B, encoder_A_prev; // valcoder
unsigned char SW_State;
int Ru=300;// set R
byte D;// filling
int DR;// delta R
int Rcp,Rup;
byte CT; //cykle counter
byte pc=0; //Precission coeficent
int i_test;




void setup() {
   myOLED.begin();                                    // Инициируем работу с дисплеем.
    myOLED.setFont(SmallFontRus);                      // Указываем шрифт который требуется использовать для вывода цифр и текста.
    myOLED.clrScr(); 
     pinMode(UPin,OUTPUT); //Pitanie
  digitalWrite(UPin,LOW);
 // attachInterrupt(1, encoder, FALLING);
//countersset();

      myOLED.print( "oper"    ,   OLED_L, 2); 
      myOLED.print( "D" ,  OLED_C, 2); 
      myOLED.print( "add"    ,   OLED_R, 2); 
      
}

void loop() {
  Rc=analogRead(TRPin);
  if(Rc<Roff)
  {digitalWrite(UPin,LOW);
    myOLED.print("ПЕРЕГРЕВ",OLED_C,6);
    }
    else
    {
  encoder();
  mainloop();
    }

}



//=====================================================Functions==================================



void encoder()                                                                  //Encoder
 {
          
   //    myOLED.print( "              "    ,   OLED_C, 0); 
   //       myOLED.print( "void encoder()"    ,   OLED_C, 0);
         long unsigned te0;
         long unsigned te1;
         long unsigned ted;
         byte mfl=0;
          
 
  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
  SW_State = digitalRead(SW);

 //if(!encoder_A)
   {
    // myOLED.clrScr(); 
   // myOLED.print( "JN"    ,   OLED_C, 3);
   // delay(1000);
 te0=millis();
 te1=te0;
  encoder_A_prev = HIGH;
  //myOLED.print("VK",(wid+4),0);

 while(ted<toe)
 {
 
  /* myOLED.print( ted/100    ,   OLED_C, 6);
   myOLED.print( te0/100    ,   OLED_C, 5);
   myOLED.print( te1/100    ,   OLED_C, 4);*/


  encoder_A = digitalRead(CLK);
  encoder_B = digitalRead(DT);
/*  SW_State=digitalRead(2);
 myOLED.print(SW_State,OLED_L,6);
  //if(!SW_State) myOLED.print("SW=-",OLED_L,6);
   if(mfl==0) myOLED.print("mfl=0",OLED_C,6); 
   if(mfl!=0) myOLED.print("mfl!=0",OLED_C,6);*/
  te1=millis();
  ted=te1-te0;
  if(!encoder_A && encoder_A_prev)
  {
   
    if(encoder_B)
    {
      
      Ru=Ru-1;
      //if(SK<1)
     // SK=SK+2;
      myOLED.print( "      "    ,   OLED_L, 1); 
      myOLED.print( Ru    ,  OLED_L, 1); 
      te0=millis();
    }
       else
    {
      Ru=Ru+1;
      //if(SK>100)
     // SK=SK-2;
      
      myOLED.print( "     "    ,   OLED_L, 1); 
      myOLED.print( Ru    ,  OLED_L, 1); 
       te0=millis();
      }
  }
    encoder_A_prev = encoder_A;

  //Работаем с центральной осью энкодера - кнопкой
  //Этот кусок кода образует собой как бы перекидной триггер
  //Считываем значение пина 2 на плате Arduino
  //к которому подключен контакт SW энкодера
  //Если центральная ось нажата - то сигнал SW будет иметь низкий уровень
/*  if(!SW_State)
  if(mfl==0)
  {
    myOLED.print("+",OLED_R,6);
    //Если переменная SW_State установлена в false то установить её в true 
    //if(SW_State)             //if valcoder is pressed write temp to eeprom
    {
      //ts1=floor(TS);
     // ts2=(TS-ts1)*10;
      EEPROM.put(temset,TS);
     // EEPROM.write(temset+1,ts2);
    myOLED.print( "*" , (wid+1)+3, 0);
      te0=millis();
      mfl=1;
      //И запомнить состояние
     // SW_State = true;
    }
    //И наоборот - если переменная SW_State установлена в true, 
    //то сбросить её в false 
  
  } */
 } //cicle ends
   // countersset();
}     
 }        

void countersset(byte D)                                    // seting of counters
{
  if(D>100)
  D=100;
  if(D<0)
  D=0;
  TS1=T/100*D;
  TS0=T-TS1;
  //myOLED.print( TS1    ,  OLED_L, 1);
  //myOLED.print( TS0    ,  OLED_R, 1); 
 //  myOLED.print( T    ,  OLED_C, 2); 
  }
 

void mainloop()                                                           //main loop
{
Rc=analogRead(TRPin);
DR=abs(Rc-Ru);

      myOLED.print( "     "    ,   OLED_C, 1); 
      myOLED.print( Rc    ,  OLED_C, 1); 
      
      myOLED.print( "     "    ,   OLED_R, 1); 
      myOLED.print( DR    ,  OLED_R, 1); 
      
if(DR>PD)
reach_fl=0;
if(reach_fl==0)
rough();
else
accurate();

}

void rough()
{
  
      //myOLED.print( "      "    ,   OLED_L, 3); 
      myOLED.print( "rough   ",  OLED_L, 3); 
  if (Rc>Ru)
 { digitalWrite(UPin,HIGH);
  myOLED.print("+",OLED_L,0);
  myOLED.print("100",OLED_C,3);}
  else
 { digitalWrite(UPin,LOW);
 myOLED.print(" 0 ",OLED_C,3);
  myOLED.print("-",OLED_L,0);}
  if (Rc==Ru)
  reach_fl=1;
  }

                                                                              // accurate
void accurate()
{
  
     // myOLED.print( "      "    ,   OLED_L, 3); 
      myOLED.print( "accurate"    ,  OLED_L, 3); 
  if(reach_fl==1)
  {D=70;
  Tmp=Tmn;
  reach_fl=2;}
  Tmn=millis();
  Tmd=Tmn-Tmp;

  
                      if(Tmd>Tms)
                      {
                    if(Rc<Ru&&Rc<Rcp)
                    D=D-add[pc];
                    if(Rc>Ru&&Rc>Rcp)
                    D=D+add[pc];
                    if(pc<3)
                    {
                    if ((Rc<Ru&&Rcp>Ru)||(Rc>Ru&&Rcp<Ru))
                    pc++;
                    }
      myOLED.print( "     "    ,   OLED_C, 3); 
      myOLED.print( D    ,  OLED_C, 3); 
      myOLED.print( "     "    ,   OLED_R, 3); 
      myOLED.print( add[pc]    ,  OLED_R, 3); 
                    Tmp=Tmn;
                    i_test++;
      myOLED.print( "     "    ,   OLED_L, 4); 
      myOLED.print( i_test    ,   OLED_L, 4); 
    /*  myOLED.print(     ,  OLED_C, 4); 
      myOLED.print( "     "    ,   OLED_C, 4); 
      myOLED.print( add[pc]    ,  OLED_C, 4); */
                    
                    countersset(D);
                    }
     
cycle();
  
  Rcp=Rc;
 // Rup=Ru;
  }

  void cycle()
  {TC=millis();
  TD=TC-TP; 
  //myOLED.print(TD,OLED_C,3);
   MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
   
 //if(tpon_fl==0)
  if (TD>TS1)
  if (chenge_fl==0)
  {//myOLED.print("    ",OLED_C,1);
    digitalWrite(UPin,LOW);
    myOLED.print("-",OLED_L,0);
  chenge_fl=1;}
 
  if (TD>T)
  {TP=TC;
  Tcheck=thermocouple.readCelsius();
  Rc=analogRead(TRPin);
myOLED.print( Tcheck    ,  OLED_L, 4); 
//myOLED.print( Rc    ,  OLED_R, 2); 
//  myOLED.print("    ",OLED_C,2);
 // myOLED.print("    ",OLED_C,1);
  digitalWrite(UPin,HIGH);
 myOLED.print("+",OLED_L,0);
  chenge_fl=0;
  tpon_fl=0;
    }
      
  }
