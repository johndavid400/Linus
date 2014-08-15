// Linus the Line-bot
// Follows a Black line on a White surface (poster-board and electrical tape).
// Code by JDW 2014 – feel free to modify.

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

// Create variables for sensor readings
int left = 0;
int center = 0;
int right = 0;

// value to define a threshold for whether reading white or black
int threshold = 512;

int debug_pin = 2;
boolean debug = false;

void setup(){
  Serial.begin(9600); // start serial monitor to see sensor readings

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz

  // Set the speed to start, from 0 (off) to 255 (max speed)
  leftMotor->setSpeed(255);
  rightMotor->setSpeed(255);
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  // turn on motor
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);

  // declare debug_pin as an INPUT
  pinMode(debug_pin, INPUT);
  // write enable pull-up resistor by writing D2 input pin HIGH
  digitalWrite(debug_pin, HIGH);
  // check D2 and if grounded, enable debugging mode
  if (digitalRead(debug_pin) == LOW) {
    debug = true;
  }
}

void update_sensors(){
  // this function simply reads the 3 infrared sensor values into variables
  // sensors are attached to Analog pins A0, A1, and A2
  left = analogRead(0);
  center = analogRead(1);
  right = analogRead(2);
}

void loop(){
  uint8_t i;

  // first, read the sensors
  update_sensors();

  // next, check the value of the center sensor to see if it is reading the black tape
  if (center < threshold){
    // if center sensor value is below threshold (ie. above the black tape), check surrounding sensors
    if (left > threshold && right > threshold){
      // if center is the only one reading black tape, drive forward
      left_motor_forward();
      right_motor_forward();
    }
    // the bot will stop when all sensors are above the black tape.
    else if (left < threshold && center < threshold && right < threshold){
      //  if all sensors are reading black, stop Linus.
      left_motor_stop();
      right_motor_stop();
    }
  }
  else {
    // if the center sensor is not reading the black tape, then check the left sensor
    if (left < threshold){
      // if the left sensor is reading the tape, we need to turn back to the left (ie. stop the left motor)
      left_motor_stop();
      right_motor_forward();
    }
    // then check the right sensor
    else if (right < threshold){
      // if the right sensor is reading the tape, we need to turn back to the right (ie. stop the right motor)
      left_motor_forward();
      right_motor_stop();
    }
  }
  if (debug){
    // if pin D2 was grounded when Linus was powered on, then write the sensor values to the serial monitor for debugging.
    serial_print_stuff();
  }
}

void serial_print_stuff(){
  // print each sensor value (from 0-1023) to the serial monitor
  // a low value indicates that the sensor is above the black tape
  Serial.print(left);
  Serial.print("   ");
  Serial.print(center);
  Serial.print("   ");
  Serial.println(right);
}

void left_motor_forward(){
  leftMotor->run(FORWARD);
}

void left_motor_stop(){
  leftMotor->run(RELEASE);
}

void right_motor_forward(){
  rightMotor->run(FORWARD);
}

void right_motor_stop(){
  rightMotor->run(RELEASE);
}


