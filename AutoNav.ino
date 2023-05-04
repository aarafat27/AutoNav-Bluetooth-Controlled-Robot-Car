#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//COLOR SENSOR
#define S0 5
#define S1 4
#define S2 7
#define S3 6
#define sensorOut 8

//CAR
#define in1 12
#define in2 11
#define in3 10
#define in4 9
//
//boolean brake = false;
//boolean rgbEnabled = false;

int command;
int Speed = 204;
int Speedsec;
int buttonState = 0;
int lastButtonState = 0;
int Turnradius = 0;
int brakeTime = 45;
int brkonoff = 1;

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Stores the red. green and blue colors
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

void setup() {
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  //Motor
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);

  // Setting frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  Serial.begin(9600);
}

void loop() {

  if (Serial.available() > 0) {
    command = Serial.read();
    Stop();
    switch (command) {
      case 'F':
        forward();
        break;
      case 'B':
        back();
        break;
      case 'L':
        left();
        break;
      case 'R':
        right();
        break;
      case'X':
        light();
        break;

      case '0':
        Speed = 100;
        break;
      case '1':
        Speed = 140;
        break;
      case '2':
        Speed = 153;
        break;
      case '3':
        Speed = 165;
        break;
      case '4':
        Speed = 178;
        break;
      case '5':
        Speed = 191;
        break;
      case '6':
        Speed = 204;
        break;
      case '7':
        Speed = 216;
        break;
      case '8':
        Speed = 229;
        break;
      case '9':
        Speed = 242;
        break;
      case 'q':
        Speed = 255;
        break;
    }
    Speedsec = Turnradius;
    if (brkonoff == 1) {
      brakeOn();
    } else {
      brakeOff();
    }
  }
}

void forward() {
  analogWrite(in1, Speed);
  analogWrite(in3, Speed);
}

void back() {
  analogWrite(in2, Speed);
  analogWrite(in4, Speed);
}

void left() {
  analogWrite(in3, Speed);
  analogWrite(in2, Speed);
}

void right() {
  analogWrite(in4, Speed);
  analogWrite(in1, Speed);
}


void Stop() {
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
}

void brakeOn() {

  buttonState = command;
  if (buttonState != lastButtonState) {
    if (buttonState == 'S') {
      if (lastButtonState != buttonState) {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, HIGH);
        delay(brakeTime);
        Stop();
      }
    }
    lastButtonState = buttonState;
  }
}
void brakeOff() {
}

void light()
{
  // COLOR DETECTION
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  redFrequency = pulseIn(sensorOut, LOW);
  redColor = map(redFrequency, 60, 180, 255, 0);
  Serial.print("R = ");
  Serial.print(redColor);
  delay(100);

  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);

  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  greenColor = map(greenFrequency, 91, 204, 255, 0);

  // Printing the GREEN (G) value
  Serial.print(" G = ");
  Serial.print(greenColor);
  delay(100);

  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);


  blueFrequency = pulseIn(sensorOut, LOW);
  blueColor = map(blueFrequency, 64, 200, 255, 0);

  // Printing the BLUE (B) value
  Serial.print(" B = ");
  Serial.print(blueColor);
  delay(100);

  if (redColor > greenColor && redColor > blueColor) {
    lcd.setCursor(2, 0);  //Set cursor to character 2 on line 0
    lcd.print(" Red Detected ");

  }
  if (greenColor > redColor && greenColor > blueColor) {
    lcd.setCursor(2, 0);  //Set cursor to character 2 on line 0
    lcd.print("Green Detected");

  }
  if (blueColor > redColor && blueColor > greenColor) {
    lcd.setCursor(2, 0);  //Set cursor to character 2 on line 0
    lcd.print(" Blue Detected");
  }
}
