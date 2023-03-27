#include <max6675.h>
#include <iarduino_OLED_txt.h>                         // Подключаем библиотеку iarduino_OLED_txt.
iarduino_OLED_txt myOLED(0x78);
extern uint8_t SmallFontRus[];   
int d,DZ;// Подключаем шрифт SmallFontRus.
unsigned char encoder_A, encoder_B, encoder_A_prev;
//Переменная для отслеживания нажатий кнопки - 
//центральной оси энкодера
static bool SW_State = false;
float t1=10;
byte thermoDO = 12; //
byte thermoCS = 11;  //  termocouple pins
byte thermoCLK = 10;  //

void setup() {
  // put your setup code here, to run once:
   myOLED.begin();                                    // Инициируем работу с дисплеем.
    myOLED.setFont(SmallFontRus);                      // Указываем шрифт который требуется использовать для вывода цифр и текста.
//  myOLED.setCoding(TXT_UTF8);   
 myOLED.print( "температура"    ,      0, 0);
 // myOLED.print( "Заданное значение"     , OLED_C, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
myOLED.print( t1    ,      0, 3);
delay(1000);
t1=thermocouple.readCelsius();
delay(1000);
 myOLED.print( "8888888"    ,      0, 3);
}
