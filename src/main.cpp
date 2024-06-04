#include <Arduino.h>

#include <AccelStepper.h>

#define UNO

#ifdef STM32F103
#define COIL_ROTATION_MOTOR_STEP_PIN 27 //PB1
#define COIL_ROTATION_MOTOR_DIR_PIN 28 //PB0
#define SPOOL_POSITION_MOTOR_STEP_PIN 29 //PB5
#define SPOOL_POSITION_MOTOR_DIR_PIN 30 //PB4
#endif
#ifdef UNO
#define COIL_ROTATION_MOTOR_STEP_PIN 13
#define COIL_ROTATION_MOTOR_DIR_PIN 12
#define SPOOL_POSITION_MOTOR_STEP_PIN 11
#define SPOOL_POSITION_MOTOR_DIR_PIN 10
#endif

void 


// #define test 27 //PB1

// void setup() {
//   // put your setup code here, to run once:
//   pinMode(test, OUTPUT);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   digitalWrite(test, HIGH);
//   delay(5000);
//   digitalWrite(test, LOW);
//   delay(5000);
// }

// arduino uno:

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}


// Define some steppers and the pins the will use
AccelStepper stepper1; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepper2(AccelStepper::FULL4WIRE, 6, 7, 8, 9);
AccelStepper stepper3(AccelStepper::FULL2WIRE, 10, 11);

void setup()
{  
    stepper1.setMaxSpeed(200.0);
    stepper1.setAcceleration(100.0);
    stepper1.moveTo(24);
    
    stepper2.setMaxSpeed(300.0);
    stepper2.setAcceleration(100.0);
    stepper2.moveTo(1000000);
    
    stepper3.setMaxSpeed(300.0);
    stepper3.setAcceleration(100.0);
    stepper3.moveTo(1000000); 
}

void loop()
{
    // Change direction at the limits
    if (stepper1.distanceToGo() == 0)
	stepper1.moveTo(-stepper1.currentPosition());
    stepper1.run();
    stepper2.run();
    stepper3.run();
}