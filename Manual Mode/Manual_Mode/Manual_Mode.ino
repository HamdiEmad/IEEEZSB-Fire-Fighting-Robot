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
#define pump A0   // Pump pin

byte data, angle = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(pump, OUTPUT);
  myServo.attach(2);  // Servo is connected to pin 10
  myServo.write(90);   // Default position of the servo
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("   Manual Mode  ");
  if (Serial.available()) {
    data = Serial.read();
    Serial.println(data);
  }

  delay(30);

  if (data == 1) {              // Moving forward
    analogWrite(IN1, 255);
    analogWrite(IN2, 0);
    analogWrite(IN3, 255);
    analogWrite(IN4, 0);
  }

  else if (data == 2) {        // Moving backward
    analogWrite(IN1, 0);
    analogWrite(IN2, 255);
    analogWrite(IN3, 0);
    analogWrite(IN4, 255);
  }
  else if (data == 3) {        // Moving Right
    analogWrite(IN1, 255);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
  }
  else if (data == 4) {        // Moving left
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
    analogWrite(IN3, 255);
    analogWrite(IN4, 0);
  }
  else if (data == 5) {       // Stop
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
  }
  else if (data == 7) {       // Moving servo towards left direction
    angle += 5;
    myServo.write(angle);
    }
  else if (data == 6) {       // Moving servo towards right direction
    angle -= 5;
    myServo.write(angle);
  }
  else if (data == 78)        // Activating pumb
    digitalWrite(pump, HIGH);
  else if (data == 70)        // Disabling pumb
    digitalWrite(pump, LOW);
}
