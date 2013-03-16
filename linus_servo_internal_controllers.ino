// Linus the Line-bot
// Follows a Black line on a White surface (poster-board and electrical tape).
// Code by JDW 2013 – feel free to modify.

// This code is different from what is in my book. It is intended for use with continuous rotation servo motors that still have the internal motor-controllers in place. This code does NOT use the Adafruit motor-shield, instead the motor control wires shoudl be connected to pins D9 and D10.

// Create variables for sensor readings

int sensor1 = 0;
int sensor2 = 0;
int sensor3 = 0;
int sensor4 = 0;
int sensor5 = 0;

// Create variables for adjusted readings

int adj_1 = 0;
int adj_2 = 0;
int adj_3 = 0;
int adj_4 = 0;
int adj_5 = 0;


// You can change the min/max values below to fine tune each sensor on your bot

int s1_min = 200;
int s1_max = 950;

int s2_min = 200;
int s2_max = 950;

int s3_min = 200;
int s3_max = 950;

int s4_min = 200;
int s4_max = 950;

int s5_min = 200;
int s5_max = 950;

// this threshold defines when the sensor is reading the black line
int lower_threshold = 20;

// value to define a middle threshold (half of the total 255 value range)
int threshold = 128;

// this threshold defines when the sensor is reading the white poster board
int upper_threshold = 230;

// this value sets the maximum speed of linus (255 = max). 
// using a speed potentiometer will over-ride this setting.

int speed_value = 512;
int speed_pot;

// declare motor pins
int motor_left = 9;
int motor_right = 10;
int stop_pulse = 1500;

// end of changeable variables

void setup()
{
  Serial.begin(9600); // start serial monitor to see sensor readings

  // declare motor pins as outputs
  pinMode(motor_left, OUTPUT);
  pinMode(motor_right, OUTPUT);

}

void update_sensors(){

  // this will read sensor 1
  sensor1 = analogRead(0);
  adj_1 = map(sensor1, s1_min, s1_max, 0, 255);
  adj_1 = constrain(adj_1, 0, 255);

  // this will read sensor 2
  sensor2 = analogRead(1);     //  sensor 2 = left-center
  adj_2 = map(sensor2, s2_min, s2_max, 0, 255);
  adj_2 = constrain(adj_2, 0, 255);

  // this will read sensor 3
  sensor3 = analogRead(2);         // sensor 3 = center
  adj_3 = map(sensor3, s3_min, s3_max, 0, 255);
  adj_3 = constrain(adj_3, 0, 255);

  // this will read sensor 4
  sensor4 = analogRead(3);     //  sensor 4 = right-center
  adj_4 = map(sensor4, s4_min, s4_max, 0, 255);
  adj_4 = constrain(adj_4, 0, 255);

  // this will read sensor 5
  sensor5 = analogRead(4);  // sensor 5 = right
  adj_5 = map(sensor5, s5_min, s5_max, 0, 255);
  adj_5 = constrain(adj_5, 0, 255);

  // check value for speed potentiometer if present  
  speed_pot = analogRead(5) / 2;

}

void loop(){
  update_sensors(); // update sensors 
  speed_value = speed_pot;  // Leave commented out, unless using potentiometer 
  // first, check the value of the center sensor
  if (adj_3 < lower_threshold){
    // if center sensor value is below threshold, check surrounding sensors
    if (adj_2 > threshold && adj_4 > threshold){
      // if all sensors check out, drive forward	
      motor_left_forward();
      motor_right_forward();
    }
    // you want the bot to stop when it reaches the black box.
    else if (adj_1 < 1){
      if (adj_2 < 1){
        if (adj_3 < 1){
          if (adj_4 < 1){
            if (adj_5 < 1){
              //  if all sensors are reading black, stop Linus.
              motor_left_stop();
              motor_right_stop();
            }
          }
        }
      }
    }
  }
  // otherwise, the center sensor is above the threshold
  // so we need to check what sensor is above the black line 
  else {
    // first check sensor 1
    if (adj_1 < upper_threshold && adj_5 > upper_threshold){
      motor_left_stop();
      motor_right_forward();
    }
    // then check sensor 5
    else if (adj_1 > upper_threshold && adj_5 < upper_threshold){
      motor_left_forward();
      motor_right_stop();
    }

    // if not sensor 1 or 5, then check sensor 2
    else if (adj_2 < upper_threshold && adj_4 > upper_threshold){
      motor_left_stop();
      motor_right_forward();
    }

    // if not sensor 2, then check sensor 4
    else if (adj_2 > upper_threshold && adj_4 < upper_threshold){
      motor_left_forward();
      motor_right_stop();
    }
  }

  ///// Print values for each sensor
  /////sensor 1 values
  Serial.print("sensor 1:  ");
  Serial.print(sensor1);
  Serial.print("  -  ");

  Serial.print("Adj 1:  ");
  Serial.print(adj_1);
  Serial.print("  -  ");

  /////sensor 2 values
  Serial.print("sensor 2:  ");
  Serial.print(sensor2);
  Serial.print("  -  ");

  Serial.print("Adj 2:  ");
  Serial.print(adj_2);
  Serial.print("  -  ");

  /////sensor 3 values
  Serial.print("sensor 3:  ");
  Serial.print(sensor3);
  Serial.print("  -  ");

  Serial.print("Adj 3:  ");
  Serial.print(adj_3);
  Serial.print("  -  ");

  /////sensor 4 values
  Serial.print("sensor 4:  ");
  Serial.print(sensor4);
  Serial.print("  -  ");

  Serial.print("Adj 4:  ");
  Serial.print(adj_4);
  Serial.print("  -  ");

  /////sensor 5 values
  Serial.print("sensor 5:  ");
  Serial.print(sensor5);
  Serial.print("  -  ");

  Serial.print("Adj 5:  ");
  Serial.print(adj_5);
  Serial.print(" ");

  Serial.print("speed:  ");
  Serial.print(speed_pot);
  Serial.println(" ");
}

// functions for motors

void motor_left_forward(){
  digitalWrite(door_motor_pin, HIGH);
  delayMicroseconds(stop_pulse + motor_speed);
  digitalWrite(door_motor_pin, LOW);
}

void motor_left_stop(){
  digitalWrite(door_motor_pin, HIGH);
  delayMicroseconds(stop_pulse);
  digitalWrite(door_motor_pin, LOW);
}

void motor_right_forward(){
  digitalWrite(door_motor_pin, HIGH);
  delayMicroseconds(stop_pulse + motor_speed);
  digitalWrite(door_motor_pin, LOW);
}

void motor_right_stop(){
  digitalWrite(door_motor_pin, HIGH);
  delayMicroseconds(stop_pulse);
  digitalWrite(door_motor_pin, LOW);
}

// end of code

