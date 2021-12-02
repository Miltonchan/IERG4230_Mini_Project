/*
  IERG4230 IoT Testing Program
  Capacitive Touch Sensor
  Matter Needing attention:
  On Boot/Reset/wakeup,
  GPIO15(D8) keep LOW and GPIO2(D4)keep HIGH
  GPIO0(D3) HIGH = Run mode / LOW = Flash mode
  On Board LEDs (active LOW):
    GPIO16(D0)
    GPIO2(D4)

 Connections:
 Buzzer       LaunchPad
 ---------    ---------
 VCC          3.3V
 I/O          GPIO 14 (D5)
 GND          GND

 TTP223       ESP8266-12E
 ---------    ---------
 SIG          GPIO 12 (D6)
 VCC          3.3V
 GND          GND
*/
#include "Buzzer.h"

Buzzer buzzer(14); // (Buzzer pin,LED pin)

// Capacitive Touch Sensor
int  inputPin  = 12;  // GPIO 12(D6)
int  led       = 16;  // On board LED GPIO 16(D0)
bool detect    = false;

void setup()
{
  Serial.begin(115200);
  Serial.println("ESP8266-12E/F Buzzer testing program\n");
  Serial.println("Build-in LED at GPIO-16(D0)");
  Serial.println("Buzzer pin at GPIO-14(D5)");
  Serial.println("Capacitive Touch Sensor Pin at GPIO-12(D6)");
  pinMode (inputPin, INPUT);
  pinMode (led, OUTPUT);
}

void loop()
{
  detect = digitalRead(inputPin); // touch = High

  if (detect == true) {
    digitalWrite(led, LOW); // turn ON on board LED GPIO 16(D0)
    Serial.println("Key pressed");
    Serial.println("Buzzer play");
    buzzer.begin(0);
    /* Method - It creates a "normal distortion" effect on the Buzzer */
    //    buzzer.distortion(NOTE_C3, NOTE_C5);
    //    buzzer.distortion(NOTE_C5, NOTE_C3);

    /* Method - It makes the Buzzer sound */
    buzzer.sound(NOTE_A3, 100); // (Note, duration)
    buzzer.end(100);
  }
  else {
    digitalWrite(led, HIGH);
    Serial.println("No Key pressed");
  }
}
