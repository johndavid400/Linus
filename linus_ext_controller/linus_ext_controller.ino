// Linus the Line-bot
// Follows a Black line on a White surface (poster-board and electrical tape).
// Code by JDW 2014 – feel free to modify.

// motor pins when using the Arduino Motor Shield R3
int left_motor_dir = 12;
int left_motor_pwm = 3;
int right_motor_pwm = 11;
int right_motor_dir = 13;

// create variables for sensor readings
int left = 0;
int left_center = 0;
int center = 0;
int right_center= 0;
int right = 0;

// analog input pins for IR sensors
int left_pin = 1;
int left_center_pin = 2;
int center_pin = 3;
int right_center_pin = 4;
int right_pin = 5;

// value to define a threshold for whether reading white or black
// this is an analog value from 0-1023, where 0 would be solid black and 1023 would be solid white.
int threshold = 512;

// set a default speed value of 64 out of 255. Basically 1/4 of full speed.
int speed_value = 64;

// analog pin to connect potentiometer used for speed control
int speed_pin = 0;

// set use speed potentiometer to false by default
// set to true if you want to use a potentiometer connected to pin A0 to set the speed
boolean use_speed_pot = false;

// use pin 2 to allow user to put into debug mode by grounding this pin during startup.
// debug mode prints IR sensor values to the serial monitor
int debug_pin = 2;

// set debug mode to false by default
boolean debug = false;


void setup(){
  Serial.begin(9600); // start serial monitor to see sensor readings
  // declare left motor
  pinMode(left_motor_pwm, OUTPUT);
  pinMode(left_motor_dir, OUTPUT);
  // declare right motor
  pinMode(right_motor_pwm, OUTPUT);
  pinMode(right_motor_dir, OUTPUT);
  // Ground pin 2 at startup to go into debug mode
  pinMode(debug_pin, INPUT);
  digitalWrite(debug_pin, HIGH);
  
  check_debug();
}

void check_debug(){
  if (digitalRead(debug_pin) == LOW){
    // if debug_pin is grounded during startup, print IR values to serial monitor
    debug = true;
  }
}

void update_sensors(){
  // read values for each IR sensor
  left = analogRead(left_pin);
  left_center = analogRead(left_center_pin);
  center = analogRead(center_pin);
  right_center = analogRead(right_center_pin);
  right = analogRead(right_pin);
  
  if (use_speed_pot){
    // check value for speed potentiometer
    speed_value = analogRead(speed_pin) / 4;
  }
}

void loop(){

  update_sensors();

  // first, check the value of the center sensor
  if (center < threshold){
    // if center sensor value is below threshold (on the black line), check surrounding sensors
    if (left_center > threshold && right_center > threshold){
      // if center is the only one reading the black line, drive forward
      left_motor_forward(speed_value);
      right_motor_forward(speed_value);
    }
    else if (left < threshold && left_center < threshold && center < threshold && right_center < threshold && right < threshold){
      //  if all sensors are reading black, stop Linus.
      left_motor_stop();
      right_motor_stop();
    }
  }
  else {
    if (left < threshold || left_center < threshold){
      // if either left sensor is reading the black line, stop the left motor and drive the right motor
      left_motor_stop();
      right_motor_forward(speed_value);
    }
    else if (right < threshold || right_center < threshold){
      // if either right sensor is reading the black line, stop the right motor and drive the left motor
      left_motor_forward(speed_value);
      right_motor_stop();
    }
  }
  if (debug){
    // if you grounded the debug_pin (D2) during startup, print serial values
    serial_print_stuff();
  }
}

void serial_print_stuff(){
  // print each IR sensor value from left to right
  Serial.print(left);
  Serial.print("   ");
  Serial.print(left_center);
  Serial.print("   ");
  Serial.print(center);
  Serial.print("   ");
  Serial.print(right_center);
  Serial.print("   ");
  Serial.println(right);
}

void left_motor_forward(int spd){
  digitalWrite(left_motor_dir, LOW);
  analogWrite(left_motor_pwm, spd);
}

void left_motor_reverse(int spd){
  digitalWrite(left_motor_dir, HIGH);
  analogWrite(left_motor_pwm, spd);
}

void left_motor_stop(){
  digitalWrite(left_motor_pwm, LOW);
}

void right_motor_forward(int spd){
  digitalWrite(right_motor_dir, LOW);
  analogWrite(right_motor_pwm, spd);
}

void right_motor_reverse(int spd){
  digitalWrite(right_motor_dir, HIGH);
  analogWrite(right_motor_pwm, spd);
}

void right_motor_stop(){
  digitalWrite(right_motor_pwm, LOW);
}


