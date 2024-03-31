#include <math.h>
#include <digitalWriteFast.h>

const int XStep = 2;
const int XDir = 5;
int Flag = 0;

struct State {
  int XStepPin;
  int YStepPin;
  int ZStepPin;
  int AStepPin;
  int BStepPin;
  int CStepPin;

  int X;
  int Y;
  int Z;

  int A;
  int B;
  int C;
  int Grip;
};

String ReadInSerial = "";
State sInitial;  // initial state
State sFinal;    // final state

void setup() {
  // sets up motors functions and directions
  pinMode(XStep, OUTPUT);
  pinMode(XDir, OUTPUT);

  //HomeMotors(XStep);

  Serial.begin(9600);
  Serial.println("Begin Serial");

  //Initialize First values for steppers
  sInitial.X = 0;
  sInitial.Y = 0;
  sInitial.Z = 0;
  sInitial.A = 0;
  sInitial.B = 0;
  sInitial.C = 0;

  sInitial.XStepPin = 2;
  sInitial.YStepPin = 0;
  sInitial.ZStepPin = 0;
  sInitial.AStepPin = 0;
  sInitial.BStepPin = 0;
  sInitial.CStepPin = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    Serial.println("Reading serial port");

    Flag = 0;
    ReadInSerial = Serial.readString();
    sFinal = ParseString(ReadInSerial);
  }
}

State ParseString(String InputString) {
  State s;

  int StartAtChar = 0;
  ////////// SUBSTRING X //////////
  int xCommaVal = InputString.indexOf(",", StartAtChar);
  String xTempString = InputString.substring(0, xCommaVal);
  s.X = xTempString.toInt();
  Serial.println("SUBSTRING X");
  Serial.println(s.X);
  moveStepper(sInitial.XStepPin, s.X, sInitial.X);
  sInitial.X = s.X;
  StartAtChar = xCommaVal + 1;

  ////////// SUBSTRING Y //////////
  int yCommaVal = InputString.indexOf(",", StartAtChar);
  String yTempString = InputString.substring(StartAtChar, yCommaVal);
  s.Y = yTempString.toDouble();
  Serial.println("SUBSTRING Y");
  Serial.println(yTempString);
  Serial.println(s.Y);
  sInitial.Y = s.Y;
  StartAtChar = yCommaVal + 1;

    ////////// SUBSTRING Z //////////
  int zCommaVal = InputString.indexOf(",", StartAtChar);
  String zTempString = InputString.substring(StartAtChar, zCommaVal);
  s.Z = zTempString.toDouble();
  Serial.println("SUBSTRING Z");
  Serial.println(zTempString);
  Serial.println(s.Z);
  sInitial.Z = s.Z;
  StartAtChar = zCommaVal + 1;

  ////////// SUBSTRING A //////////
  int aCommaVal = InputString.indexOf(",", StartAtChar);
  String aTempString = InputString.substring(StartAtChar, aCommaVal);
  s.A = aTempString.toDouble();
  Serial.println("SUBSTRING A");
  Serial.println(aTempString);
  Serial.println(s.A);
  sInitial.A = s.A;
  StartAtChar = aCommaVal + 1;

    ////////// SUBSTRING B //////////
  int bCommaVal = InputString.indexOf(",", StartAtChar);
  String bTempString = InputString.substring(StartAtChar, bCommaVal);
  s.B = bTempString.toDouble();
  Serial.println("SUBSTRING B");
  Serial.println(bTempString);
  Serial.println(s.B);
  sInitial.B = s.B;
  StartAtChar = bCommaVal + 1;

  ////////// SUBSTRING C //////////
  int cCommaVal = InputString.indexOf(",", StartAtChar);
  String cTempString = InputString.substring(StartAtChar, cCommaVal);
  s.C = cTempString.toDouble();
  Serial.println("SUBSTRING C");
  Serial.println(cTempString);
  Serial.println(s.C);
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
      digitalWriteFast(MotorPin, HIGH);
      delay(1);
      digitalWriteFast(MotorPin, LOW);
      delay(1);
    }
  } else {
    digitalWrite(XDir, HIGH);  // High = clockwise
    sFinalX = abs(sFinalX - sInitialX);

    for (int k = 0; k < sFinalX * 8 * 0.5555555555; k++) {
      digitalWriteFast(MotorPin, HIGH);
      delay(1);
      digitalWriteFast(MotorPin, LOW);
      delay(1);
    }
  }
  return 0;
}


void HomeMotors(int MotorPin) {
  digitalWrite(XDir, HIGH);  // High = clockwise
  int t = 0;
  while (t < 360 * 8 * 0.5555555555) {
    digitalWrite(MotorPin, HIGH);
    delay(1);
    digitalWrite(MotorPin, LOW);
    delay(1);
    t += 1;
  }
}
