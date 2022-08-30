#ifndef Start

#include <DHT.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"

#define EntrySensor_ECHO_PIN 8
#define EntrySensor_TRIG_PIN 9

#define ExitSensor_ECHO_PIN 6
#define ExitSensor_TRIG_PIN 7

#define SanitizationSensor_ECHO_PIN 10
#define SanitizationSensor_TRIG_PIN 11

#define buzzer 12

#define Temperature_Sensor 4

#define Red 14
#define Green 15
#define Blue 16

#define Smart_Lighting  49

#define And &&

#define Mask 2
#define No_Mask 3

int convertHighLow(int);
void writeDigit(int);
bool Timeout(void);
int Check_Time(void);
void Reset(void);
void write_message(char*,int);

LiquidCrystal_I2C lcd(0x27,20,4); 
DHT dht[] = {{Temperature_Sensor, DHT22}};
RTC_DS1307 rtc;
Servo Door;

int count = 0;
int Entry,Exit;
float temperature;

float readDistanceCM(int TRIG_PIN,int ECHO_PIN) 
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW); 
  int duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

float read_temperature()
{
    float temperature = dht[0].readTemperature();
    return temperature;
}

void Open_door()
{
  for(int i = 91; i<180; i++)
  {
    Door.write(i);
    delay(25);
  }
  delay(500);
  for(int i = 180; i >= 90; i--)
  {
    Door.write(i);
    delay(25);
  }
}
bool Check(int TRIG_PIN,int ECHO_PIN)
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  int duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;
  if(distance < 150)
    return true;
  else
    return false;
}

void Check_entry()
{
    Entry = Check_Time();
    while(1)
    {
      if(Timeout())
      {
        Reset();
        break;
      }
      else if(Check(EntrySensor_TRIG_PIN,EntrySensor_ECHO_PIN))
      {
        Entry = Check_Time();
        while(1)
        {
          if(Timeout())
          {
            Reset();
            break;
          }
          else if(!Check(EntrySensor_TRIG_PIN,EntrySensor_ECHO_PIN))
          {
            count = count + 1;
            writeDigit(count);
            lcd.init();
            write_message("Person entered..",9);
            write_message("No of people inside the room are ",10);
            break;
          }
        }
        break;
      }
    }
}
void Check_exit()
{
  Entry = Check_Time();
  while(1)
  {
    if(Timeout())
    {
      Reset();
      break;
    }
    else if(Check(ExitSensor_TRIG_PIN,ExitSensor_ECHO_PIN))
    {
      Entry = Check_Time();
      while(1)
      {
        if(Timeout())
        {
          Reset();
          break;
        }
        else if(!Check(ExitSensor_TRIG_PIN,ExitSensor_ECHO_PIN))
        {
          count = count - 1;
          writeDigit(count);
          lcd.init();
          write_message("Person left..",14);
          write_message("No of people inside the room are ",10);
          break;
        }
      }
      break;
    }
  }
}
void Display_Moto()
{
  lcd.setCursor(4,0);
  lcd.print("IOT ENABLED");
  lcd.setCursor(4,1);
  lcd.print("SMART COVID");
  lcd.setCursor(2,2);
  lcd.print("PREVENTION DOOR");
  lcd.setCursor(0,3);
  lcd.print("CS698T_Final_Project");
}
void write_message(char message[100],int pos)
{
  if(pos == 1)
    lcd.setCursor(3,pos-1);
  else if(pos == 2)
    lcd.setCursor(3,pos-1);
  else if(pos == 3)
    lcd.setCursor(1,pos-1);
  else if(pos == 4)
    lcd.setCursor(2,pos-2);
  else if(pos == 5)
    lcd.setCursor(1,pos-2);
  else if(pos == 6)
    lcd.setCursor(2,pos-3);
  else if(pos == 7)
  {
    lcd.setCursor(2,0);
    lcd.print("Covid Prevention");
    lcd.setCursor(4,1);
    message = "Test Done.";
  }
  else if(pos == 8)
    lcd.setCursor(2,3);
  else if(pos == 9)
    lcd.setCursor(2,0);
  else if(pos == 10)
  {
    lcd.setCursor(0,2);
    lcd.print("No of people inside ");
    lcd.setCursor(0,3);
    lcd.print("the room are :  ");
    lcd.print(count);
    message = "";
  }
  else if(pos == 11)
  {
    lcd.setCursor(1,2);
    lcd.print("Temperature is high");
    lcd.setCursor(1,3);
    lcd.print("You're not allowed");
    message = "";
  }
  else if(pos == 12)
  {
    lcd.setCursor(0,0);
    lcd.print("Room occupancy full..");
    lcd.setCursor(0,2);
    lcd.print("Now no one allowed ");
    lcd.setCursor(0,3);
    lcd.print("further..");
    message = "";
  }
  else if(pos == 13)
  {
    lcd.setCursor(3,0);
    lcd.print("Person Arrived");
    lcd.setCursor(2,1);
    lcd.print("Mask not detected");
    lcd.setCursor(2,2);
    lcd.print("Entry mot allowed");
    lcd.setCursor(4,3);
    lcd.print("without mask..");
    message = "";
  }
  else if(pos == 14)
    lcd.setCursor(3,0);
  else if(pos == 15)
    lcd.setCursor(1,0);
  else if(pos == 16)
    lcd.setCursor(0,2);
  else if(pos == 17)
  {
    lcd.setCursor(5,0);
    lcd.print("TIMEOUT..!");
    lcd.setCursor(0,1);
    lcd.print("Your session expired");
    lcd.setCursor(1,3);
    lcd.print("PLEASE TRY AGAIN.");
    message = "";
  }
  lcd.print(message);
}
void erase_lcd(int line)
{
  int i = 0;
  lcd.setCursor(0,line);
  while(i < 20)
  {
    lcd.print(" ");
    i++;
  }
}

int Check_Time()
{
  DateTime now = rtc.now();
  int sec = now.second();
  return sec;
}

int Diff(int a,int b)
{
  if(a < b)
    return (b-a) * 1000;
  else 
    return ((60 - a) + b) * 1000;
}

bool Timeout()
{
  Exit = Check_Time();
  if(Diff(Entry,Exit) > 30000)
    return true;
  else 
    return false; 
}

void Reset()
{
  lcd.init();
  write_message("Timeout..:( \n Session Expired ",17);
  for(int i = 0; i<4; i++)
  {
    tone(buzzer, 1200, 500);
    delay(1000);
  }
  digitalWrite(Blue, LOW);
  digitalWrite(Red, LOW);
  delay(3000);
  lcd.init();
  Display_Moto();
}

#define type 1 
const int displayPins[7] = {34,35,36,37,38,39,40};
const int dicimalPoint = 9;

// array for seven segment LED values. Should not be changed.

byte displayLEDs[10][7] = { 
        { 0,0,0,0,0,0,1 },  // = 0
        { 1,0,0,1,1,1,1 },  // = 1
        { 0,0,1,0,0,1,0 },  // = 2
        { 0,0,0,0,1,1,0 },  // = 3
        { 1,0,0,1,1,0,0 },  // = 4
        { 0,1,0,0,1,0,0 },  // = 5
        { 0,1,0,0,0,0,0 },  // = 6
        { 0,0,0,1,1,1,1 },  // = 7
        { 0,0,0,0,0,0,0 },  // = 8
        { 0,0,0,0,1,0,0 }   // = 9     
        }; 

void writeDigit(int digit) 
{
  int digitValue;
  for (int i=0; i < 7; i++) 
  {

      digitalWrite(displayPins[i], convertHighLow(displayLEDs[digit][i]));
  }
}
int convertHighLow(int v)
{
  int convertedValue;
    if(type ==0)
    {
      convertedValue = 1 - v;
    }
    else
    {
      convertedValue = v;
    } 

    return convertedValue;
}

#endif