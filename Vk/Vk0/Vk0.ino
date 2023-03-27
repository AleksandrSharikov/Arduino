

//pin

byte w_pib; // while pin
byte c_pin; // cadence pin
byte b1; //buttons
byte b2;
byte br;
byte cd_l,cd_g,cd_h; // cadence diods

//variables

float sp=0; //speed
byte cd=0; //cadence
byte cd0;
byte spd0,spd1;//displayable speed 
unsigned double ct0,ct1,st0,st1;// cadens and speed timers
int wl=2023; // wheel lenth
byte w=0;//wheel and cadens signals
byte c=0;
byte w_old=0,c_old=0;
unsigned double distance;

//flags
byte ride_flag=0;
byte cd_flag=0;
byte new_flag=0; //1=>new speed; 2=>new cadence; 3=> new cadence_flag
byte cd_led_flag=0;

//constants
int ride_delay=3000;
int cd_delay=3000;
byte lc=85;//low cadens
byte hc=110;//high cadens


void setup() {
  pinMode(cd_l,OUTPUT);
  digitalWrite(cd_l,LOW);
    pinMode(cd_g,OUTPUT);
  digitalWrite(cd_g,LOW);
    pinMode(cd_h,OUTPUT);
  digitalWrite(cd_h,LOW);

}

void loop() {
  while(ride_flag==1)
  {
    messure();
    st1=millis();
    if(w==1&&w_old==0)
    speed_clack();
    if(c==1&&c_old==0)
    cadens_calck();
    if(new_flag!=0)
    displey_ride();
    
    w_old=w;
    c_old=c;
    if(ride_flag==1)
    if(st1-st0-ride_delay<0)
    ride_flag=0;
    if(cd_flag==1)
    if(st1-cd0-cd_delay<0)
    {cd_flag=0;
    new_flag=2;}}

}


void messure()                                   //geting signals from gercons
{
  pinMode(w_pin,INPUT);
  pinMode(c_pin,OUTPUT);
  digitalWrite(c_pin,HIGH);
  w=digitalRead(w_pin);
  digitalWrite(c_pin,LOW);
  pinMode(c_pin,INPUT);
  pinMode(w_pin,OUTPUT);
  digitalWrite(w_pin,HIGH);
  c=digitalRead(c_pin);
  digitalWrite(w_pin,LOW);
  }

void speed_calck()//                                 Speed calculation
  {
  //  st1=millis();
    sp=wl*3.6/(st1-st0);
    spd1=round(sp*10);
              if(spd1!=spd0)
              {new_flag=1;
              spd0=spd1;}
    st0=st1;
    statistic();
    }

void cadence_calck()//                            Cadence calculation
  {
   // ct1=millis();
    cd=60000/(sp1-ct0);
   // spd1=round(sp*10);
              if(cd!=cd0)
              {new_flag=2;
              cd0=cd;}
    cd_flag=1;
    cd0=sp1;
    }

void displey_ride()//                              Displey in ride mode
{
      if(new_flag==2)
            cd_led();
  }

void cd_led();//                                  Cadence led switch
                {
                  if(cd_flag==0&&cd_led_flag!=0)
                {digitalWrite(cd_l,LOW);
                digitalWrite(cd_g,LOW);
                digitalWrite(cd_h,LOW);
                cd_led_flag=0;}
                 
                 if(cd_flag==1)
                 {if(cd<lc)
                 {
                  if(cd_led_flag==0)
                 {digitalWrite(cd_l,HIGH);
                 cd_led_flag=1;}
                 if(cd_led_flag==2)
                 {digitalWrite(cd_l,HIGH);
                 digitalWrite(cd_g,LOW);
                 cd_led_flag=1;}
                 if(cd_led_flag==3)
                 {digitalWrite(cd_l,HIGH);
                 digitalWrite(cd_h,LOW);
                 cd_led_flag=1;}                 
                 }
                 if(cd>hc)
                 {
                 if(cd_led_flag==0)
                 {digitalWrite(cd_h,HIGH);
                 cd_led_flag=3;}
                 if(cd_led_flag==2)
                 {digitalWrite(cd_h,HIGH);
                 digitalWrite(cd_g,LOW);
                 cd_led_flag=3;}
                 if(cd_led_flag==1)
                 {digitalWrite(cd_h,HIGH);
                 digitalWrite(cd_l,LOW);
                 cd_led_flag=3;}                 
                 }
                 if(cd<=hc&&cd>=lc)
                 {
                 if(cd_led_flag==0)
                 {digitalWrite(cd_g,HIGH);
                 cd_led_flag=2;}
                 if(cd_led_flag==1)
                 {digitalWrite(cd_g,HIGH);
                 digitalWrite(cd_l,LOW);
                 cd_led_flag=2;}
                 if(cd_led_flag==3)
                 {digitalWrite(cd_g,HIGH);
                 digitalWrite(cd_h,LOW);
                 cd_led_flag=2;}                 
                 }
                  }
                
                }

void statistic()//                                Statistic calculations
{    
  if(w==1&&w_old==0)
  distance=distance+wl;
}
