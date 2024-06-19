# Arduino Uno Clock

My project usinng ds3231 rtc module, dht22 and lcd i2c. It use buzzer for an alarm (with a relay controll).
I programmed a lcd menu, controlling by two buttons. To reset Arduino Uno click two buttons for 5 seconds. To enter for a menu you have to click one of two buttons.


How to connect?<br>
    <p>  </p>lcd:<br>
        GND → GND<br>
        VCC → 5v<br>
        SDA → SDA<br>
        SCL → SCL<br>
    ds3231:<br>
        GND → GND<br>
        VCC → 5v<br>
        SDA → SDA<br>
        SCL → SCL<br>
    dht22:<br>
        + → 3.3V<br>
        - → GND<br>
        OUT → GPIO 2<br>
    buzzer:<br>
        + → NC (NORMALLY CLOSED)<br>
        - → GND<br>
        OUT → GPIO 12<br>
    RED BUTTON:<br>
        VCC → 5V<br>
        GND → GND<br>
        OUT → GPIO 8<br>
    YELLOW BUTTON:<br>
        VCC → 5V<br>
        GND → GND<br>
        OUT → GPIO 7<br>
    RELAY:<br>
        CONTROL CIRCULATION:<br>
            VCC → 3.3V<br>
            GND → GND<br>
            IN → GPIO 4<br>
        OUTPUT CIRCULATION:<br>
            COMMMON CONTACT → 5V<br>
            NORMALLY CLOSED → BUZZER'S +<br> 
            NORMALLY OPEN - NOT CONNECTED<br>
