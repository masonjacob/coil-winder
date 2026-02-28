#include <Arduino.h>
#include <RotaryEncoder.h>
#include <AccelStepper.h>
#include <LcdMenu.h>
#include <interface/LiquidCrystalAdapter.h>
#include <utils/commandProccesors.h>

#define NANO

#ifdef STM32F103
#define COIL_ROTATION_MOTOR_STEP_PIN 27  // PB1
#define COIL_ROTATION_MOTOR_DIR_PIN 28   // PB0
#define SPOOL_POSITION_MOTOR_STEP_PIN 29 // PB5
#define SPOOL_POSITION_MOTOR_DIR_PIN 30  // PB4
#endif
#ifdef NANO

#define COIL_ROTATION_MOTOR_STEP_PIN 6
#define COIL_ROTATION_MOTOR_DIR_PIN 5
#define COIL_ROTATION_MOTOR_NUM_STEPS 200
#define COIL_ROTATION_MOTOR_MICROSTEPS 1

#define SPOOL_POSITION_MOTOR_STEP_PIN 7
#define SPOOL_POSITION_MOTOR_DIR_PIN 8
#define SPOOL_POSITION_MOTOR_NUM_STEPS 200
#define SPOOL_POSITION_MOTOR_MICROSTEPS 32

#define ROTARY_ENCODER_CLK_PIN 2
#define ROTARY_ENCODER_DT_PIN 3
#define ROTARY_ENCODER_SW_PIN 4

#define LCD_ROWS 2
#define LCD_COLS 16
#define LCD_V0_PIN 11
#define LCD_RS_PIN 15 // A1
#define LCD_RW_PIN 16 // A2
#define LCD_E_PIN 17  // A3
#define LCD_D4_PIN 18 // A4
#define LCD_D5_PIN 19 // A5
#define LCD_D6_PIN 10
#define LCD_D7_PIN 9
#endif

#define STEPS_PER_REVOLUTION 200
#define WIRE_DIAMETER 0.1143 // mm (0.0045 in)
#define COIL_DIAMETER 73.0   // mm
#define LEAD 8               // mm
#define COIL_ROTATION_GEAR_RATIO 2.5
#define BASE_SPEED 100
#define MIN_ENCODER -1
#define MAX_ENCODER 4

int encoderPosition = 0;
double spoolPositionRotationRatio = (WIRE_DIAMETER / LEAD);
bool motorsEnabled = true;
bool menuOpened = false;
int numberOfRotations = 0;
int numberOfSteps = 0;

void encoderClicked();
void encoderPositionChanged(int position);
void initializeInfoScreen();
void updateInfoScreen();
void updateSteps();

AccelStepper coil_rotation_driver(AccelStepper::DRIVER, COIL_ROTATION_MOTOR_STEP_PIN, COIL_ROTATION_MOTOR_DIR_PIN);

AccelStepper spool_position_driver(AccelStepper::DRIVER, SPOOL_POSITION_MOTOR_STEP_PIN, SPOOL_POSITION_MOTOR_DIR_PIN);

LiquidCrystalAdapter lcdAdapter(LCD_RS_PIN, LCD_E_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN, LCD_COLS, LCD_ROWS);

LcdMenu menu(lcdAdapter);

RotaryEncoder encoder(ROTARY_ENCODER_DT_PIN, ROTARY_ENCODER_CLK_PIN, ROTARY_ENCODER_SW_PIN, &encoderClicked, &encoderPositionChanged);

void encoderClicked()
{
  numberOfRotations = 0;
  updateInfoScreen();
  // if (!menuOpened)
  // {
  // }
  // if (speed == 0)
  // {
  //   speed = BASE_SPEED;
  //   coil_rotation_driver.setSpeed(speed * encoderPosition * COIL_ROTATION_GEAR_RATIO);
  //   spool_position_driver.setSpeed(speed * encoderPosition * spoolPositionRotationRatio);
  // }
  // else
  // {
  //   speed = 0;
  //   coil_rotation_driver.setSpeed(speed);
  //   spool_position_driver.setSpeed(speed);
  // }
}

void encoderPositionChanged(int position)
{
  if (position < MIN_ENCODER)
  {
    encoderPosition = MIN_ENCODER;
  }
  else if (position > MAX_ENCODER)
  {
    encoderPosition = MAX_ENCODER;
  }
  else
  {
    encoderPosition = position;
  }
  coil_rotation_driver.setSpeed(BASE_SPEED * encoderPosition * COIL_ROTATION_GEAR_RATIO * COIL_ROTATION_MOTOR_MICROSTEPS);
  spool_position_driver.setSpeed(BASE_SPEED * encoderPosition * spoolPositionRotationRatio * SPOOL_POSITION_MOTOR_MICROSTEPS);
  // Serial.print("Coil Rotation Speed: ");
  // Serial.print(coil_rotation_driver.speed());
  // Serial.print(" Spool Position Speed: ");
  // Serial.println(spool_position_driver.speed());

  if (!menuOpened)
  {
    updateInfoScreen();
  }
}

void updateSteps()
{
  numberOfSteps++;
  if (numberOfSteps == STEPS_PER_REVOLUTION * COIL_ROTATION_GEAR_RATIO)
  {
    numberOfRotations++;
    numberOfSteps = 0;
    updateInfoScreen();
  }
}

void initializeInfoScreen()
{
  lcdAdapter.lcd.clear();
  lcdAdapter.lcd.setCursor(0, 0);
  lcdAdapter.lcd.print("Speed:");
  lcdAdapter.lcd.setCursor(6, 0);
  lcdAdapter.lcd.print("    ");
  lcdAdapter.lcd.setCursor(7, 0);
  lcdAdapter.lcd.print(encoderPosition);
  lcdAdapter.lcd.setCursor(0, 1);
  lcdAdapter.lcd.print("Revolutions:");
  lcdAdapter.lcd.setCursor(12, 1);
  lcdAdapter.lcd.print("    ");
  lcdAdapter.lcd.setCursor(13, 1);
  lcdAdapter.lcd.print(numberOfRotations);
}

void updateInfoScreen()
{
  lcdAdapter.lcd.setCursor(6, 0);
  lcdAdapter.lcd.print("    ");
  lcdAdapter.lcd.setCursor(7, 0);
  lcdAdapter.lcd.print(encoderPosition);
  lcdAdapter.lcd.setCursor(12, 1);
  lcdAdapter.lcd.print("    ");
  lcdAdapter.lcd.setCursor(13, 1);
  lcdAdapter.lcd.print(numberOfRotations);
}

void setup()
{
  pinMode(LCD_RW_PIN, OUTPUT);
  digitalWrite(LCD_RW_PIN, LOW);
  pinMode(LCD_V0_PIN, OUTPUT);
  analogWrite(LCD_V0_PIN, 50);
  lcdAdapter.begin();
  initializeInfoScreen();

  encoder.begin();

  coil_rotation_driver.setMaxSpeed(1000);
  coil_rotation_driver.setSpeed(0);
  coil_rotation_driver.setAcceleration(50);
  spool_position_driver.setMaxSpeed(1000);
  spool_position_driver.setSpeed(0);
  spool_position_driver.setAcceleration(50);
}

void loop()
{
  encoder.tick();
  if (motorsEnabled)
  {
    bool stepped = coil_rotation_driver.runSpeed();
    spool_position_driver.runSpeed();
    if (stepped)
    {
      updateSteps();
    }
  }
};