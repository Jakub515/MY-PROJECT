# Arduino Uno Clock

My project usinng ds3231 rtc module, dht22 and lcd i2c. It use buzzer for an alarm (with a relay controll).
I programmed a lcd menu, controlling by two buttons. To reset Arduino Uno click two buttons for 5 seconds. To enter for a menu you have to click one of two buttons.<br>


How to connect?<br><br>
    lcd:<br>
        GND → GND<br>
        VCC → 5v<br>
        SDA → SDA<br>
        SCL → SCL<br><br>
    ds3231:<br>
        GND → GND<br>
        VCC → 5v<br>
        SDA → SDA<br>
        SCL → SCL<br><br>
    dht22:<br>
        + → 3.3V<br>
        - → GND<br>
        OUT → GPIO 2<br><br>
    buzzer:<br>
        + → NC (NORMALLY CLOSED)<br>
        - → GND<br>
        OUT → GPIO 12<br><br>
    RED BUTTON:<br>
        VCC → 5V<br>
        GND → GND<br>
        OUT → GPIO 8<br><br>
    YELLOW BUTTON:<br>
        VCC → 5V<br>
        GND → GND<br>
        OUT → GPIO 7<br><br>
    RELAY:<br><br>
        CONTROL CIRCULATION:<br><br>
            VCC → 3.3V<br>
            GND → GND<br>
            IN → GPIO 4<br><br>
        OUTPUT CIRCULATION:<br><br>
            COMMMON CONTACT → 5V<br>
            NORMALLY CLOSED → BUZZER'S +<br> 
            NORMALLY OPEN - NOT CONNECTED<br>
