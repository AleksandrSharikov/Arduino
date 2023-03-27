#include <iarduino_OLED_txt.h>                         // Подключаем библиотеку iarduino_OLED_txt.
iarduino_OLED_txt myOLED(0x78);
extern uint8_t SmallFontRus[];   // Подключаем шрифт SmallFontRus.
 int t;
void setup() {
  // put your setup code here, to run once:
myOLED.begin();                                    // Инициируем работу с дисплеем.
   myOLED.setFont(SmallFontRus);                          // Указываем шрифт который требуется использовать для вывода цифр и текста.
    myOLED.clrScr(); 
}

void loop() {
 
  t=analogRead(17);
 myOLED.print( t    ,   OLED_C, 0); 
 delay(2000);
 myOLED.clrScr();   
  // put your main code here, to run repeatedly:

}
