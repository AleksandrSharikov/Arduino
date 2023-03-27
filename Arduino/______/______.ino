#include <iarduino_OLED_txt.h>                         // Подключаем библиотеку iarduino_OLED_txt.
iarduino_OLED_txt myOLED(0x78);
extern uint8_t SmallFontRus[];   // Подключаем шрифт SmallFontRus.
void setup() {
  // put your setup code here, to run once:
   myOLED.begin();                                    // Инициируем работу с дисплеем.
    myOLED.setFont(SmallFontRus);                      // Указываем шрифт который требуется использовать для вывода цифр и текста.
    myOLED.clrScr(); 

}

void loop() {

  int a=digitalRead(2);
  myOLED.print(a,OLED_C,3);
  int b=digitalRead(3);
  myOLED.print(b,OLED_R,3);
  int c=digitalRead(4);
  myOLED.print(c,OLED_L,3);
 myOLED.print("jpopa",OLED_L,5); myOLED.print("Жопа",OLED_L,1);
  /* int a=digitalRead(2);
  myOLED.print(a,OLED_R,3);
   int a=digitalRead(2);
  myOLED.print(a,OLED_L,3);*/
  // put your main code here, to run repeatedly:

}
