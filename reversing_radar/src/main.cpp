#include <Arduino.h>

// AJ-SR04M
#ifdef ARDUINO_ARCH_ESP8266
int trig_pin = D6; // Trig Pin
int echo_pin = D7; // Echo Pin
#elif ARDUINO_ARCH_AVR
int trig_pin = PD6; // Trig Pin
int echo_pin = PD7; // Echo Pin
#endif

// Passive Buzzer
#ifdef ARDUINO_ARCH_ESP8266
int buzzer_pin = D4;
#elif ARDUINO_ARCH_AVR
int buzzer_pin = 13;
#endif

#define MAX_DISTANCE_CM 150
#define MIN_DISTANCE_CM 20

void buzz_100ms() {
  for (int i = 0; i < 50; i++) {
    digitalWrite(buzzer_pin, HIGH);
    delay(1);
    digitalWrite(buzzer_pin, LOW);
    delay(1);
  }
}

void buzz_and_delay_by(unsigned long distance) {
  if (distance > MAX_DISTANCE_CM) {
    delay(1000);
    return;
  }

  buzz_100ms();
  if (distance > MIN_DISTANCE_CM) {
    delay((distance - MIN_DISTANCE_CM) * 2);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  pinMode(buzzer_pin, OUTPUT);

  // Check is everything OK
  buzz_100ms();
  delay(500);
  buzz_100ms();
  delay(500);
  buzz_100ms();
}

unsigned long get_duration() {
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(5);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  return pulseIn(echo_pin, HIGH);
}

void loop() {
  unsigned long duration, distance;

  duration = get_duration();

  // Convert from duration to distance
  distance = (unsigned long)(((double)duration / 2) / 29.1);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");

  buzz_and_delay_by(distance);
}
