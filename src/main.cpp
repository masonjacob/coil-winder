#include <Arduino.h>


#define test 27 //PB1

void setup() {
  // put your setup code here, to run once:
  pinMode(test, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(test, HIGH);
  delay(5000);
  digitalWrite(test, LOW);
  delay(5000);
}