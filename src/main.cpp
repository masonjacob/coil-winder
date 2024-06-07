#include <Arduino.h>
#include "RotaryEncoder.h"
#include <AccelStepper.h>

#define UNO

#ifdef STM32F103
#define COIL_ROTATION_MOTOR_STEP_PIN 27 //PB1
#define COIL_ROTATION_MOTOR_DIR_PIN 28 //PB0
#define SPOOL_POSITION_MOTOR_STEP_PIN 29 //PB5
#define SPOOL_POSITION_MOTOR_DIR_PIN 30 //PB4
#endif
#ifdef UNO
#define COIL_ROTATION_MOTOR_STEP_PIN 8
#define COIL_ROTATION_MOTOR_DIR_PIN 9
#define SPOOL_POSITION_MOTOR_STEP_PIN 10
#define SPOOL_POSITION_MOTOR_DIR_PIN 11
#define ROTARY_ENCODER_CLK_PIN 2
#define ROTARY_ENCODER_DT_PIN 3
#define ROTARY_ENCODER_SW_PIN 4
#endif

#define STEPS_PER_REVOLUTION 200
#define WIRE_DIAMETER 0.11176 //mm (0.0044 in)
#define COIL_DIAMETER 73.0 //mm
#define LEAD 8 //mm
#define COIL_GEAR_RATIO 20/50
#define MAX_ENCODER_POSITION 50.0
#define ENCODER_TO_SPEED_RATIO 5


int EncoderPosition = 0; 
unsigned long coilPreviousTime = 0;
unsigned long coilDelay = 0;
unsigned long spoolPreviousTime = 0;
unsigned long spoolDelay = 0;
unsigned long printPreviousTime = 0;
unsigned long delayRatio = LEAD * (1/WIRE_DIAMETER); // delay ratio between the two motors, the spool spins slower, so the delay is greator
void RotaryChanged(); //we need to declare the func above so Rotary goes to the one below

RotaryEncoder Rotary(&RotaryChanged, ROTARY_ENCODER_DT_PIN, ROTARY_ENCODER_CLK_PIN, ROTARY_ENCODER_SW_PIN); // Pins 2 (DT), 3 (CLK), 4 (SW)

void RotaryChanged()
{
  const unsigned int state = Rotary.GetState();
  
  if (state & DIR_CW)  
    EncoderPosition++;
    
  if (state & DIR_CCW)  
    EncoderPosition--;    
  
  if (EncoderPosition < 0)
    EncoderPosition = 0;

  if (EncoderPosition > MAX_ENCODER_POSITION)
    EncoderPosition = MAX_ENCODER_POSITION;

  coilDelay =  1e6 / (EncoderPosition * ENCODER_TO_SPEED_RATIO);
  spoolDelay = coilDelay * delayRatio;
}


void setup() {
  pinMode(COIL_ROTATION_MOTOR_STEP_PIN, OUTPUT);
  pinMode(COIL_ROTATION_MOTOR_DIR_PIN, OUTPUT);
  pinMode(SPOOL_POSITION_MOTOR_STEP_PIN, OUTPUT);
  pinMode(SPOOL_POSITION_MOTOR_DIR_PIN, OUTPUT);

  digitalWrite(COIL_ROTATION_MOTOR_DIR_PIN, HIGH);
  digitalWrite(SPOOL_POSITION_MOTOR_DIR_PIN, HIGH);

  Rotary.setup();  
  Serial.begin(9600);  
  Serial.println("Rotary Encoder Tests"); 
}

void loop() {
  if (coilDelay > 0 && spoolDelay > 0) {
    if (micros() - coilPreviousTime > coilDelay) {
      digitalWrite(COIL_ROTATION_MOTOR_STEP_PIN, HIGH);
      delayMicroseconds(1);
      digitalWrite(COIL_ROTATION_MOTOR_STEP_PIN, LOW);
      coilPreviousTime = micros();
    }
    if (micros() - spoolPreviousTime > spoolDelay) {
      digitalWrite(SPOOL_POSITION_MOTOR_STEP_PIN, HIGH);
      delayMicroseconds(1);
      digitalWrite(SPOOL_POSITION_MOTOR_STEP_PIN, LOW);
      spoolPreviousTime = micros();
    }
  }
  else {
    digitalWrite(COIL_ROTATION_MOTOR_STEP_PIN, LOW);
    digitalWrite(SPOOL_POSITION_MOTOR_STEP_PIN, LOW);
  }
  if (micros() - printPreviousTime > 1e6) {
    Serial.println("Encoder Position: " + String(EncoderPosition));
    Serial.println("Coil Delay: " + String(coilDelay) + " Spool Delay: " + String(spoolDelay));
    printPreviousTime = micros();
  }
}


// // Define some steppers and the pins the will use
// AccelStepper stepper1; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
// AccelStepper stepper2(AccelStepper::FULL4WIRE, 6, 7, 8, 9);
// AccelStepper stepper3(AccelStepper::FULL2WIRE, 10, 11);

// void setup()
// {  
//     stepper1.setMaxSpeed(200.0);
//     stepper1.setAcceleration(100.0);
//     stepper1.moveTo(24);
    
//     stepper2.setMaxSpeed(300.0);
//     stepper2.setAcceleration(100.0);
//     stepper2.moveTo(1000000);
    
//     stepper3.setMaxSpeed(300.0);
//     stepper3.setAcceleration(100.0);
//     stepper3.moveTo(1000000); 
// }

// void loop()
// {
//     // Change direction at the limits
//     if (stepper1.distanceToGo() == 0)
// 	stepper1.moveTo(-stepper1.currentPosition());
//     stepper1.run();
//     stepper2.run();
//     stepper3.run();
// }