#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DS3231.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,20,4);
DS3231 clock;
RTCDateTime dt;
bool display_state = true;
bool bre = false;
int buzzer = 12;
int relay = 4;

byte degree[] = {
  B00000,
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
};
byte underline[] = {
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

int set_year;
int set_month;
int set_day;
int set_hour;
int set_minute;
int set_sec;

int start_time = 0;
int start_time2 = 0;
int start_time3 = 0;
int timer_delay = 500;
int start_time_reset = 0;
int menu_start_time = 0;
bool alarm_flag = false;

void(* resetFunc) (void) = 0; //void using to reset Arduino Uno

void elec_init() { // void using to init lcd, ds3231, dht22 and define buzzer, buttons and relay pins
  dht.begin();
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.display();
  lcd.createChar(0, degree);
  lcd.createChar(1, underline);
  lcd.noCursor();
  digitalWrite(13, LOW);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH );
  clock.begin();
  pinMode(8, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
}

void menu(){ //void using to controll menu (with menu we can set time and set or disarm alarm
  menu_start_time = millis();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MENU");
  delay(100);
  lcd.setCursor(0,1);
  lcd.print("Loading...");
  delay(500);
  lcd.setCursor(0,2);
  lcd.print("Done!");
  delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set the time");
  lcd.setCursor(0,1);
  lcd.print("Set the alarm");
  lcd.setCursor(0,2);
  lcd.print("Disarm the alarm");
  lcd.setCursor(0,3);
  lcd.print("Turn display off");
  lcd.setCursor(19,0);
  lcd.print("*");
  int control = 0;
  while(true){
    delay(250);
    if((millis()-menu_start_time)>1000*30){
    lcd.clear();
    start_time2 = 0;
    return;
    }
  if(digitalRead(7) || digitalRead(8)){
    menu_start_time = millis();
    }
    if(digitalRead(7)){
      control += 1;
      if(control == 1){
        lcd.setCursor(19,0);
        lcd.print(" ");
        lcd.setCursor(19,1);
        lcd.print("*");
      } else if(control == 2){
          lcd.setCursor(19,1);
          lcd.print(" ");
          lcd.setCursor(19,2);
          lcd.print("*");
      } else if(control == 3){
          lcd.setCursor(19,2);
          lcd.print(" ");
          lcd.setCursor(19,3);
          lcd.print("*");
       }else if(control == 4){
          lcd.setCursor(19,3);
          lcd.print(" ");
          lcd.setCursor(19,0);
          lcd.print("*");
          control = 0;
       }
     }
          
    if(digitalRead(8) && digitalRead(7)){
      lcd.clear();
      break;
      }
    if(digitalRead(8)){
      if(control == 3){
        lcd.clear();
        lcd.noDisplay();
        lcd.noBacklight();
        display_state = false;
        return;
        }else{
          if(control == 2){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Alarm disarming...");
            delay(500);
            clock.armAlarm1(false);
            clock.clearAlarm1();
            delay(500);
            lcd.setCursor(0,1);
            lcd.print("Done!");
            delay(1000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Set the time");
            lcd.setCursor(0,1);
            lcd.print("Set the alarm");
            lcd.setCursor(0,2);
            lcd.print("Disarm the alarm");
            lcd.setCursor(0,3);
            lcd.print("Back");
            lcd.setCursor(19,0);
            lcd.print("*");
            control = 0;
          }else if(control == 1){
            lcd.clear();
            dt = clock.getDateTime();
            int year= dt.year;
            int month = dt.month;
            int day = dt.day;
            int hour = dt.hour;
            int minute = dt.minute;
            int sec = dt.second;

            lcd.setCursor(0, 0);
            lcd.print(year);
            lcd.print(".");
            if(month < 10){
              lcd.print(0);
            }
            lcd.print(month);
            lcd.print(".");
    
            if(day < 10){
              lcd.print(0);
            }
            lcd.print(day);
            lcd.setCursor(12, 0);
            if(hour < 10){
              lcd.print(0);
            }
            if(hour < 10){
              lcd.print(0);
            }
            lcd.print(hour);
            lcd.print(":");
            if(minute < 10){
              lcd.print(0);
            }
            lcd.print(minute);
            lcd.print(":");
            if(sec < 10){
              lcd.print(0);
            }
            lcd.print(sec);
            delay(400);
            lcd.setCursor(5,1);
            lcd.print("  ");
            lcd.setCursor(8,1);
            delay(10);
            lcd.write(byte(1));
            delay(10);
            lcd.write(byte(1));
            delay(100);
            while(true){
              if((millis()-menu_start_time)>1000*30){
                lcd.clear();
                start_time2 = 0;
                return;
              }
              if(digitalRead(7) || digitalRead(8)){
                 menu_start_time = millis();
              }
              if(digitalRead(7)){
                day+=1;
                if(day == 29 && month == 2 && year%4 != 0 ){
                  day=1;
                  }
                else if(day == 31 && (month == 4 || month == 6 ||month == 9 ||month == 11)){
                  day=1;
                  }
                else if(day > 31){
                  day = 1;
                  }
                lcd.setCursor(8,0);
                if(day < 10){
                  lcd.print(0);
                  }
                lcd.print(day);
              }
              if(digitalRead(8)){
                set_day = day;
                break;
                }
                delay(200);
              }
            delay(400);
            lcd.setCursor(8,1);
            lcd.print("  ");
            lcd.setCursor(12,1);
            delay(10);
            lcd.write(byte(1));
            delay(10);
            lcd.write(byte(1));
            delay(100);
            while(true){
              if((millis()-menu_start_time)>1000*30){
                lcd.clear();
                start_time2 = 0;
                return;
                }
              if(digitalRead(7) || digitalRead(8)){
                menu_start_time = millis();
                }
              if(digitalRead(7)){
                if(hour <= 24){
                  hour+=1;
                }else{
                  hour=1;
                  }
                lcd.setCursor(12,0);
                if(hour < 10){
                  lcd.print(0);
                  }
                lcd.print(hour);
                }
              if(digitalRead(8)){
                set_hour = hour;
                break;
                }
                delay(200);
              }


            delay(400);
            lcd.setCursor(12,1);
            lcd.print("  ");
            lcd.setCursor(15,1);
            delay(10);
            lcd.write(byte(1));
            delay(10);
            lcd.write(byte(1));
            delay(100);
            while(true){
              if((millis()-menu_start_time)>1000*30){
                lcd.clear();
                start_time2 = 0;
                return;
              }
              if(digitalRead(7) || digitalRead(8)){
                menu_start_time = millis();
                }
              if(digitalRead(7)){
                if(minute <= 59){
                  minute+=1;
                }else{
                  minute=1;
                  }
                lcd.setCursor(15,0);
                if(minute < 10){
                  lcd.print(0);
                  }
                lcd.print(minute);
                }
              if(digitalRead(8)){
                set_minute = minute;
                break;
                }
                delay(200);
              }


            delay(400);
            lcd.setCursor(15,1);
            lcd.print("  ");
            delay(1);
            lcd.setCursor(18,1);
            delay(10);
            lcd.write(byte(1));
            delay(10);
            lcd.write(byte(1));
            delay(400);
            while(true){
              if((millis()-menu_start_time)>1000*30){
              lcd.clear();
              start_time2 = 0;
              return;
              }
              if(digitalRead(7) || digitalRead(8)){
                menu_start_time = millis();
              }
              if(digitalRead(7)){
                if(sec < 60){
                  sec+=1;
                }else{
                  sec=1;
                  }
                lcd.setCursor(18,0);
                if(sec < 10){
                  lcd.print(0);
                  }
                lcd.print(sec);
                }
              if(digitalRead(8)){
                set_sec = sec;
                break;
                }
                delay(200);
              }
              clock.setAlarm1(set_day, set_hour, set_minute, set_sec, DS3231_MATCH_DT_H_M_S);
              clock.armAlarm1(false);
              clock.clearAlarm1();
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Set the time");
              lcd.setCursor(0,1);
              lcd.print("Set the alarm");
              lcd.setCursor(0,2);
              lcd.print("Disarm the alarm");
              lcd.setCursor(0,3);
              lcd.print("Turn display off");
              lcd.setCursor(19,0);
              lcd.print("*");
              control = 0;
              
          }else if(control == 0){
            lcd.clear();
            dt = clock.getDateTime();
            int year= dt.year;
            int month = dt.month;
            int day = dt.day;
            int hour = dt.hour;
            int minute = dt.minute;
            int sec = dt.second;
            

            lcd.setCursor(0, 0);
            lcd.print(year);
            lcd.print(".");
            if(month < 10){
              lcd.print(0);
            }
            lcd.print(month);
            lcd.print(".");
    
            if(day < 10){
              lcd.print(0);
            }
            lcd.print(day);
            lcd.setCursor(12, 0);
            if(hour < 10){
              lcd.print(0);
            }
            if(hour < 10){
              lcd.print(0);
            }
            lcd.print(hour);
            lcd.print(":");
            if(minute < 10){
              lcd.print(0);
            }
            lcd.print(minute);
            lcd.print(":");
            if(sec < 10){
              lcd.print(0);
            }
            lcd.print(sec);
            delay(400);
            lcd.setCursor(0,1);
            delay(10);
            lcd.write(byte(1));
            delay(10);
            lcd.write(byte(1));
            delay(10);
            lcd.write(byte(1));
            delay(10);
            lcd.write(byte(1));
            delay(70);
            while(true){
              if((millis()-menu_start_time)>1000*30){
                lcd.clear();
                start_time2 = 0;
                return;
              }
              if(digitalRead(7) || digitalRead(8)){
                menu_start_time = millis();
              }
              if(digitalRead(7)){
                if(year < 2100){
                  year+=1;
                }else{
                  year=2000;
                  }
                lcd.setCursor(0,0);
                if(year < 10){
                  lcd.print(0);
                  }
                lcd.print(year);
                }
              if(digitalRead(8)){
                set_year = year;
                break;
                }
                delay(200);
              }

            lcd.setCursor(0,1);
            lcd.print("    ");
            lcd.setCursor(5,1);
            delay(10);
            lcd.write(byte(1));
            delay(10);
            lcd.write(byte(1));
            delay(400);
            while(true){
              if((millis()-menu_start_time)>1000*30){
                lcd.clear();
                start_time2 = 0;
                return;
              }
              if(digitalRead(7) || digitalRead(8)){
              menu_start_time = millis();
              }
              if(digitalRead(7)){
                if(month < 12){
                  month+=1;
                }else{
                  month=1;
                  }
                lcd.setCursor(5,0);
                if(month < 10){
                  lcd.print(0);
                  }
                lcd.print(month);
                }
              if(digitalRead(8)){
                set_month = month;
                break;
                }
                delay(200);
              }
            delay(400);
            lcd.setCursor(5,1);
            lcd.print("  ");
            lcd.setCursor(8,1);
            delay(10);
            lcd.write(byte(1));
            delay(10);
            lcd.write(byte(1));
            delay(100);
            while(true){
              if((millis()-menu_start_time)>1000*30){
                lcd.clear();
                start_time2 = 0;
                return;
                }
              if(digitalRead(7) || digitalRead(8)){
                menu_start_time = millis();
              }
              if(digitalRead(7)){
                day+=1;
                if(day == 29 && month == 2 && year%4 != 0 ){
                  day=1;
                  }
                else if(day == 31 && (month == 4 || month == 6 ||month == 9 ||month == 11)){
                  day=1;
                  }
                else if(day > 31){
                  day = 1;
                  }
                lcd.setCursor(8,0);
                if(day < 10){
                  lcd.print(0);
                  }
                lcd.print(day);
              }
              if(digitalRead(8)){
                set_day = day;
                break;
                }
                delay(200);
              }
            delay(400);
            lcd.setCursor(8,1);
            lcd.print("  ");
            lcd.setCursor(12,1);
            delay(10);
            lcd.write(byte(1));
            delay(10);
            lcd.write(byte(1));
            delay(100);
            while(true){
                if((millis()-menu_start_time)>1000*30){
                lcd.clear();
                start_time2 = 0;
                return;
              }
              if(digitalRead(7) || digitalRead(8)){
                menu_start_time = millis();
              }
              if(digitalRead(7)){
                if(hour < 50){
                  hour+=1;
                }else{
                  hour=1;
                  }
                lcd.setCursor(12,0);
                if(hour < 10){
                  lcd.print(0);
                  }
                lcd.print(hour);
                }
              if(digitalRead(8)){
                set_hour = hour;
                break;
                }
                delay(200);
              }


            delay(400);
            lcd.setCursor(12,1);
            lcd.print("  ");
            lcd.setCursor(15,1);
            delay(10);
            lcd.write(byte(1));
            delay(10);
            lcd.write(byte(1));
            delay(100);
            while(true){
              delay(10);
              if((millis()-menu_start_time)>1000*30){
                lcd.clear();
                start_time2 = 0;
                return;
              }
              if(digitalRead(7) || digitalRead(8)){
                menu_start_time = millis();
                }
              if(digitalRead(7)){
                if(minute < 60){
                  minute+=1;
                }else{
                  minute=1;
                  }
                lcd.setCursor(15,0);
                if(minute < 10){
                  lcd.print(0);
                  }
                lcd.print(minute);
                }
              if(digitalRead(8)){
                set_minute = minute;
                break;
                }
                delay(200);
              }


            delay(400);
            lcd.setCursor(15,1);
            lcd.print("  ");
            delay(1);
            lcd.setCursor(18,1);
            delay(10);
            lcd.write(byte(1));
            delay(10);
            lcd.write(byte(1));
            delay(400);
            while(true){
              if((millis()-menu_start_time)>1000*30){
                lcd.clear();
                start_time2 = 0;
                return;
                }
              if(digitalRead(7) || digitalRead(8)){
                menu_start_time = millis();
                }
                
              if(digitalRead(7)){
                if(sec < 60){
                  sec+=1;
                }else{
                  sec=1;
                  }
                lcd.setCursor(18,0);
                if(sec < 10){
                  lcd.print(0);
                  }
                lcd.print(sec);
                }
              if(digitalRead(8)){
                set_sec = sec;
                break;
                }
                delay(200);
              }
              delay(5);
              clock.setDateTime(set_year, set_month, set_day, set_hour, set_minute, set_sec);
              delay(10); 
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Set the time");
              lcd.setCursor(0,1);
              lcd.print("Set the alarm");
              lcd.setCursor(0,2);
              lcd.print("Disarm the alarm");
              lcd.setCursor(0,3);
              lcd.print("Turn display off");
              lcd.setCursor(19,0);
              lcd.print("*");
              control = 0;
            }
        }
     }
  }
  delay(100);
}



void setup() { //void using to init Serial (with baudration 9600), to begin <Wire.h> (using to i2c interface) and it induce elec_init() void
  Serial.begin(9600);
  Wire.begin();
  elec_init();
  //clock.setDateTime(__DATE__,__TIME__);  
}

void loop() { //void using to write date, time and sensor date to a display and to check button states(reset or menu)
  if ((millis() - start_time3 > timer_delay/2 | start_time3 == 0) && alarm_flag == false) {
    if(digitalRead(8) || digitalRead(7)){  
      delay(500);
      if(digitalRead(8) && digitalRead(7)){
        if(start_time_reset == 0){start_time_reset == millis();}
        if (millis() - start_time_reset > 7500) {
            resetFunc();
          }else if(digitalRead(8) != true && digitalRead(7) != true){
            if(display_state == true){
              menu();
              delay(500);
              }else {
                lcd.display();
                lcd.backlight();
                display_state = true;
                delay(10);
                Serial.println("ok1");
              }
          }
      
      }else{
        if(display_state == true){
          menu();
          delay(500);
        }else{
          lcd.display();
          lcd.backlight();
          display_state = true;
          delay(10);
          Serial.println("ok2");
          }
      }
    }
    start_time3 = millis();
  }
  if ((millis() - start_time > timer_delay | start_time == 0)&& display_state == true) {
    if (clock.isAlarm1() || alarm_flag == true){
      alarm_flag = true;
      if(digitalRead(7)|| digitalRead(8)){
        lcd.setCursor(6,3);
        delay(5);
        lcd.print("        ");
        clock.armAlarm1(false);
        clock.clearAlarm1();
        alarm_flag = false;
        start_time2 = 0;
        start_time = 0;;
        delay(100);
        lcd.clear();
        }
    
      lcd.setCursor(6,3);
      lcd.print("ALARM!!!");
      digitalWrite(relay,LOW);
      delay(10);
      unsigned char i;
      for(i=0;i<80;i++){
        digitalWrite(buzzer,HIGH);
        delay(1);
        digitalWrite(buzzer,LOW);
        delay(1);
        }
      delay(5);
      digitalWrite(relay,HIGH);
      delay(5);
    }else{
      delay(10);
      lcd.setCursor(6,3);
      delay(10);
      lcd.print("        ");
      delay(10);
      }
    dt = clock.getDateTime();
    
    int year= dt.year;pinMode(relay, OUTPUT);
    int month = dt.month;
    int day = dt.day;
    int hour = dt.hour;
    int minute = dt.minute;
    int sec = dt.second;
    lcd.setCursor(0, 0);
    lcd.print(year);
    lcd.print(".");
    if(month < 10){
      lcd.print(0);
    }
    lcd.print(month);
    lcd.print(".");
    
    if(day < 10){
      lcd.print(0);
    }
    lcd.print(day);
    lcd.setCursor(12, 0);
    if(hour < 10){
      lcd.print(0);
    }
    lcd.print(hour);
    lcd.print(":");
    if(minute < 10){
      lcd.print(0);
    }
    lcd.print(minute);
    lcd.print(":");
    if(sec < 10){
      lcd.print(0);
    }
    lcd.print(sec);
    
    start_time = millis();
  }
  if ((millis() - start_time2 > timer_delay*10 | start_time2 == 0)&& display_state == true) {
    lcd.setCursor(0,2);
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    float t_conv = floorf(t * 100) / 100;
    float h_conv = floorf(h * 100) / 100;
    lcd.print(t_conv);
    delay(10);
    lcd.write(byte(0));
    lcd.print("C");
    if(h_conv < 10){
      lcd.setCursor(13,2);
      }
    else{
      if(h_conv < 100){
        lcd.setCursor(12,2);
        }
      else{
        lcd.setCursor(11,2);
        }
    }
    lcd.print(h_conv);
    lcd.print("%RH");
    start_time2 = millis();
    }
}
