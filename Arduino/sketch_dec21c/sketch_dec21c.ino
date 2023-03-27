#include <EEPROM.h>
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  byte a;
  byte i;
 // Serial.println('a');
a=Serial.read();
  for(i=0;i<40;i++)
  {//Serial.println('b');
    a=EEPROM.read(i);
  Serial.println(a);}
  delay(2000);
  
  // put your main code here, to run repeatedly:

}
