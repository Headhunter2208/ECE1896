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

  double X;
  double Y;
  double Z;

  double A;
  double B;
  double C;
  double Grip;
};

String ReadInSerial = "";
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

  int StartAtChar = 0;
  ////////// SUBSTRING X //////////
  int xCommaVal = InputString.indexOf(",", StartAtChar);
  String xTempString = InputString.substring(0, xCommaVal);
  s.X = xTempString.toDouble();
  Serial.println("SUBSTRING X");
  Serial.println(xTempString);
  Serial.println(s.X);
  sInitial.X = s.X;
  StartAtChar = xCommaVal + 1;

  ////////// SUBSTRING Y //////////
  int yCommaVal = InputString.indexOf(",", StartAtChar);
  String yTempString = InputString.substring(StartAtChar, yCommaVal);
  s.Y = yTempString.toDouble();
  Serial.println("SUBSTRING Y");
  Serial.println(yTempString);
  Serial.println(s.Y * 10000);
  sInitial.Y = s.Y;
  StartAtChar = yCommaVal + 1;

    ////////// SUBSTRING Z //////////
  int zCommaVal = InputString.indexOf(",", StartAtChar);
  String zTempString = InputString.substring(StartAtChar, zCommaVal);
  s.Z = zTempString.toDouble();
  Serial.println("SUBSTRING Z");
  Serial.println(zTempString);
  Serial.println(s.Z * 10000);
  sInitial.Z = s.Z;
  StartAtChar = zCommaVal + 1;

  ////////// SUBSTRING A //////////
  int aCommaVal = InputString.indexOf(",", StartAtChar);
  String aTempString = InputString.substring(StartAtChar, aCommaVal);
  s.A = aTempString.toDouble();
  Serial.println("SUBSTRING A");
  Serial.println(aTempString);
  Serial.println(s.A * 10000);
  sInitial.A = s.A;
  StartAtChar = aCommaVal + 1;

    ////////// SUBSTRING B //////////
  int bCommaVal = InputString.indexOf(",", StartAtChar);
  String bTempString = InputString.substring(StartAtChar, bCommaVal);
  s.B = bTempString.toDouble();
  Serial.println("SUBSTRING B");
  Serial.println(bTempString);
  Serial.println(s.B * 10000);
  sInitial.B = s.B;
  StartAtChar = bCommaVal + 1;

  ////////// SUBSTRING C //////////
  int cCommaVal = InputString.indexOf(",", StartAtChar);
  String cTempString = InputString.substring(StartAtChar, cCommaVal);
  s.C = cTempString.toDouble();
  Serial.println("SUBSTRING C");
  Serial.println(cTempString);
  Serial.println(s.C * 10000);
  sInitial.C = s.C;
  StartAtChar = cCommaVal + 1;

  ////////// SUBSTRING GRIPPER //////////
  int gripCommaVal = InputString.indexOf(",", StartAtChar);
  Serial.println(gripCommaVal);
  String gripTempString = InputString.substring(StartAtChar, gripCommaVal);
  s.Grip = gripTempString.toDouble();
  Serial.println("SUBSTRING GRIPPER");
  Serial.println(gripTempString);
  Serial.println(s.Grip);
  sInitial.Grip = s.Grip;
  StartAtChar = gripCommaVal + 1;

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
