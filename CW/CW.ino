int b1=2; //dash pin
int b2=3; //dot pin
int o=7; //output pin заменить на 7 при травке(было 4) заменить на 6
int s=6; //sound pin заменить на 5
byte reo=2; // reostat pin in cercit 0

byte k1=0; //button1 flag
byte k2=0; //button2 flag
byte sf=0; //spase flag
byte nf=0; //next flag

float fc=500; //reostat
//byte simb; //simbol

int fr; //sound friquancy
int dotL; //dot lenth
int dashL; //dash lenth
unsigned long t2=0;
unsigned long t1=0;
unsigned long dt;

byte simb[5]={0,0,0,0,0};
byte leti=0;
char let;


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(b1,INPUT);
pinMode(b2,INPUT);
pinMode(o,OUTPUT);
}

void loop() {
  noTone(s);
  digitalWrite(o,LOW);
  // put your main code here, to run repeatedly:
  fr=1200; //tow friguency
  fc=analogRead(reo);
  dotL=(40+150*fc/1000); //dot lenth
  dashL=dotL*3;
  k1=digitalRead(b1);
  k2=digitalRead(b2);
//  Serial.print(dotL);
//  Serial.print('\n');
if(k1==HIGH && k2==LOW) //dash
dash();
if(k1==LOW && k2==HIGH) //dot
dot();
if(k1==HIGH && k2==HIGH) //-.
yamb();
t2=millis();
dt=t2-t1;
if(dt>3*dotL&&nf==0)
next();
if(dt>7*dotL&&sf==0)
spase();



}

    void dash()
{
tone(s,fr);
digitalWrite(o,HIGH);
delay(dashL);
if(leti<5)
simb[leti]=2;
  noTone(s);
  digitalWrite(o,LOW);
//  Serial.print("-");
delay(dotL);
t1=millis();
leti=leti+1;
//Serial.print(leti);
sf=0;
nf=0;
}

    void dot()
{
tone(s,fr);
digitalWrite(o,HIGH);
delay(dotL);
if(leti<5)
simb[leti]=1;
  noTone(s);
  digitalWrite(o,LOW);
//Serial.print(".");
delay(dotL);
t1=millis();
leti=leti+1;
sf=0;
nf=0;
}

   void yamb()
{
tone(s,fr);
digitalWrite(o,HIGH);
delay(dashL);

  noTone(s);
    digitalWrite(o,LOW);
  delay(dotL);

tone(s,fr);
digitalWrite(o,HIGH);
delay(dotL);
  noTone(s);
  digitalWrite(o,LOW);
  delay(dotL);
 // Serial.print("-.");
  t1=millis();
  sf=0;
  nf=0;
  if(leti<4)
  {
simb[leti]=2;
simb[leti+1]=1;
leti=leti+2;
  }
}


void spase()
{
 Serial.print(" ");
 //simb=0;
 sf=1; 
  }

  void next()
  {
    byte flag;
  //  Serial.print("|");
    leti=0;
    nf=1;
    flag=leter();
    if(flag==1)
    Serial.print(let);
    else
   { Serial.print("!!mis!!");
    Serial.print(let);}
   int i;
  for(i=0;i<5;i++)
   simb[i]=0;
    }
    byte leter()        //determination
    {
      byte i;
      byte flag=0;
      
  //   Serial.print("!");
   //   for(i=0;i<5;i++)
   //   Serial.print(simb[i]);
      
      if(simb[0]==1)                    //[1xxxx]
        {
          if(simb[1]==0)
          {let='e';flag=1;}
          if(simb[1]==1)                   //[11xxx]
            {
              if(simb[2]==0)
              {let='i';flag=1;}
              if(simb[2]==1)              //[111xx]
                {
                  if(simb[3]==0)
                  {let='s';flag=1;}
                  if(simb[3]==1)
                      {
                        if(simb[4]==0)
                        {let='h';flag=1;}
                        if(simb[4]==1)
                        {let='5';flag=1;}
                        if(simb[4]==2)
                        {let='4';flag=1;}
                        }
                    if(simb[3]==2)
                        {
                        if(simb[4]==0)
                        {let='v';flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {let='3';flag=1;}
                          }
                }
                          if(simb[2]==2)      //[112XX]
                {
                  if(simb[3]==0)
                  {let='u';flag=1;}
                  if(simb[3]==1)
                      {
                        if(simb[4]==0)
                        {let='f';flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {flag=0;}
                        }
                    if(simb[3]==2)
                        {
                        if(simb[4]==0)
                        {flag=0;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {let='2';flag=1;}
                          }
                     }


             
              }
          
                    if(simb[1]==2)                   //[12xxx]
            {
              if(simb[2]==0)
              {let='a';flag=1;}
              if(simb[2]==1)              //[121xx]
                {
                  if(simb[3]==0)
                  {let='r';flag=1;}
                  if(simb[3]==1)
                      {
                        if(simb[4]==0)
                        {let='l';flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {flag=0;}
                        }
                    if(simb[3]==2)
                     /*   {
                        if(simb[4]==0)
                        {let='v';flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {let='3';flag=1;}
                          } */
                          flag=0;
                }
                          if(simb[2]==2)      //[122XX]
                {
                  if(simb[3]==0)
                  {let='w';flag=1;}
                  if(simb[3]==1)
                      {
                        if(simb[4]==0)
                        {let='p';flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {flag=0;}
                        }
                    if(simb[3]==2)
                        {
                        if(simb[4]==0)
                        {let='j'; flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {let='1';flag=1;}
                          }
                     }


             
              
          }
      }

            if(simb[0]==2)                    //[2xxxx]
        {
          if(simb[1]==0)
          {let='t';flag=1;}
          if(simb[1]==1)                   //[21xxx]
            {
              if(simb[2]==0)
              {let='n';flag=1;}
              if(simb[2]==1)              //[211xx]
                {
                  if(simb[3]==0)
                  {let='d';flag=1;}
                  if(simb[3]==1)
                      {
                        if(simb[4]==0)    //[2111x]
                        {let='b';flag=1;}
                        if(simb[4]==1)
                        {let='6';flag=1;}
                        if(simb[4]==2)
                        {flag=0;}
                        }
                    if(simb[3]==2)       //[2112]
                        {
                        if(simb[4]==0)
                        {let='x';flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {flag=0;}
                          }
                }
                          if(simb[2]==2)      //[212XX]
                {
                  if(simb[3]==0)
                  {let='k';flag=1;}
                  if(simb[3]==1)
                      {
                        if(simb[4]==0)    //[2121x]
                        {let='c';flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {flag=0;}
                        }
                    if(simb[3]==2)     //[2122x]
                        {
                        if(simb[4]==0)
                        {let='y';flag=1;}
                        if(simb[4]==1)
                        {flag=0;}
                        if(simb[4]==2)
                        {flag=0;}
                          }
                     }


             
              }
          
                    if(simb[1]==2)                   //[22xxx]
            {
              if(simb[2]==0)
              {let='m';flag=1;}
              if(simb[2]==1)              //[221xx]
                {
                  if(simb[3]==0)
                  {let='g';flag=1;}
                  if(simb[3]==1)    //[2211x]
                      {
                        if(simb[4]==0)
                        {let='z';flag=1;}
                        if(simb[4]==1)
                        {let='7';flag=1;}
                        if(simb[4]==2)
                        {flag=0;}
                        }
                    if(simb[3]==2) //[2212x]
                       {
                        if(simb[4]==0)
                       {let='q';flag=1;}
                        if(simb[4]==1)
                       {flag=0;}
                        if(simb[4]==2)
                      {flag=0;}
                          } 
                }
                

                          if(simb[2]==2)      //[222XX]
                {
                  if(simb[3]==0)
                  {let='o';flag=1;}
                     if(simb[3]==1)              //[2221x]
                      {
                        if(simb[4]==0)
                        {flag=0;}
                        if(simb[4]==1)
                        {let='8';flag=1;}
                        if(simb[4]==2)
                        {flag=0;}
                        }
                    if(simb[3]==2)         //[2222x]
                        {
                        if(simb[4]==0)
                        {flag=0;}
                        if(simb[4]==1)
                        {let='9';flag=1;}
                        if(simb[4]==2)
                        {let='0';flag=1;}
                          }
                     }


             
              }
        }
     
      return(flag);
    }
