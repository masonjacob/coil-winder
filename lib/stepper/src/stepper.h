#include <Arduino.h>

class StepperMotor {
    private:
        int stepsPerRevolution;
        int stepPin;
        int dirPin;

    public:
        StepperMotor(int stepsPerRev, int stepPin, int dirPin) {
            this->stepsPerRevolution = stepsPerRev;
            this->stepPin = stepPin;
            this->dirPin = dirPin;

            pinMode(stepPin, OUTPUT);
            pinMode(dirPin, OUTPUT);
        }

        void step(int numSteps, int delayTime) {
            int stepsRemaining = abs(numSteps);
            int stepDirection = (numSteps > 0) ? HIGH : LOW;

            digitalWrite(dirPin, stepDirection);

            while (stepsRemaining > 0) {
                digitalWrite(stepPin, HIGH);
                delayMicroseconds(delayTime);
                digitalWrite(stepPin, LOW);
                delayMicroseconds(delayTime);

                stepsRemaining--;
            }
        }
};