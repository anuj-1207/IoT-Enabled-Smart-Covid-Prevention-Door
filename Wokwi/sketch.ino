#include "Functions.h"

void setup() 
{
  lcd.init(); // initialize the lcd
  lcd.backlight();
  for (auto& sensor : dht) 
  {
    sensor.begin();
  }
  for(int d=0; d<8; d++)
  {
    pinMode(displayPins[d], OUTPUT);// Declaring 7 segment display pins as output  
  }
  if (! rtc.begin())
   {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  pinMode(dicimalPoint,OUTPUT);
  digitalWrite(dicimalPoint,LOW);
  writeDigit(0);
  Door.attach(5);
  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);
  pinMode(Smart_Lighting, OUTPUT);
  pinMode(EntrySensor_TRIG_PIN, OUTPUT);
  pinMode(EntrySensor_ECHO_PIN, INPUT);
  pinMode(ExitSensor_TRIG_PIN, OUTPUT);
  pinMode(ExitSensor_ECHO_PIN, INPUT);
  pinMode(SanitizationSensor_TRIG_PIN, OUTPUT);
  pinMode(SanitizationSensor_ECHO_PIN, INPUT);
  pinMode(Mask, INPUT_PULLUP);
  pinMode(No_Mask, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  Door.write(90);
  Display_Moto();
  Serial.begin(9600);

}

void loop() 
{
  int Mask_detected = digitalRead((Mask));
  int Mask_not_detected = digitalRead((No_Mask));
  int flag = 1;
  if(!Mask_detected And count < 3)
  {
    A:;
    Entry = Check_Time();
    lcd.init();
    write_message("Person Arrived",1);
    write_message("Mask detected",2);
    delay(1500);
    write_message("Reading temperature",3);
    delay(5000);
    while(1)
    {
      temperature = read_temperature();
      if(Timeout())
      {
        Reset();
        break;
      }
      else if(temperature <= 37.5)
      {
        erase_lcd(2);
        write_message("Temp. is normal",4);
        delay(2000);
        Entry = Check_Time();
        while(1)
        {
          if(Timeout())
          {
            Reset();
            break;
          }
          float distance = readDistanceCM(SanitizationSensor_TRIG_PIN,SanitizationSensor_ECHO_PIN);
          bool sanitized = distance < 15;
          if(sanitized)
          {
            Entry = Check_Time();
            while(1)
            {
              if(Timeout())
              {
                Reset();
                break;
              }
              digitalWrite(Blue, HIGH);
              distance = readDistanceCM(SanitizationSensor_TRIG_PIN,SanitizationSensor_ECHO_PIN);
              bool sanitized_done = distance > 15;
              if(sanitized_done)
              {
                digitalWrite(Blue, LOW);
                erase_lcd(3);
                write_message("Hands Sanitized",6);
                // digitalWrite(Green, HIGH);
                delay(3000);
                digitalWrite(Green, HIGH);
                lcd.init();
                write_message("Covid Prevention Test done",7);
                write_message("YOU'RE WELCOME..",8);
                Open_door();
                Check_entry();
                if(count == 1)
                { digitalWrite(Smart_Lighting, HIGH); }
                break;
              }
            }
            break;
          }
          else if(!sanitized And flag)
          {
            flag = 0;
            write_message("Sanitize your hands",5);
          }
        }
        break;
      }
      else if(temperature > 37.5)
      {
        erase_lcd(2);
        write_message("Temperature is high..You're not allowed",11);
        for(int i = 0; i<4; i++)
        {
          digitalWrite(Red, LOW);
          tone(buzzer, 1200, 500);
          digitalWrite(Red, HIGH);
          delay(1000);
        }
        digitalWrite(Red, LOW);
        break;
      }
    }
    delay(3000);
    lcd.init();
    Display_Moto();
    digitalWrite(Green, LOW);
  }
  else if(!Mask_detected And count == 3)
  {
    lcd.init();
    write_message("Room occupancy full..!\nNow no one allowed further..",12);
    for(int i = 0; i<4; i++)
    {
      digitalWrite(Red, LOW);
      tone(buzzer, 1200, 500);
      digitalWrite(Red, HIGH);
      delay(1000);
    }
    digitalWrite(Red, LOW);
    delay(3000);
    lcd.init();
    Display_Moto();
  }
  else if(!Mask_not_detected And count < 3)
  {
    lcd.init(); 
    Entry = Check_Time();
    write_message("Person Arrived \n Mask not detected \n Not allowed without mask",13);
    while(1)
    {
      if(Timeout())
      {
        Reset();
        break;
      }
      Mask_detected = digitalRead((Mask));
      Mask_not_detected = digitalRead((No_Mask));
      digitalWrite(Red, LOW);
      tone(buzzer, 1200, 500);
      digitalWrite(Red, HIGH);
      delay(1000);
      if(!Mask_detected And Mask_not_detected)
      {
        digitalWrite(Red, LOW);
        goto A;
        break;
      }
    }
  }
  else if(!Mask_not_detected)
  {
    lcd.init();
    write_message("Room occupancy full..!\nNow no one allowed further..",12);
    for(int i = 0; i <4; i++)
    {  
      digitalWrite(Red, LOW);
      tone(buzzer, 1200, 500);
      digitalWrite(Red, HIGH);
      delay(1000);
    }
    digitalWrite(Red, LOW);
    delay(3000);
    lcd.init();
    Display_Moto();
  }
  else if(Check(ExitSensor_TRIG_PIN,ExitSensor_ECHO_PIN) And count > 0)
  {
    Check_exit();
    if(count == 0)
    { digitalWrite(Smart_Lighting, LOW); }
    // lcd.init();
    delay(3000);
    lcd.init();
    Display_Moto();
  }
  else if (count == 0 And Check(ExitSensor_TRIG_PIN,ExitSensor_ECHO_PIN))
  {
    Entry = Check_Time();
    while(1)
    {
      if(Timeout())
      {
        Reset();
        break;
      }
      else if(!Check(ExitSensor_TRIG_PIN, ExitSensor_ECHO_PIN))
      {
        lcd.init();
        write_message("Room occupancy 0..",15);
        write_message("Everyone left room..",16);
        delay(3000);
        lcd.init();
        Display_Moto();
        break;
      }
    }
  }
}