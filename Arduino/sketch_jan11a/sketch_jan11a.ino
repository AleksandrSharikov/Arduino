// программа измерения напряжения
// на аналоговых входах A0 и A1

//#include <MsTimer2.h>

#define MEASURE_PERIOD 500  // время периода измерения
//#define R1  15.  // сопротивление резистора R1
//#define R2  4.22 // сопротивление резистора R2

//int timeCount;  // счетчик времени
int u1, u2;   // измеренные напряжения

void setup() {
  Serial.begin(9600);  // инициализируем порт, скорость 9600
 // MsTimer2::set(1, timerInterupt); // прерывания по таймеру, период 1 мс 
 // MsTimer2::start();              // разрешение прерывания
}

void loop() {

  // период 500 мс 
 // if ( timeCount >= MEASURE_PERIOD ) {
   // timeCount= 0;

    // чтение кода канала 1 и пересчет в напряжение
    u1=  ((int)analogRead(A0));
    // чтение кода канала 2 и пересчет в напряжение
    u2=  ((int)analogRead(A1));

    // передача данных через последовательный порт
    Serial.print("U1 = ");     Serial.print(u1);
    Serial.print("   U2 = ");  Serial.println(u2);     
    delay(500);     
  }  


// обработка прерывания 1 мс
//void  timerInterupt() {
 // timeCount++;

