#include <OneWire.h>

#include <OneWire.h>
#include <DallasTemperature.h>
 
OneWire oneWire(10);  // порт подключения датчика (A1)
DallasTemperature ds(&oneWire);

void setup() {
  Serial.begin(9600);   // инициализация монитора порта
  ds.begin();                 // инициализация датчика ds18b20
}

void loop() {
  ds.requestTemperatures();                       // считываем температуру с датчика
  
  Serial.print(ds.getTempCByIndex(0));   // выводим температуру на монитор
  Serial.println("C");
}
