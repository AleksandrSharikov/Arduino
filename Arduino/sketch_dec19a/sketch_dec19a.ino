//float Step = 5.0F / 1024; // Вычисляем шаг Uопорн / на градацию 
int U,i;
int a=2000;
void setup() 
{ 
Serial.begin(9600); // Задаем скорость работы монитор порта 
} 

void loop() 
{ 
  Serial.flush();
  //Serial.println();
  Serial.print(a);
  for(i=0;i<6;i++)
  {
U = analogRead(i); // Задаем переменную analogValue для считывания показаний 
//float voltageValue = analogValue * Step; // Переводим в вольты (показание * шаг) 
//Serial.print(U,DEC); // Выводим значение в вольтах в порт 
  }
delay(500); // Ждем пол секунды 
}
