# Arduino Uno Clock

My project usinng ds3231 rtc module, dht22 and lcd i2c. It use buzzer for an alarm (with a relay controll).
I programmed a lcd menu, controlling by two buttons. To reset Arduino Uno click two buttons for 5 seconds. To enter for a menu you have to click one of two buttons.


How to connect?<br>
  lcd:
    GND → GND
    VCC → 5v
    SDA → SDA
    SCL → SCL
  ds3231:
    GND → GND
    VCC → 5v
    SDA → SDA
    SCL → SCL
  dht22:
    + → 3.3V
    - → GND
    OUT → GPIO 2
  buzzer:
    + → NC (NORMALLY CLOSED)
    - → GND
    OUT → GPIO 12
  RED BUTTON:
    VCC → 5V
    GND → GND
    OUT → GPIO 8
  YELLOW BUTTON:
    VCC → 5V
    GND → GND
    OUT → GPIO 7
  RELAY:
    CONTROL CIRCULATION:
      VCC → 3.3V
      GND → GND
      IN → GPIO 4
    OUTPUT CIRCULATION:
      COMMMON CONTACT → 5V
      NORMALLY CLOSED → BUZZER'S + 
      NORMALLY OPEN - NOT CONNECTED
