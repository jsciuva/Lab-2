#include <Arduino.h>

#define RED_LED_PIN 25
#define YEL_LED_PIN 26
#define GRN_LED_PIN 27

#define BUZZER_PIN 2
#define BUTTON_PIN 15

// button struct to use with interrupt isr function
struct Button {
  const uint8_t PIN;
  bool pressed;
};

// Button object for use with isa function 
Button butt = {BUTTON_PIN, false};

// interrupt function for use with button
void IRAM_ATTR isr() {
  butt.pressed = true;
}

// for sounding the buzzer 
void redBuzzCycle(int pin, int cycles);
void grnBuzzCycle(int pin, int cycles);

void setup() {
  // Serial.begin(9600);
  
  pinMode(butt.PIN, INPUT_PULLUP); // use internal pullup resistor for button
  // use interrupt for button
  attachInterrupt(butt.PIN, isr, FALLING); // button connected to gnd
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YEL_LED_PIN, OUTPUT);
  pinMode(GRN_LED_PIN, OUTPUT);
  
  
}

void loop() {
  // reset the button from previous loop
  butt.pressed = false;

  // go through sequence from red -> red/yellow -> green
  digitalWrite(RED_LED_PIN, HIGH);
  // Buzzer sound alternates between on and off for 8 sec
  redBuzzCycle(BUZZER_PIN, 8);
  digitalWrite(YEL_LED_PIN, HIGH);
  // Buzzer sound for 2 more sec while yel/red
  redBuzzCycle(BUZZER_PIN, 2);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YEL_LED_PIN, LOW);
  digitalWrite(GRN_LED_PIN, HIGH);

  // when green, wait for button input and buzz
  while(!butt.pressed) {
    grnBuzzCycle(BUZZER_PIN, 1);
  }
  // stay green for 5 seconds after button pushed
  grnBuzzCycle(BUZZER_PIN, 5);
  // go through sequence from green -> yellow -> red
  digitalWrite(GRN_LED_PIN, LOW);
  digitalWrite(YEL_LED_PIN, HIGH);
  delay(2000);
  digitalWrite(YEL_LED_PIN, LOW);

}

// for sounding buzzer when light is red
void redBuzzCycle(int pin, int seconds = 0) {
  int cycles = seconds / 2;
  for (int i = 0; i < cycles; ++i) {
    tone(pin, 100);
    delay(1500);
    noTone(pin);
    delay(500);
  }
}

// for sounding buzzer when light is green
void grnBuzzCycle(int pin, int seconds = 0) {
  int cycles = seconds / 0.5;
  for (int i = 0; i < cycles; ++i) {
    tone(pin, 500);
    delay(250);
    noTone(pin);
    delay(250);
  }
}