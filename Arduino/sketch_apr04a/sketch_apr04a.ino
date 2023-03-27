#include <iarduino_OLED_txt.h>                         // Подключаем библиотеку iarduino_OLED_txt.
iarduino_OLED_txt myOLED(0x78);
extern uint8_t SmallFontRus[];   // Подключаем шрифт SmallFontRus.
extern uint8_t MediumFontRus[];   // Подключаем шрифт SmallFontRus.


void setup() {
  myOLED.begin();                                    // Инициируем работу с дисплеем.
              
    myOLED.clrScr(); 
    
    myOLED.setFont(SmallFontRus);    
myOLED.print("1111",OLED_L,0);  
myOLED.print("2222",OLED_C,0);  
myOLED.print("3333",OLED_R,0);  
myOLED.print("4444",OLED_L,1);  
myOLED.print("5555",OLED_C,1);  
myOLED.print("6666",OLED_R,1);  
 myOLED.setFont(MediumFontRus);    
myOLED.print("7777",OLED_L, 4);  
myOLED.print("8888",OLED_R, 4);  

 myOLED.setFont(SmallFontRus);      
myOLED.print("1111",OLED_L,6);  
myOLED.print("2222",OLED_C,6);  
myOLED.print("3333",OLED_R,6);  
myOLED.print("4444",OLED_L,7);  
myOLED.print("5555",OLED_C,7);  
myOLED.print("6666",OLED_R,7);  
 
 


          
}

void loop() {
  // put your main code here, to run repeatedly:

}
