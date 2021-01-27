# attiny85-ds18b20-4seg
Code and wiring for display temperature from ds18b20 on 7 segments display with ATtiny85

## Wiring

<img src="https://raw.githubusercontent.com/ZoLuSs/attiny85-ds18b20-7seg/main/Schematic_attiny85-ds18b20-7seg.svg" alt="wiring">

The resistor between DS18B20 data and VCC is idealy 4.7K Ohms but it can be everything between 4.7K and 5.3K Ohms.

## Libraries
- DS18B20 (interpreteur): <a href="https://github.com/milesburton/Arduino-Temperature-Control-Library">DallasTemperature</a>
- DS18B20 (Communication): <a href="https://www.pjrc.com/teensy/td_libs_OneWire.html">OneWire</a>
- 7-segments (TM1637): <a href="https://github.com/avishorp/TM1637">TM1637</a>

## Code details

### Import Libraries
```c++
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TM1637Display.h>
```

### Set variables and object
- ONE_WIRE_BUS = DS18B20 pin
- CLK = TM1637 Clock pin
- DIO = TM1637 Data pin

const uint8_t = Create custom segments referring to <a href="https://www.arduinoplatform.com/wp-content/uploads/2020/01/TM1637-Display.jpg">This picture</a>
```c++
#define ONE_WIRE_BUS 2
#define CLK 4
#define DIO 3

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
TM1637Display display(CLK, DIO);

int temp;

const uint8_t degree[] = {
  SEG_A | SEG_B | SEG_F | SEG_G  // °
};

const uint8_t celsius[] = {
  SEG_A | SEG_D | SEG_E | SEG_F   // C
};

const uint8_t Err[] = {
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,            // E
  SEG_E | SEG_G,                                    // r
  SEG_E | SEG_G                                     // r
};

const uint8_t CAPt[] = {
  SEG_A | SEG_D | SEG_E | SEG_F,                    // C
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,    // A
  SEG_A | SEG_B | SEG_E | SEG_F | SEG_G,            // P
  SEG_D | SEG_E | SEG_F | SEG_G                     // t
};
```

### Setup
- Set brightness of 4-segments display. Can be hex or int, from 8 (min) to 15 (max)
- Clear display
- Open communication with DS18B20

```C++
void setup() {
  display.setBrightness(10);// 8 - 15 | min - max
  display.clear();
  sensors.begin();
}
```

### Loop
- Read sensor
### Temp
In order
1. temperature_from_sensor * 100
21.38 -> 2138
2. 2138 / 10
213.8
3. round
213.8 -> 214

The purpose is to put in int, one decimal rounded float value (temperature)

### if else
- Check if the sensor is wired (unwired sensors output -128 or -1280 after calc) and also if the value is in the capacities of the sensors

#### if data is good
- Display 2 first numbers of int (by dividing it) | false for not showing 0 after when value under 10 | 2 is the number of digits we want to show | 0 is the segment to display
- Display custom segments degree 
- Display custom segments celsius

- Like before display 2 first number of int
- Same
- Display decimal

#### else
- display Err (custom segments)
- "" is for delete old degree symbols without display.clear()
- display CAPt
CAPt(capteur) is SENs(sensor) in french...

```C++

void loop() {
  sensors.requestTemperatures();

  temp = round((sensors.getTempCByIndex(0)*100)/10);

  if (temp < 1250 && temp > -1270) {
    display.showNumberDec(temp / 10, false, 2, 0);
    display.setSegments(degree, 1, 2);
    display.setSegments(celsius, 1, 3);
    delay(2000);

    display.showNumberDec(temp / 10, false, 2, 0);
    display.setSegments(degree, 1, 2);
    display.showNumberDec(temp, false, 1, 3);
    delay(2000);
  }
  else {
    display.setSegments(Err, 3, 0);
    display.setSegments("", 1, 3);
    delay(1000);
    display.setSegments(CAPt, 4, 0);
    delay(1000);
  }
}
```

## More

### Multiple sensors
You can have more than one sensor on the OneWire bus.
If it is, juste change number in sensors.getTempCByIndex(X).

### Other µcontroller
The code is completely compatible with any other Arduino-like µcontroller.
The only limitation is library compatibilities.
