#include <BigCrystal.h>
#include <BigFont.h>

#include <Wire.h>  // подключаем библиотеку Wire
#include <LiquidCrystal_I2C.h>  // подключаем библиотеку ЖКИ

#define printByte(args) write(args); //

uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0}; // битовая маска символа «сердце»

LiquidCrystal_I2C lcd(0x27, 16, 2); // Задаём адрес 0x27 для LCD дисплея 16x2

void setup() {
  lcd.init();  // инициализация ЖК дисплея
 lcd.backlight();  // включение подсветки дисплея
 // lcd.createChar(3, heart);  // создаём символ «сердце» в 3 ячейке памяти
  lcd.home();  // ставим курсор в левый верхний угол, в позицию (0,0)
  
  lcd.print("Hello SolTau.ru!");  // печатаем строку текста
  lcd.setCursor(0, 1);  // перевод курсора на строку 2, символ 1
  //lcd.print(" i ");  // печатаем сообщение на строке 2
 // lcd.printByte(3); // печатаем символ «сердце», находящийся в 3-ей ячейке
  lcd.print(" Arduino ");
}

void loop() { // мигание последнего символа
  lcd.setCursor(13, 1);   // перевод курсора на строку 2, символ 1
  lcd.print("\t");
  delay(500);             
  lcd.setCursor(13, 1);   // перевод курсора на строку 2, символ 1
  lcd.print(" ");
  delay(500);
}
