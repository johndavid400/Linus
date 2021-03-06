// Linus the Line-bot
// Follows a Black line on a White surface (poster-board and electrical tape).
// Code by JDW 2014 – feel free to modify.

int left_motor = 9;
int right_motor = 10;

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
  // declare motor output pins
  pinMode(left_motor, OUTPUT);
  pinMode(right_motor, OUTPUT);
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
  digitalWrite(left_motor, HIGH);
}

void left_motor_stop(){
  digitalWrite(left_motor, LOW);
}

void right_motor_forward(){
  digitalWrite(right_motor, HIGH);
}

void right_motor_stop(){
  digitalWrite(right_motor, LOW);
}


