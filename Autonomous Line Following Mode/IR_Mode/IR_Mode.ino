#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define IN1 3     // Left Motor forward
#define IN2 5     // Left Motor backward
#define IN3 6     // Right motor forward
#define IN4 9     // Right Motor backward

//Pins of 5 channel IR sensor
#define L2 4   
#define L1 10  
#define middle 8
#define R1 7
#define R2 11

bool sensorValues[5];
char lastMove = 'F';

void setup() {
  // put your setup code here, to run once:
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(L2, INPUT);
  pinMode(L1, INPUT);
  pinMode(middle, INPUT);
  pinMode(R1, INPUT);
  pinMode(R2, INPUT);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("Line Follow Mode");
  readSensors(); 
  if (!sensorValues[4]) {
    turn_right();
  } 
  else if (!sensorValues[3]) {
    right();
  } 
  else if (!sensorValues[0]) {
    turn_left();
  } 
  else if (!sensorValues[1]) {
    left();
  } 
  else if (!sensorValues[2]) {
    forward();
  } 
  else {
    continueLastMove();
  }
}

// Moving forward
void forward() {
  analogWrite(IN1, 150);
  analogWrite(IN2, 0);
  analogWrite(IN3, 150);
  analogWrite(IN4, 0);
  lastMove = 'F';
}

// Turning right
void right() {
  analogWrite(IN1, 75);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
  lastMove = 'R';
}

// Tight turning right
void turn_right() {
  analogWrite(IN1, 75);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 75);
  lastMove = 'R';
}

// Turning left
void left() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 75);
  analogWrite(IN4, 0);
  lastMove = 'L';
}

// Tight turning left
void turn_left() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 75);
  analogWrite(IN3, 75);
  analogWrite(IN4, 0);
  lastMove = 'L';
}

void readSensors() {
  sensorValues[0] = digitalRead(L2);
  sensorValues[1] = digitalRead(L1);
  sensorValues[2] = digitalRead(middle);
  sensorValues[3] = digitalRead(R1);
  sensorValues[4] = digitalRead(R2);

  Serial.print("Sensors: ");
  for (byte i = 0; i < 5; i++) {
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void continueLastMove() {
  if (lastMove == 'L') left();
  else if (lastMove == 'R') right();
  else forward();
}
