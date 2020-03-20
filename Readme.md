# Thermometer with Oled screen and attiny 85

## Features
  - show temperature measured by sensor DS18B20
  - push button stop showing value or make it appears
  - Auto powerdown for attiny85 and Oled after 30 secondes
  - push button awakes attiny for sleep mode
  
## Components
  - **Attiny 85** flashed at 8 MHz
  - Oled screen 0.91"
  - **DS18B20** Dallas digital temperature sensor
  - powered with one CR2032 battery 220 mAh
  - push button for interrupt
  - switch for battery control
  
## Consumption
  - deep sleeping mode for Attiny and powersave for Oled screen
    - 7µA
  - during temperature measurement and showing value
    - 7 mA = 3 mA for Attiny + 4 mA for Oled
    
## Life time estimation
  - Suppose showing temperature 10 times a day with 30 sec duration
  
