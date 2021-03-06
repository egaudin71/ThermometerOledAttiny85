# Thermometer with Oled screen and Attiny 85

## Features
  - show temperature measured by sensor DS18B20
  - push button stop showing value or make it appears
  - Auto powerdown for attiny85 and Oled after 30 secondes
  - push button awakes attiny for sleep mode
  
## Components
  - **Attiny 85** flashed at 8 MHz
  - **Oled screen** 0.91"
  - **DS18B20** Dallas digital temperature sensor
  - powered with one **CR2032 battery** 3.3 V 220 mAh
  - push button for interrupt
  - switch for battery control
  
## Libraries
  - #include <avr/io.h>
  - #include <avr/interrupt.h>
  - #include <avr/sleep.h>
  - #include <util/delay.h> for switch debouncing
  - #include <U8x8lib.h> for SS1306 Oled scrren
  - #include <OneWire.h> for Dallas sensor

  
## Consumption [Measured]
  - deep sleeping mode for Attiny and powersave for Oled screen
    - 7 µA
  - during temperature measurement and showing value
    - 7 mA = 3 mA for Attiny + 4 mA for Oled
    
## Life time [Estimation]
  - suppose showing temperature 10 times a day with 30 sec duration at 7 mA
    - 0.083 hrs/day = 0.583 mAh/day
  - suppose remaining time at 7 µA
    - ~ 24 hrs/day = 0.168 mAh/day
  - total consumption 0.751 mAh/day
    - with CR2032 capacity at 200 mAh : 366 days ~ 1 year
    
