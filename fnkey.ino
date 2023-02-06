#include <SoftwareSerial.h>
SoftwareSerial mySerial(8,9);

#include <LiquidCrystal.h>
LiquidCrystal lcd(6, 7, 5, 4, 3, 2);

 char res[130];

unsigned char enroll[12]={0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X03,0X01,0X00,0X05};   // ok
unsigned char generate_ch[13]={0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0x02,0X01,0X00,0X08};   //ok
unsigned char generate_ch1[13]={0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0x02,0X02,0X00,0X09};  //ok
unsigned char un_cmd[12]={0xef,0x01,0xff,0xff,0xff,0xff, 0x01,0x00,0x03,0x05,0x00,0x09 };
unsigned char store[12]={0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X06,0X06,0X02,0x00};     //ok
unsigned char identify[17]={0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x08,0x1b,0x01,0x00,0x00,0x01,0x01,0x00,0x27};                                                                       


int party1_count=0,party2_count=0,total_count=0;
int sts0=0,sts1=0,sts2=0,sts3=0;
char rcv;

char pastnumber[11];


void okcheck()
{
  unsigned char rcr;
  do{
      rcr = Serial.read();
    }while(rcr != 'K');
}

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
} 


int fpenroll(char);
int fpsearch();

int err =0;

int enrol_sw  = 11;
int identi_sw = 12;
int buzzer    = 13;
int relay     = 10;



int idk = 0,eid=0;


void beep()
{
  digitalWrite(buzzer, LOW);delay(3000);digitalWrite(buzzer, HIGH);
}
 
void setup() 
{  
  pinMode(enrol_sw, INPUT); 
  pinMode(identi_sw, INPUT); 
  pinMode(buzzer, OUTPUT); 
  pinMode(relay, OUTPUT); 


  digitalWrite(enrol_sw, HIGH);        
  digitalWrite(identi_sw, HIGH);
  digitalWrite(buzzer, HIGH);
  digitalWrite(relay, LOW);


  Serial.begin(9600);
  mySerial.begin(57600);

  lcd.begin(16,2);
  lcd.clear();lcd.print("FP Based Examination");
  
    delay(4000);  //delay(000);  
} 

void loop()
{ mn:
 //lcd.clear();lr  
  lcd.clear();lcd.print("Put Finger.....");
  
if(digitalRead(enrol_sw) == LOW)
{
  
  lcd.clear();lcd.setCursor(0, 0);lcd.print("ENROLLING..");
  if(fpenroll(eid) == -1)
  {
    //Serial.print("Enroll failed:");Serial.print(err);Serial.println("");
    err=0;
    lcd.clear();lcd.setCursor(0, 0);lcd.print("ENROLL FAILED");
  }
  else
  //if(eid >= 0 && eid <= 9)
  {
    lcd.clear();lcd.setCursor(0, 0);lcd.print("ENROLLED:");lcd.print((int)eid);
    //Serial.print("Enroll Success to id:");Serial.print((int)eid);Serial.println("");
    eid++;
  }
 
  delay(2000);
 // lcd.clear();lcd.setCursor(0, 0);lcd.print("SELECT OPTION");
}

if(digitalRead(identi_sw) == LOW)
{
  lcd.clear();lcd.setCursor(0, 0);lcd.print("Identifing..");
    idk = fpsearch();
      if(err == 1)
        {err=0;
         lcd.clear();lcd.print("Not Found...");
         digitalWrite(buzzer, LOW);delay(2000);delay(2000);digitalWrite(buzzer, HIGH);    
        }
      if(idk >= 0 && idk <= 9)
        { 
          /*
             lcd.clear();lcd.print("Correct Match");
             lcd.setCursor(0,1);lcd.print("Accessed");
          */
          if(idk == 0)
            {
             lcd.clear();lcd.print("H.No:0001"); 
             lcd.setCursor(0,1);lcd.print("Room_No:101");
             digitalWrite(relay, HIGH);
              delay(2000);   
             digitalWrite(relay, LOW);delay(500);  
            }
          if(idk == 1)
            {
             lcd.clear();lcd.print("H.No:0002"); 
             lcd.setCursor(0,1);lcd.print("Room_No:102");
             digitalWrite(relay, HIGH);
              delay(2000);   
             digitalWrite(relay, LOW);delay(500);  
            }
          if(idk == 2)
            {
             lcd.clear();lcd.print("H.No:0003"); 
             lcd.setCursor(0,1);lcd.print("Room_No:103");
             digitalWrite(relay, HIGH);
              delay(2000);   
             digitalWrite(relay, LOW);delay(500);  
            }
          if(idk == 3)
            {
             lcd.clear();lcd.print("H.No:0004"); 
             lcd.setCursor(0,1);lcd.print("Room_No:104");
             digitalWrite(relay, HIGH);
              delay(2000);   
             digitalWrite(relay, LOW);delay(500);  
            }      
             delay(3000);  //delay(000);  
        }

}//identify

delay(500);
}


int ct=0;
char dummy=0x0f;
int fpenroll(char id)
{
 
  serialFlush();

 //enroll buffer send 12 bytes
  for(int i =0;i<12;i++)
      mySerial.write(enroll[i]);
  res[9] = 1;//
  delay(1000);//wait some time to get replay from r305
  while(mySerial.available()){res[ct] = mySerial.read();ct++;}ct=0;
  if(res[9] == 0){}
  else{err=3;return -1;}

  //generate ch1 buffer
  for(int i =0;i<13;i++)
      mySerial.write(generate_ch1[i]);
  res[9] = 1;
  delay(1000);//wait some time to get replay from r305
  while(mySerial.available()){res[ct] = mySerial.read();ct++;}ct=0;
  if(res[9] == 0){}
  else{err=4;return -1;}

  //uncmd buffer send 12 bytes
  for(int i =0;i<12;i++)
      mySerial.write(un_cmd[i]);
  res[9] = 1;//
  delay(1000);//wait some time to get replay from r305
  while(mySerial.available()){res[ct] = mySerial.read();ct++;}ct=0;
  if(res[9] == 0){}
  else{err=5;return -1;}

  //store buffer send 12 bytes
  for(int i =0;i<12;i++)
      mySerial.write(store[i]);
  dummy = 0x0f+id;
  mySerial.write((uint8_t)id);
  mySerial.write((uint8_t)0x00);
  while(mySerial.available()){res[ct] = mySerial.read();ct++;}ct=0;
  if(res[9] == 0){return id;}
  else{err=6;return -1;}
}
int fpsearch()
{
  ct=0;
   serialFlush();

   //generate ch buffer
  for(int i =0;i<13;i++)
      mySerial.write(generate_ch[i]);
  res[9] = 1;
  delay(1000);//wait some time to get replay from r305
  while(mySerial.available()){res[ct] = mySerial.read();ct++;}ct=0;
  if(res[9] == 0){}
  else{err=2;return -1;}


  //enroll buffer send 12 bytes
  for(int i =0;i<17;i++)
      mySerial.write(identify[i]);
  res[9] = 1;//
  delay(1000);//wait some time to get replay from r305
  while(mySerial.available()){res[ct] = mySerial.read();ct++;}ct=0;
  if(res[9] == 0){return (int)res[11];}
  else{err=1;return -1;}

}




int readSerial(char result[]) 
{
  int i = 0;
  while (1) 
  {
    while (Serial.available() > 0) 
    {
      char inChar = Serial.read();
      if (inChar == '\n') 
         {
          Serial.flush();
          return 0;
         }
      if (inChar != '\r') 
         {
          result[i] = inChar;
          i++;
         }
    }
  }
}


void converts(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
    
     
   Serial.write(a);
   Serial.write(c);
   Serial.write(e); 
   Serial.write(g);
   Serial.write(h);
}

void convertl(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
    
     
   lcd.write(a);
   lcd.write(c);
   lcd.write(e); 
   lcd.write(g);
   lcd.write(h);
}
