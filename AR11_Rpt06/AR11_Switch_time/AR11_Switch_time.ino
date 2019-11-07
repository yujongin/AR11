#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,16,2); 

const int inputPin = 2;
long startTime = 10;
long swCountTimer = 0;
unsigned time;

void setup(){
  pinMode(inputPin, INPUT_PULLUP);
  Serial.begin(9600);
  lcd.init(); // LCD 설정
  lcd.clear();
  lcd.backlight(); 
  lcd.begin(16,2);
  lcd.print("AR11 time check");
}

void loop(){
  if(digitalRead(inputPin) == LOW){
 
    startTime = millis();
    
    while(digitalRead(inputPin) == LOW);
    
    swCountTimer = millis() - startTime;

    Serial.print(swCountTimer);
    Serial.println(" ms");
    
    lcd.setCursor(0,1);
    lcd.print(swCountTimer);
    lcd.print(" ms");
  }
}
