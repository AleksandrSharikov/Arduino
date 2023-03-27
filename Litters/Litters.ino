#include <iarduino_OLED_txt.h>                         // Подключаем библиотеку iarduino_OLED_txt.
iarduino_OLED_txt myOLED(0x3C);
extern uint8_t MediumFontRus[];   
//extern uint8_t SmallFontRus[];// Подключаем шрифт SmallFontRus.


//pins
byte Pow=2;
byte S=3;

//constants
float c=12.5;



// other
long unsigned T1,T2; //timer
unsigned int t[4];//time
byte i;// t index
float ta,talast;// averange time
float V;//litters
byte Si=1;//signal condition



void setup() {
  // put your setup code here, to run once:
    pinMode(Pow,OUTPUT);  //Power on
    digitalWrite(Pow,HIGH); 
    pinMode(S,INPUT);  //Intranse
    digitalWrite(S,LOW);

    myOLED.begin();                                  
    myOLED.clrScr(); 
    myOLED.setFont(MediumFontRus);
    myOLED.print("RASHODOMER",OLED_C,1);
    delay(1000);
    
}

void loop() {
  // put your main code here, to run repeatedly:

  T1=millis();
  while (Si==1)
  Si=digitalRead(S);
  while (Si==0)
  Si=digitalRead(S);
  T2=millis();  

  for(i=2;i>0;i=i-1)
  t[i]=t[i-1];
  t[0]=T2-T1;

  ta=(t[0]+t[1]+t[2])/3;

  if(ta!=talast)
  {
    V=1000/(ta*c);
    myOLED.print(V,OLED_C,3);
    }
  
}
