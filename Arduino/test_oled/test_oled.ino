//  подключаем библиотеку LiquidCrystal:
#include <LiquidCrystal.h>

// Создаем LCD объект. Выводы: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() 
{
  pinMode(15,OUTPUT);
  // устанавливаем количество столбцов и строк на дисплея:
  lcd.begin(16, 2);

  // Очищаем LCD дисплей 
  lcd.clear();
}

void loop() 
{
  // Печатаем сообщение на LCD.
lcd.print(25851256);

  // устанавливаем курсор на столбец 0, строка 1
  // (примечание: строка 1 - вторая строка, так как отсчет начинается с 0):
  lcd.setCursor(0, 1);
  // Печатаем сообщение на LCD.
  lcd.print(" LCD Tutorial");
  while(1<2)
  {digitalWrite(15,HIGH);
  delay(1000);
  digitalWrite(15,LOW);
  delay(500);
    }
}
