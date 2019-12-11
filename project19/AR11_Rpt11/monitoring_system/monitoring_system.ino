#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
const byte ROWS = 4;    // 행(rows) 개수
const byte COLS = 4;    // 열(columns) 개수
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
#define SS_PIN 53
#define RST_PIN 49
const int trigPin = 10;
const int echoPin = 11;
const int CdSPin = 0;
const int led = 31;

int wrong = 0;
int wrong2 = 0;
int counter = 0;
int counter2 = 0;
int turnOn = 0;
int detected = 0;
int pause = 0;
byte rowPins[ROWS] = {6,7,8,9};   // R1, R2, R3, R4 단자가 연결된 아두이노 핀 번호
byte colPins[COLS] = {5,4,3,2};   // C1, C2, C3, C4 단자가 연결된 아두이노 핀 번호
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
 
MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class
byte password[4]={50,52,54,56};
byte password2[4]={0,0,0,0};
byte rfid[8] = {0,0,0,0,0,0,0,0};
byte rfid2[8] = {244,189,86,211};
int rfidIsOk = 0;
int count = 0;
int switchOk = 0;
int back = 0;
long duration;
int distance;
boolean Lock = 0;
boolean ledCheck = 0;
void setup() {
   Serial.begin(9600);
   lcd.init(); // LCD 설정
   lcd.clear();      // LCD를 모두 지운다.
   lcd.backlight();  // 백라이트를 켠다.  
   lcd.setCursor(0,0);
   lcd.print("press password");
   delay(3000);
   lcd.setCursor(0,1);
   lcd.print("or put Cardkey ");

   pinMode(led,OUTPUT);
   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT); 

   SPI.begin();       
   mfrc522.PCD_Init();
   Serial.println("RFID reading UID");
}
void loop() {
  
if ( mfrc522.PICC_IsNewCardPresent())
    {
        if ( mfrc522.PICC_ReadCardSerial())
        {
           Serial.print("Tag UID:");
           for (byte i = 0; i < mfrc522.uid.size; i++) {
                  Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                  Serial.print(mfrc522.uid.uidByte[i], HEX);
            }
            for(byte i = 0; i<8; i++)
              {
                rfid[i]=0;
              }
            for(byte i = 0; i<mfrc522.uid.size; i++)
              {
                  rfid[i] = mfrc522.uid.uidByte[i];
              }
            
              rfidIsOk=0;
             for(byte i = 0; i<mfrc522.uid.size; i++)
             { 
              if(rfid[i] == rfid2[i])
              {
                rfidIsOk++;
                pause = 0;
              }
              else
              {
                 wrong++;
                 pause = 0;
              }
             }
             if(wrong>0){
               lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Wrong Cardkey");
                delay(3000);
                lcd.clear();
                lcd.print("press password");
                lcd.setCursor(0,1);
                lcd.print("or put Cardkey");
                wrong=0;
             }
             if(rfidIsOk==4){
              if(Lock==0)
              {
                Lock=1;
                lcd.clear();
                lcd.backlight();  
                lcd.setCursor(0,0);
                lcd.print("Start Monitoring");
                delay(3000);
                lcd.clear();  
              }
              else
              {
              Lock=0;
              lcd.clear();
              lcd.backlight(); 
              lcd.setCursor(0,0);
              lcd.print("Stop Monitoring");
              delay(5000);
              lcd.clear();
              lcd.print("press password");
              lcd.setCursor(0,1);
              lcd.print("or put Cardkey");   
              }
           
             }
             Serial.println();
            Serial.print(Lock);
            
              Serial.println();
            Serial.print(rfidIsOk);
            Serial.println();
            mfrc522.PICC_HaltA();
        }
}
char key = keypad.getKey();
        if(key){
                  pause = 1;
                  lcd.backlight();
                  Serial.println(key);
                  password2[count]=key;
                  lcd.clear();
                  lcd.setCursor(0,0);
                  lcd.print(key);
                  count++;  
               }
               if(key=='A')
               {
                 lcd.backlight();
                 lcd.setCursor(0,0);
                 lcd.print("detected:");
                 lcd.setCursor(0,1);
                 lcd.print(detected);
                 lcd.print("times");
                 count =0;
                 delay(3000);
                 lcd.clear();
                  if(Lock==0)
                  { 
                     lcd.print("press password");
                     lcd.setCursor(0,1);
                     lcd.print("or put Cardkey");
                  }
               }
               if(key=='B')
               {
                 lcd.backlight();
                 lcd.setCursor(0,0);
                 lcd.print("Turned on:");
                 lcd.setCursor(0,1);
                 lcd.print(turnOn);
                 lcd.print("times");
                  delay(3000);
                  lcd.clear();
                  if(Lock==0)
                  { 
                     lcd.print("press password");
                     lcd.setCursor(0,1);
                     lcd.print("or put Cardkey");
                  }
                 count = 0;
               }
                if(key=='C')
               {
                 if(ledCheck==0)
                 {
                   digitalWrite(led,HIGH);
                   ledCheck = 1;
                   lcd.clear();
                   lcd.setCursor(0,0);
                   lcd.print("led on");
                    delay(3000);
                    lcd.clear();
                     if(Lock==0)
                  {
                     lcd.print("press password");
                     lcd.setCursor(0,1);
                     lcd.print("or put Cardkey");
                  }
                 }
                 else
                 {
                   digitalWrite(led,LOW);
                   ledCheck = 0;
                   lcd.clear();
                   lcd.setCursor(0,0);
                   lcd.print("led off");
                    delay(3000);
                    lcd.clear();
                     if(Lock==0)
                  {
                     lcd.print("press password");
                     lcd.setCursor(0,1);
                     lcd.print("or put Cardkey");
                  }
                 }
                 count = 0;
               }
               if(key=='D')
               {
                if(back==0){
                  lcd.noBacklight();
                  back = 1;                  
                }
                else{
                  lcd.backlight();
                  lcd.clear();
                  lcd.print("backlight on");
                  delay(3000);
                  lcd.clear();
                  back = 0;
                  if(Lock==0)
                  {
                     lcd.print("press password");
                     lcd.setCursor(0,1);
                     lcd.print("or put Cardkey");
                  }
                }
                 count = 0;
               }
               if(count==4)
               { 
                   switchOk=0;
                   for(int i =0; i<4; i++)
                   {
                      if(password2[i] == password[i])
                      { 
                         switchOk++;
                         pause = 0;     
                      }
                      else
                      {
                         wrong2++;
                         pause = 0;
                      }
                   }
                   if(wrong2>0){
               lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Wrong password");
                delay(3000);
                lcd.clear();
                lcd.print("press password");
                lcd.setCursor(0,1);
                lcd.print("or put Cardkey");
                wrong2=0;
             }
                   if(switchOk==4)
                   {
                        if(Lock==0)
                        {
                           Lock=1;
                            lcd.clear();
                            lcd.backlight();  
                            lcd.setCursor(0,0);
                            lcd.print("Start Monitoring");
                            delay(3000);
                            lcd.clear();                          
                        }
                         else
                         {
                             Lock=0;
                             lcd.clear(); 
                             lcd.backlight(); 
                             lcd.setCursor(0,0);
                             lcd.print("Stop Monitoring"); 
                             delay(5000);
                             lcd.clear(); 
                             lcd.print("press password");
                             lcd.setCursor(0,1);
                             lcd.print("or put Cardkey");     
                         }
                   }
                   count = 0;
                 Serial.print(Lock);
                 Serial.println();
                 Serial.print(switchOk);
                 Serial.println();
                }
       
      if(Lock==1)
      {
        if(pause==0)
        {   
          digitalWrite(trigPin, LOW);
         delayMicroseconds(2); 
         digitalWrite(trigPin, HIGH);
         delayMicroseconds(10);
         digitalWrite(trigPin, LOW);

         duration = pulseIn(echoPin, HIGH);

         distance= duration*0.034/2;

         lcd.setCursor(0,1);
         Serial.print("Distance: ");
         Serial.println(distance); 
         delay(500);
         if(distance<100)
         {
           counter++;
           
         }
         detected = counter;
        }
      }
      else
      {
        counter = 0;
      }
    if(Lock==1)
    {   
      if(pause==0)
      {
         lcd.setCursor(0,1);
         lcd.clear();
         
      int illuminance; // 현재의 밝기. 0~100%   
      int adcValue; // 실제 센서로부터 읽은 값 (0~1023)

      // CdS cell을 통하여 입력되는 전압을 읽는다.
      adcValue = analogRead(CdSPin);
      // 아날로그 입력 값을 0~100의 범위로 변경한다.
      illuminance = map(adcValue, 0, 1023, 100, 0);
      Serial.print("illuninance: ");
      Serial.println(illuminance);
      if(illuminance>50)
      {
        counter2++;      
      }
      turnOn=counter2;
      
    }
   }
   else
   {
     counter2 = 0;    
   }      
}
