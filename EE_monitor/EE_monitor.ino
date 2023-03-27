#include <EEPROM.h>

struct call{byte t; int r;};    //srtucture calibr
call k[100];
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  EEPROM.get(30,k);
  int i=0;
  while(i<101)
  {
    Serial.print(i);
    Serial.print('\t');
    Serial.print(k[i].t);
    Serial.print('\t');
    Serial.print(k[i].r);
    Serial.print('\n');
    i++;
    }
while(1<2)
{}
}
