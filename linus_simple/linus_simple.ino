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
  left = analogRead(0);
  center = analogRead(1);
  right = analogRead(2);
}

void loop(){

  update_sensors();

  // first, check the value of the center sensor
  if (center < threshold){
    // if center sensor value is below threshold, check surrounding sensors
    if (left > threshold && right > threshold){
      // if center is the only one reading black tape, drive forward
      left_motor_forward();
      right_motor_forward();
    }
    // you want the bot to stop when it reaches the black box.
    else if (left < threshold && center < threshold && right < threshold){
      //  if all sensors are reading black, stop Linus.
      left_motor_stop();
      right_motor_stop();
    }
  }
  else {
    // first check the left sensor
    if (left < threshold){
      left_motor_stop();
      right_motor_forward();
    }
    // then check the right sensor
    else if (right < threshold){
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


