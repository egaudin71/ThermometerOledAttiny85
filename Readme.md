# Thermometer

## Components
  - Attiny 85
  - Oled screen 0.91"
  - DS18B20 digital temperature sensor
  - powered with one CR2032 battery 220 mAh
  - push switch for interrupt
  - switch for battery control
  
## Consumption
  - deep sleeping mode for Attiny and powersave for Olde screen
    - 7µA
  - during temperature measurement and showing value
    - 7 mA = 3 mA for Attiny + 4 mA for Oled
