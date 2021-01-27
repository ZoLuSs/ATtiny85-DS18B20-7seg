#include <OneWire.h>
#include <DallasTemperature.h>
#include <TM1637Display.h>

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

void setup() {
  display.setBrightness(10);// 8 - 15 | min - max
  display.clear();
  sensors.begin();
}

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
