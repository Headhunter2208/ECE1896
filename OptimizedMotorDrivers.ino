#include <math.h>

const int XStep = 2;
const int XDir = 5;
int Flag = 0;

struct State {
  int XStep;
  int YStep;
  int ZStep;
  int AStep;
  int BStep;
  int CStep;

  int X;
  int Y;
  int Z;

  int A;
  int B;
  int C;
};

String ReadInSerial = "";
int StartAtChar = 0;
State sInitial;  // initial state
State sFinal;    // final state

void setup() {
  // sets up motors functions and directions
  pinMode(XStep, OUTPUT);
  pinMode(XDir, OUTPUT);
  digitalWrite(XDir, HIGH);  // High = clockwise

  HomeMotors(XStep);

  Serial.begin(9600);
  Serial.println("Begin Serial");

  //Initialize First values for steppers
  sInitial.X = 0;
  sInitial.Y = 0;
  sInitial.Z = 0;
  sInitial.A = 0;
  sInitial.B = 0;
  sInitial.C = 0;

  sInitial.XStep = 2;
  sInitial.YStep = 0;
  sInitial.ZStep = 0;
  sInitial.AStep = 0;
  sInitial.BStep = 0;
  sInitial.CStep = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    Serial.println("Reading serial port");

    Flag = 0;
    ReadInSerial = Serial.readString();
  }

  sFinal = ParseString(ReadInSerial);
}

State ParseString(String InputString) {
  State s;

  if (Flag == 0) {
    if (InputString.length() == 40) {
      //Flag = 1;
      ////////// SUBSTRING X //////////
      if (InputString.substring(0, 1).equals("X")) {
        s.X = InputString.substring(2, 5).toInt();
        moveStepper(sInitial.XStep, s.X, sInitial.X);
        sInitial.X = s.X;
        Flag = 1;
      } else s.X = sInitial.X;

      ////////// SUBSTRING Y //////////
      if (InputString.substring(7, 8).equals("Y")) {
        //Serial.println("Valid String Y");
        s.Y = InputString.substring(9, 12).toInt();
      } else s.Y = sInitial.Y;

      ////////// SUBSTRING Z //////////
      if (InputString.substring(14, 15).equals("Z")) {
        //Serial.println("Valid String Z");
        s.Z = InputString.substring(16, 19).toInt();
      } else s.Z = sInitial.Z;

      ////////// SUBSTRING A //////////
      if (InputString.substring(21, 22).equals("A")) {
        //Serial.println("Valid String A");
        s.A = InputString.substring(23, 26).toInt();
      } else s.A = sInitial.A;

      ////////// SUBSTRING B //////////
      if (InputString.substring(28, 29).equals("B")) {
        //Serial.println("Valid String B");
        s.B = InputString.substring(30, 33).toInt();
      } else s.B = sInitial.B;

      ////////// SUBSTRING C //////////
      if (InputString.substring(35, 36).equals("C")) {
        //Serial.println("Valid String C");
        s.C = InputString.substring(37, 40).toInt();
      } else s.C = sInitial.C;
    }

    //else for if the whole input string is longer than 40 chars
    else {
      Flag = 1;
      Serial.println("Invalid String");
      s.X = sInitial.X;
      s.Y = sInitial.Y;
      s.Z = sInitial.Z;
      s.A = sInitial.A;
      s.B = sInitial.B;
      s.C = sInitial.C;
    }
  }
  return s;
}

void moveStepper(int MotorPin, int sFinalX, int sInitialX) {
  if (sFinalX < sInitialX) {
    digitalWrite(XDir, LOW);  // High = clockwise
    sFinalX = abs(sFinalX - sInitialX);

    for (int k = 0; k < sFinalX * 8 * 0.5555555555; k++) {
      digitalWrite(MotorPin, HIGH);
      delay(1);
      digitalWrite(MotorPin, LOW);
      delay(1);
    }
  } else {
    digitalWrite(XDir, HIGH);  // High = clockwise
    sFinalX = abs(sFinalX - sInitialX);

    for (int k = 0; k < sFinalX * 8 * 0.5555555555; k++) {
      digitalWrite(MotorPin, HIGH);
      delay(1);
      digitalWrite(MotorPin, LOW);
      delay(1);
    }
  }
  return 0;
}


void HomeMotors(int MotorPin) {
  int t = 0;
  while (t < 360 * 8 * 0.5555555555) {
    digitalWrite(MotorPin, HIGH);
    delay(1);
    digitalWrite(MotorPin, LOW);
    delay(1);
    t += 1;
  }
}
