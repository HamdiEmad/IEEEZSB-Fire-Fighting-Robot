#include <Servo.h>
#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySerial(0, 1); // RX, TX
Servo myServo;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


#define IN1 3     // Left Motor forward
#define IN2 5     // Left Motor backward
#define IN3 6     // Right motor forward
#define IN4 9     // Right Motor backward

#define ir_R A1  // Right flame sensor
#define ir_L A2  // Left flame sensor
#define ir_M A3  // Center flame sensor

#define servo 2
#define pump A0  // Pump pin

//Pins of 5 channel IR sensor
#define L2 11   
#define L1 7  
#define middle 8
#define R1 10
#define R2 4

byte data, angle = 0;
int s1, s2, s3;

bool sensorValues[5];
char lastMove = 'F', Mode = 'S';
const byte threshold = 3;  // To make sure there is a flame 


void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  for (byte i = 0; i < 3; i++)
    pinMode(A1 + i, INPUT);
  pinMode(pump, OUTPUT);
  pinMode(servo, OUTPUT);
  pinMode(L2, INPUT);
  pinMode(L1, INPUT);
  pinMode(middle, INPUT);
  pinMode(R1, INPUT);
  pinMode(R2, INPUT);

  myServo.attach(servo);  // Servo is connected to pin 2
  myServo.write(90);   // Default position of the servo
  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  mySerial.begin(9600);
}

void loop() {
  /* Line follower mode             -> I
     Manual control mode            -> R
     Automatic fire fighting mode   -> A
  */
  if (mySerial.read() == 82)
    Mode = 'R';
  if (mySerial.read() == 73)
    Mode = 'I';
  if (mySerial.read() == 65)
    Mode = 'A';
  Serial.println(Mode);
  switch (Mode) {
    case 'A':
    FA();
    break;
    case 'I':
    IR();
    break;
    case 'R':
    manual();
    break;
    default :
    Serial.println("Else !");
    break;
  }
}

// Moving forward
void forward() {
  analogWrite(IN1, 120);
  analogWrite(IN2, 0);
  analogWrite(IN3, 120);
  analogWrite(IN4, 0);
  lastMove = 'F';
}

// Turning right
void right() {
  analogWrite(IN1, 150);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 150);
  lastMove = 'R';
}

// Turning left
void left() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 150);
  analogWrite(IN3, 150);
  analogWrite(IN4, 0);
  lastMove = 'L';
}

// Taking the reading of the 5 channel IR sensor
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

// Line follower mode
void IR() {
  if (Serial.read() == 65)
    Mode = 'A';
  if (Serial.read() == 82)
    Mode = 'R';
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("Line Follow Mode");
  readSensors();  
  if (sensorValues[1] == 0 || sensorValues[0] == 0) { // L1 - يسار بسيط
    left();
  } 
  else if (sensorValues[3] == 0 || sensorValues[4] == 0) { // R1 - يمين بسيط
    right();
  }  
  else if (sensorValues[2] == 0) { // الأوسط فقط إذا لم يظهر الجانبين
    forward();
  } 
  else {
    // إذا فقد الخط تمامًا، أكمل في نفس الاتجاه الأخير
    continueLastMove();
  }
}

// Manual mode (RC)
void manual() {
  if (Serial.available()) {
    data = Serial.read();
    Serial.println(data);
  }

  if (data == 65) {
    Mode = 'A';
  }
  if (data == 73) {
    Mode = 'I';
  }

  delay(30);

  if (data == 1) {
    analogWrite(IN1, 255);
    analogWrite(IN2, 0);
    analogWrite(IN3, 255);
    analogWrite(IN4, 0);
  }

  else if (data == 2) {
    analogWrite(IN1, 0);
    analogWrite(IN2, 255);
    analogWrite(IN3, 0);
    analogWrite(IN4, 255);
  }
  else if (data == 3) {
    analogWrite(IN1, 255);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
  }
  else if (data == 4) {
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
    analogWrite(IN3, 255);
    analogWrite(IN4, 0);
  }
  else if (data == 5) {
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
  }
  else if (data == 7) {
    angle += 5;
    myServo.write(angle);
    }
  else if (data == 6) {
    angle -= 5;
    myServo.write(angle);
  }
  else if (data == 78)
    digitalWrite(pump, HIGH);
  else if (data == 70)
    digitalWrite(pump, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("   Manual Mode  ");
}

// Automatic fire fighting mode
void FA() {
  if (Serial.read() == 73)
    Mode = 'I';
  if (Serial.read() == 82)
    Mode = 'R';

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println(" Fire Auto Mode ");

  s1 = analogRead(ir_R);    // Right
  s3 = analogRead(ir_M);    // front
  s2 = analogRead(ir_L);    // left

  Serial.print("Right: "); Serial.print(s1);
  Serial.print(" | Middle: "); Serial.print(s2);
  Serial.print(" | Left: "); Serial.println(s3);
  delay(50);
  if (s1 < 37) {
    Stop();
    digitalWrite(pump, 1);
    for (int angle = 90; angle <= 180; angle += 3)
      servoPulse(servo, angle);
    for (int angle = 180; angle >= 90; angle -= 3)
      servoPulse(servo, angle);
  }
  else if (s2 < 41) {
    Stop();
    digitalWrite(pump, 1);
    for (int angle = 90; angle <= 140; angle += 3) 
      servoPulse(servo, angle);
    for(int angle = 140; angle >= 40; angle -= 3)
      servoPulse(servo, angle);
    for(int angle = 40; angle <= 90; angle += 3)
      servoPulse(servo, angle);
  }
  else if (s3 < 41) {
    Stop();
    digitalWrite(pump, 1);
    for(int angle = 90; angle >= 0; angle -= 3) {
      servoPulse(servo, angle);
    }
    for(int angle = 0; angle <= 90; angle += 3) {
      servoPulse(servo, angle);
    }
  }
  else if (s1 >= 41 && s1 <= 920) {
    digitalWrite(pump, 0);
    forword();
    delay(80);
    turnRight();
    delay(200);
  }
  else if(s2 >= 41 && s2 <= 920) {
    digitalWrite(pump, 0);
    forword();
  }
  else if(s3 >= 41 && s3 <= 920){
    digitalWrite(pump, 0);
    forword();
    delay(80);
    turnLeft();
    delay(200);
  } 
  else {
    digitalWrite(pump, 0);
    Stop();
  }
  delay(10);
}

void servoPulse (int pin, int angle){
  int pwm = (angle * 11) + 500; // Convert angle to microseconds
  myServo.write(angle);
  delayMicroseconds(pwm);
  delay(50); // Refresh cycle of servo
}

void forword(){ //forword
  analogWrite(IN1, 90); 
  analogWrite(IN2, 0); 
  analogWrite(IN3, 90); 
  analogWrite(IN4, 0);
}

void backword(){ //backword
  analogWrite(IN1, 0);
  analogWrite(IN2, 90);
  analogWrite(IN3, 0);
  analogWrite(IN4, 90);
}

void turnRight(){ //turnRight
  analogWrite(IN1, 90); 
  analogWrite(IN2, 0); 
  analogWrite(IN3, 0); 
  analogWrite(IN4, 90); 
}

void turnLeft(){ //turnLeft
  analogWrite(IN1, 0);
  analogWrite(IN2, 90); 
  analogWrite(IN3, 90);
  analogWrite(IN4, 0); 
}

void Stop(){ //stop
  analogWrite(IN1, 0); 
  analogWrite(IN2, 0); 
  analogWrite(IN3, 0); 
  analogWrite(IN4, 0); 
}