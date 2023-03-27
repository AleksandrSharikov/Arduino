//#include <MsTimer2.h>

#define MEASURE_PERIOD 500  // время периода измерения
#define R1  15.  // сопротивление резистора R1
#define R2  4.22 // сопротивление резистора R2

int timeCount;  // счетчик времени
//float u1, u2;   // измеренные напряжения

void setup() {
  Serial.begin(9600);  // инициализируем порт, скорость 9600
 // MsTimer2::set(1, timerInterupt); // прерывания по таймеру, период 1 мс 
  //MsTimer2::start();              // разрешение прерывания
}

void loop() {
  int u[7];
  int i;

  // период 500 мс 
  if ( timeCount >= MEASURE_PERIOD ) {
    timeCount= 0;

    // чтение кода канала 1 и пересчет в напряжение
    u[0]=  analogRead(A0);
     u[1]=  analogRead(A1);
      u[2]=  analogRead(A2);
       u[3]=  analogRead(A3);
        u[4]=  analogRead(A4);
         u[5]=  analogRead(A5);
          u[6]=  analogRead(A6);// чтение кода канала 2 и пересчет в напряжение
    
    // передача данных через последовательный порт
    for (i=0;6;i++)
    Serial.print(u[i]);            
  }  
}

// обработка прерывания 1 мс
void  timerInterupt() {
  timeCount++;
}
