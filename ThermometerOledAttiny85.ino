#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <U8x8lib.h>
#include <OneWire.h>

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 2, /* data=*/ 0, /* reset=*/ U8X8_PIN_NONE);

#define ONE_WIRE_BUS PB1 // capteur temperature
#define INTERRUPT_PIN1 PCINT3 // Photo transistor
#define INTERRUPT_PIN0 PCINT4 // bouton
#define BUTTON PB4 // bouton
#define PHOTOTRANS PB3 // Photo transistor

#define DEBOUNCE 20  // anti rebond
#define ALARM 30000 // alarm in ms alarm time - 30 sec
#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)

OneWire oneWire(ONE_WIRE_BUS);// for Dallas sensors D18B20 precision 0.0625 deg

//----------------------- graphique et page
volatile byte page = 1;
volatile byte sleeping = 0; // 0 == awake and 1 == sleeping
unsigned long StartTime = 0;        // start time

//================================================
void setup()
//================================================
{
  u8x8.begin();
  u8x8.setPowerSave(0); //screen powered up
  u8x8.setFlipMode(0); //0 for 180 deg rotation
  u8x8.setFont(u8x8_font_inr21_2x4_n);

  DDRB &= ~(1 << BUTTON);//configured button in input
  DDRB &= ~(1 << PHOTOTRANS);//configured phototrans in input
  PORTB |= (1 << BUTTON);// pullup for button
  PORTB |= (1 << PHOTOTRANS);// pullup for phototransistor if needed

  adc_disable();// for low power reduction
  initInterupt();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

}

//================================================
void initInterupt()
//================================================
{
  cli();
  GIMSK |= (1 << PCIE);
  PCMSK |= (1 << INTERRUPT_PIN0); // interrupt for button
  PCMSK |= (1 << INTERRUPT_PIN1); // interrupt for phototrans
  sei();
}

//================================================
void enterSleep()
//================================================
{
  u8x8.setPowerSave(1); // screen powerded down
  sleeping = 1;
  sleep_enable();
  sleep_cpu();
}

//================================================
ISR(PCINT0_vect)
//================================================
{
  //  PINB & (1 << BUTTON);// 1 si relache  a cause du pullup et 0 si appuie

  cli();
  _delay_ms(DEBOUNCE);// for button debounce

  if (sleeping == 0) {// not cause by wake up the screen is powered up
    if (!(PINB & (1 << BUTTON))) { // 1 si relache et 0 si appuie
      page = (page == 0) ? 1 : 0;
    }
  }
  if (sleeping == 1) {// not cause by wake up the screen is powered up
    page = ( PINB & (1 << PHOTOTRANS) ) ? 0 : 1;
  }
  sei();

}


//================================================
void loop()
//================================================
{
  getTemp();
  u8x8.setPowerSave((page == 0) ? 1 : 0); // 1 == shutdown

  if ((millis() - StartTime) > ALARM) {
    enterSleep();// infinit loop to sleep
    u8x8.setPowerSave(0); // powered up the screen
    sleeping = 0;
    StartTime = millis();
  }
}


//================================================
char *printfloat2char( float value )
//================================================
{
  static char txt[] = " 20.9";// ou  "100.9" ou "-20.9";
  float value_l = abs(value);

  byte vv = (byte)(value_l); //sans l'arrondi
  if (value < 0)  {
    txt[0] = 45; // pour le signe moins
  }
  else {
    txt[0] = ( (vv >= 100) ? (vv / 100 + 48) : 32); // 32 pour le car espace ou les centaines
  }

  txt[1] = ( (vv >= 10) ? ((vv % 100) / 10 + 48) : 32); // les dizaines
  txt[2] = vv % 10 + 48; // les unites
  // le txt[3] est le point
  txt[4] = (byte)(10 * (value_l - 100 * (byte)(value_l / 100) - (byte)(value_l) % 100)) + 48; // la decimale pour les nb infeieurs a 100
  return txt;
}


//================================================
void getTemp()
//================================================
{
  int16_t LowByte, HighByte, SignBit, TReading;
  float temp;
  byte data[9];

  oneWire.reset();       // reset one wire bus
  oneWire.skip();

  oneWire.write(0x44);   // start conversion and wait end mesure
  delay(700);// laisse letemps de mesurer

  oneWire.reset();
  oneWire.skip();
  oneWire.write(0xBE);   // Read Scratchpad
  for ( byte i = 0; i < 9; i++) {     // 9 bytes
    data[i] = oneWire.read();
  }

  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) | LowByte;
  SignBit = TReading & 0x8000;

  if (SignBit) { // negative
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }

  temp = ((float)TReading) / 16.0; // /16.0;
  u8x8.draw1x2String(2, 0, printfloat2char(temp));
}

