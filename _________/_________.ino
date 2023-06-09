#include <iarduino_OLED_txt.h>                         // Подключаем библиотеку iarduino_OLED_txt.
iarduino_OLED_txt myOLED(0x78); // Объявляем объект myOLED, указывая адрес дисплея на шине I2C: 0x78 (если учитывать бит RW=0).
                                                       //
extern uint8_t SmallFontRus[];   
int d,DZ;// Подключаем шрифт SmallFontRus.
unsigned char encoder_A, encoder_B, encoder_A_prev;
//Переменная для отслеживания нажатий кнопки - 
//центральной оси энкодера
static bool SW_State = false;
                                                       //
void setup(){                                          //
    myOLED.begin();                                    // Инициируем работу с дисплеем.
    myOLED.setFont(SmallFontRus);                      // Указываем шрифт который требуется использовать для вывода цифр и текста.
//  myOLED.setCoding(TXT_UTF8);   
 myOLED.print( "текущее значение"    ,      0, 0);
  myOLED.print( "Заданное значение"     , OLED_C, 3);    // Выводим текст по центру 3 строки.// Выводим текст начиная с 0 столбца 0 строки.// Указываем кодировку текста в скетче. Если на дисплее не отображается Русский алфавит, то ...
}                                                      // раскомментируйте функцию setCoding и замените параметр TXT_UTF8, на TXT_CP866 или TXT_WIN1251.
                                                       //
void loop(){                                           //
   // myOLED.clrScr();                                   // Чистим экран.
    
   
   
   
  
   // myOLED.print( "РСТУФХЦЧШЩЪЫЬЭЮЯ"  , OLED_C, 6);    // Выводим текст по центру 6 строки.
    {
  //CLK подключаем к пину 3 на плате Arduino
  //DT  подключаем к пину 4 на плате Arduino
  //Считываем значения выходов энкодера
  //И сохраняем их в переменных
  encoder_A = digitalRead(3);
  encoder_B = digitalRead(4);

  //Если уровень сигнала А низкий,
  //и в предидущем цикле он был высокий 
  if(!encoder_A && encoder_A_prev)
  {
    //Если уровень сигнала В высокий
    if(encoder_B)
    {
      d=d+1;
       myOLED.print( "  ", OLED_C, 2);
       myOLED.print( d, OLED_C, 2);    // Выводим текст по центру 2 строки.
      myOLED.print( "+"  , OLED_C, 6);
      //Значит вращение происходит по часовой стрелке
      //Здесь можно вставить операцию инкремента
      //Здесь можно вставлять какие либо свои 
      //операции по обработке данных в нужном направлении
    }
    //Если уровень сигнала В низкий
    else
    {
      d=d-1;
      myOLED.print( "  ", OLED_C, 2);
       myOLED.print( d, OLED_C, 2);    // Выводим текст по центру 2 строки.
      myOLED.print( "-"  , OLED_C, 6);
      
      //Значит вращение происходит против часовой стрелки
      //Здесь можно вставить операцию декремента
      //Здесь можно вставлять какие либо свои 
      //операции по обработке данных в нужном направлении
    }
  }
  //Обязательно нужно сохранить состояние текущего уровня сигнала А
  //для использования этого значения в следующем цикле сканирования программы
  encoder_A_prev = encoder_A;

  //Работаем с центральной осью энкодера - кнопкой
  //Этот кусок кода образует собой как бы перекидной триггер
  //Считываем значение пина 2 на плате Arduino
  //к которому подключен контакт SW энкодера
  //Если центральная ось нажата - то сигнал SW будет иметь низкий уровень
  if(!digitalRead(2))
  {
    //Если переменная SW_State установлена в false то установить её в true 
    if(!SW_State)
    {
      DZ=d;
        myOLED.print( DZ , OLED_C, 5);    // Выводим текст по центру 5 строки.
      //И запомнить состояние
      SW_State = true;
    }
    //И наоборот - если переменная SW_State установлена в true, 
    //то сбросить её в false 
    else
    {
      //И запомнить состояние
      SW_State = false;
    }
  }
}
    //delay(3000);                                       // Ждём 3 секунды.
                                                       //
   
}          
