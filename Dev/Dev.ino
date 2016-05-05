// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>


#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   //#define Serial SerialUSB
#endif

//alarm button settings
const int incrTimePin = 3;
const int decrTimePin = 4;
int incrTime = 0;
int decrTime = 0;

//alarm time and date
int alarmHour = 20;
int alarmMinute = 45;

//LED initialization
int pwmTime = 0;
int steps = 64;
int sunrisespeed = 4500;
int i;
int j;
int pulsepin = 2;
/*
int delays[64] = {1,2,4,6,9,12,16,20,25,30,36,
42,49,56,64,72,81,90,100,110,121,132,
144,156,169,182,196,210,225,240,256,272,289,
306,324,342,361,380,400,420,441,462,484,506,
529,552,576,600,625,650,676,702,729,756,784,
812,841,870,900,930,961,992,992,992}; */

int delays[64] = {1,2,4,6,9,12,16,20,25,30,36,
42,49,56,64,72,81,90,100,110,121,132,
144,156,169,182,196,210,240,260,280,300,320,
340,360,380,400,420,440,465,490,515,540,565,
590,615,640,665,695,697,701,702,729,756,784,
812,841,870,900,930,961,992,992,992};

// initialize the LCD library with the numbers of the interface pins
LiquidCrystal lcd(7,8,9,10,11,12);

//initialize the real time clock
RTC_DS1307 rtc;

void displayCurrentTime(DateTime now){
  lcd.setCursor(6, 1);
    if(now.hour() < 10){
      lcd.print("0");
    }
    lcd.print(now.hour());
    lcd.print(":");
    if(now.minute() < 10){
      lcd.print("0");
    }
    lcd.print(now.minute());  
}

void setup () {

  pinMode(incrTimePin, INPUT);
  pinMode(decrTimePin, INPUT);
  //LED init
  pinMode(pulsepin, OUTPUT);
  
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Alarm:");
  lcd.setCursor(0,1);
  lcd.print("Time:");

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void loop () {
    DateTime now = rtc.now();
    
    /*alarm*/
    lcd.setCursor(7,0);
    if(alarmHour < 10){
      lcd.print("0");
    }
    lcd.print(alarmHour);
    lcd.print(":");
    if(alarmMinute < 10){
      lcd.print("0");
    }
    lcd.print(alarmMinute);
  
    /*clock*/
    displayCurrentTime(now);

    /*Logic for alarm to be triggered*/
    if(alarmHour == now.hour() && (alarmMinute-now.minute()) = 15){ //removed <=
      Serial.println("Alarm on");
      for (i=0; i<steps; i++){
        pwmTime = delays[i] * 5;
        for (j=0; j<sunrisespeed; j++)
        {
          // one pulse of PWM
          digitalWrite(pulsepin, HIGH);
          delayMicroseconds(pwmTime);
          digitalWrite(pulsepin, LOW);
          delayMicroseconds(5000-pwmTime);

          now = rtc.now();
          displayCurrentTime(now);
        }
      }
    }//end if
    else if((alarmHour-now.hour()) == 1 && (now.minute()-alarmMinute) = 45){//removed <=
      Serial.println("Alarm on");
      for (i=0; i<steps; i++){
        pwmTime = delays[i] * 5;
        for (j=0; j<sunrisespeed; j++){
          // one pulse of PWM
          digitalWrite(pulsepin, HIGH);
          delayMicroseconds(pwmTime);
          digitalWrite(pulsepin, LOW);
          delayMicroseconds(5000-pwmTime);

          now = rtc.now();
          displayCurrentTime(now);
        }
      }
    }//end else if

    //logic to increase alarm time
    incrTime = digitalRead(incrTimePin);
    if(incrTime == 0){
      alarmMinute = alarmMinute + 1;
      delay(100); //debounce
      if(alarmMinute >= 60){
        alarmMinute = 0;
        alarmHour = alarmHour + 1;
        if(alarmHour > 23){
          alarmHour = 0;
        }
      }
    }

    //logic to decrease alarm time
    decrTime = digitalRead(decrTimePin);
    if(decrTime == 0){
      alarmMinute = alarmMinute - 1;
      delay(100); //debounce
      if(alarmMinute < 0){
        alarmMinute = 59;
        alarmHour = alarmHour - 1;
        if(alarmHour < 0){
          alarmHour = 23;
        }
      }
    }
    
    delay(100);
}

