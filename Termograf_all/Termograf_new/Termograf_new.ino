
#include <EEPROM.h>
//#include <max6675.h>
#include <OneWire.h>

char com; //comand
byte thermoDO = 10; //
byte thermoCS = 11;  //  termocouple pins
byte thermoCLK = 12;  //
int vccPin = 9; // termocouple power
int t;//temproary
int ton=0;//logging
int tof=5;//stop logging
int tex=0;//exremum temp


byte i=1; // record number
byte recfl=0;
struct dat{byte i;byte io; byte tem; int tim;};
dat d;
//dat d1;
byte afl=0;
byte ld=19;   //light diod

OneWire ds(7); // Создаем объект OneWire для шины 1-Wire, с помощью которого будет осуществляться работа с датчиком


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
//i=findi();
pinMode(ld,OUTPUT);
digitalWrite(ld,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
//serialEvent();
//pinMode(ld,OUTPUT);
//digitalWrite(ld,HIGH);
}



           //FUNCTIONS


void serialEvent()        // Interrapion port
{com=Serial.read();
if (com=='s')               //select
start();
if (com=='r')
res();
if (com=='l')
reading();}






void start()                                              // start logging
{
/*  MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
  
pinMode(vccPin,OUTPUT);                                   //termocouple on
digitalWrite(vccPin,HIGH);

delay(10000);*/
 digitalWrite(ld,LOW);
  while (1<2)                                             //mesuring loop
   {serialEvent();
t=gettemp();
if (t<ton)
//if (recfl==0)
minus();
if (recfl==1)
if (t>tof)
plus();
       //         Serial.println("nothing");
       //         Serial.println(t);
    delay(30000);
    }}






void res(){              // reset
for(int a=0;a<1024;a++)
{EEPROM.write(a,0);
Serial.println(a);}
serialEvent();
}



void reading()             //reading
{
  byte ir=0;
  int j;
  byte tmp;
  dat d1;
  dat d2;
  Serial.print("activation = ");
  tmp=EEPROM.read(0);
  Serial.println(tmp);
  Serial.print("error flag = ");
  tmp=EEPROM.read(2);
  Serial.println(tmp);
//d1.i=256;
do

  {
    j=10*ir+10;
    d1.i=EEPROM.read(j);
    if(d1.i!=0){
    d1.io=EEPROM.read(j+1);
    d1.tem=EEPROM.read(j+2);
    *((byte*)(&d1.tim))=EEPROM.read(j+3);
    *((byte*)(&d1.tim)+1)=EEPROM.read(j+4);

    d2.i=EEPROM.read(j+5);
    d2.io=EEPROM.read(j+6);
    d2.tem=EEPROM.read(j+7);
    *((byte*)(&d2.tim))=EEPROM.read(j+8);
    *((byte*)(&d2.tim)+1)=EEPROM.read(j+9);

    Serial.print(d1.i);
    Serial.print(") time=");
    Serial.print(d1.tim);
    Serial.print(" minute; ");
    Serial.print("duration=");
    Serial.print(d2.tim-d1.tim);
    Serial.print(" minute; ");
    Serial.print(" min temperature=");
    Serial.print(d2.tem-100);
    Serial.println(" degrees");
    Serial.println(' ');
    
    
    ir=ir+1;
    }
    }while (d1.i!=0);
 com=Serial.read();
    }


void minus()                        //minus record
{
  int j=0;
  int tstor=0;
  byte tim1;
  byte tim2;
  unsigned long tim=0; //time
  byte aflt;         //temprorary aflag

 // Serial.println("minus");
  tim=millis();
  tstor=tim/60000;
  if (t<tex)
  tex=t;
  if (recfl==0)
              { d.i=i;               //Record
                d.io=1;  
                d.tem=0;
                d.tim=tstor;
                Serial.println(d.i);
                Serial.println(d.io);
                Serial.println(d.tem);
                Serial.println(d.tim);
                Serial.println(tex);
               

                j=10+10*(i-1);
                EEPROM.write(j,d.i);                
                EEPROM.write(j+1,d.io);
                EEPROM.write(j+2,d.tem);
                tim1=*((byte*)(&d.tim));
                tim2=*((byte*)(&d.tim)+1);
                EEPROM.write(j+3,tim1);
                EEPROM.write(j+4,tim2);
                recfl=1;
                digitalWrite(ld,HIGH);
                aflt=EEPROM.read(0);
                if (aflt==0)
                EEPROM.write(0,1);
                }
                
  
  }
void plus()                         //off record
{
  int j=0;
  int tstor=0;
  byte tim1;
  byte tim2;
  unsigned long tim=0; //time
  tim=millis();
  tstor=tim/60000;
  //Serial.println("plus");
  //if (t<tex)
  //tex=t;
  
  if (recfl==1)
              { d.i=i;               //Record
                d.io=1;  
                d.tem=tex+100;
                d.tim=tstor;

                j=15+10*(i-1);
                EEPROM.write(j,d.i);                
                EEPROM.write(j+1,d.io);
                EEPROM.write(j+2,d.tem);
                tim1=*((byte*)(&d.tim));
                tim2=*((byte*)(&d.tim)+1);
                EEPROM.write(j+3,tim1);
                EEPROM.write(j+4,tim2);
                }
                else EEPROM.write(2,1);
         recfl=0;
         digitalWrite(ld,LOW);
         i=i+1;
  }

byte findi ()                        //find i
{
  int j=10;
  byte reti=0;
  byte a;
  do
  {
    a=EEPROM.read(j);
    j=j+1;
    }while (a!=0);
if (j>11)
    {
    reti=((j-10)/10);
    a=EEPROM.read(j-5);           //test
    if (a!=j)
    EEPROM.write(2,1);
    a=EEPROM.read(j-10);           
    if (a!=j)
    EEPROM.write(2,1);
    return(reti+1);
    }
if(j<12)
    return(1);
  }


  int gettemp()
  {
    
  // Определяем температуру от датчика DS18b20
  byte data[2]; // Место для значения температуры
   
  ds.reset(); // Начинаем взаимодействие со сброса всех предыдущих команд и параметров
  ds.write(0xCC); // Даем датчику DS18b20 команду пропустить поиск по адресу. В нашем случае только одно устрйоство 
  ds.write(0x44); // Даем датчику DS18b20 команду измерить температуру. Само значение температуры мы еще не получаем - датчик его положит во внутреннюю память
   
  delay(1000); // Микросхема измеряет температуру, а мы ждем.  
   
  ds.reset(); // Теперь готовимся получить значение измеренной температуры
  ds.write(0xCC); 
  ds.write(0xBE); // Просим передать нам значение регистров со значением температуры
 
  // Получаем и считываем ответ
  data[0] = ds.read(); // Читаем младший байт значения температуры
  data[1] = ds.read(); // А теперь старший
 
  // Формируем итоговое значение: 
  //    - сперва "склеиваем" значение, 
  //    - затем умножаем его на коэффициент, соответсвующий разрешающей способности (для 12 бит по умолчанию - это 0,0625)
  int temperature =  ((data[1] << 8) | data[0]) * 0.0625;
   return(temperature);
  // Выводим полученное значение температуры в монитор порта
  //Serial.println(temperature);
    
}
