// Linus the Line-bot
// Follows a Black line on a White surface (poster-board and electrical tape).
// Code by JDW 2014 – feel free to modify.

int left_motor_dir = 12;
int left_motor_pwm = 3;
int right_motor_pwm = 11;
int right_motor_dir = 13;

// Create variables for sensor readings
int left = 0;
int left_center = 0;
int center = 0;
int right_center= 0;
int right = 0;

// value to define a threshold for whether reading white or black
int threshold = 512;

int speed_value = 255;
int speed_pot;

boolean debug = false;
boolean use_speed_pot = false;

void setup(){
  Serial.begin(9600); // start serial monitor to see sensor readings
  // declare left motor
  pinMode(left_motor_pwm, OUTPUT);
  pinMode(left_motor_dir, OUTPUT);
  // declare right motor
  pinMode(right_motor_pwm, OUTPUT);
  pinMode(right_motor_dir, OUTPUT);
  // set motors in position to go forward only
  digitalWrite(left_motor_dir, LOW);
  digitalWrite(right_motor_dir, LOW);
}

void update_sensors(){
  left = analogRead(4);
  left_center = analogRead(3);
  center = analogRead(2);
  right_center = analogRead(1);
  right = analogRead(0);
  
  if (use_speed_pot){
    // check value for speed potentiometer
    speed_value = analogRead(5) / 4;
  }
}

void loop(){

  update_sensors();

  // first, check the value of the center sensor
  if (center < threshold){
    // if center sensor value is below threshold, check surrounding sensors
    if (left_center > threshold && right_center > threshold){
      // if center is the only one reading black tape, drive forward
      left_motor_forward();
      right_motor_forward();
    }
    // you want the bot to stop when it reaches the black box.
    else if (left < threshold && left_center < threshold && center < threshold && right_center < threshold && right < threshold){
      //  if all sensors are reading black, stop Linus.
      left_motor_stop();
      right_motor_stop();
    }
  }
  else {
    // first check the left sensors
    if (left < threshold || left_center < threshold){
      left_motor_stop();
      right_motor_forward();
    }
    // then check the right sensors
    else if (right < threshold || right_center < threshold){
      left_motor_forward();
      right_motor_stop();
    }
  }
  if (debug){
    serial_print_stuff();
  }
}

void serial_print_stuff(){
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

void left_motor_forward(){
  analogWrite(left_motor_pwm, speed_value);
}

void left_motor_stop(){
  digitalWrite(left_motor_pwm, LOW);
}

void right_motor_forward(){
  analogWrite(right_motor_pwm, speed_value);
}

void right_motor_stop(){
  digitalWrite(right_motor_pwm, LOW);
}


