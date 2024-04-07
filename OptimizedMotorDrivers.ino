#include <math.h>
#include <digitalWriteFast.h>

const int StepsPerDegree = 2;
const float DegreesOverSteps = 0.55555555555555555;

const int XDir = 5;
const int YDir = 0;
const int ZDir = 0;
const int ADir = 0;
const int BDir = 0;
const int CDir = 0;
const int GripDir = 0;

const int XStepPin = 2;
const int YStepPin;
const int ZStepPin;
const int AStepPin;
const int BStepPin;
const int CStepPin;
const int GripStepPin;

struct State {
  int X = 0;
  int Y = 0;
  int Z = 0;

  int A = 0;
  int B = 0;
  int C = 0;
  int Grip = 0;
};

String ReadInSerial = "";
State sInitial;  // initial state
State sFinal;    // final state

void setup() {
  // sets up motors functions and directions
  pinMode(XStepPin, OUTPUT);
  pinMode(XDir, OUTPUT);

  sInitial.X = 0;
  sInitial.Y = 0;
  sInitial.Z = 0;
  sInitial.A = 0;
  sInitial.B = 0;
  sInitial.C = 0;

  sFinal.X = 0;
  sFinal.Y = 0;
  sFinal.Z = 0;
  sFinal.A = 0;
  sFinal.B = 0;
  sFinal.C = 0;

  Serial.begin(9600);
  Serial.println("Begin Serial");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {

    ReadInSerial = Serial.readStringUntil('\n');

    // Parse Incoming joint data via serial connection
    sFinal = ParseString(ReadInSerial);

    // Move motors according to new states
    moveStepper(XStepPin, XDir, sFinal.X, sInitial.X);
    // moveStepper(YDir, YStepPin, sFinal.Y, sInitial.Y);
    // moveStepper(ZDir, ZStepPin, sFinal.Z, sInitial.Z);
    // moveStepper(ADir, AStepPin, sFinal.A, sInitial.A);
    // moveStepper(BDir, BStepPin, sFinal.B, sInitial.B);
    // moveStepper(CDir, CStepPin, sFinal.C, sInitial.C);
    // moveStepper(GripDir, GripStepPin, sFinal.Grip, sInitial.Grip);

    //Update states for next iterations of movements
    sInitial = UpdateStates(sFinal);
  }
}

State ParseString(String InputString) {
  State s;

  int StartAtChar = 0;
  ////////// SUBSTRING X //////////
  int xCommaVal = InputString.indexOf(",", StartAtChar);
  String xTempString = InputString.substring(0, xCommaVal);
  s.X = xTempString.toInt();
  StartAtChar = xCommaVal + 1;

  ////////// SUBSTRING Y //////////
  int yCommaVal = InputString.indexOf(",", StartAtChar);
  String yTempString = InputString.substring(StartAtChar, yCommaVal);
  s.Y = yTempString.toDouble();
  StartAtChar = yCommaVal + 1;

  ////////// SUBSTRING Z //////////
  int zCommaVal = InputString.indexOf(",", StartAtChar);
  String zTempString = InputString.substring(StartAtChar, zCommaVal);
  s.Z = zTempString.toDouble();
  StartAtChar = zCommaVal + 1;

  ////////// SUBSTRING A //////////
  int aCommaVal = InputString.indexOf(",", StartAtChar);
  String aTempString = InputString.substring(StartAtChar, aCommaVal);
  s.A = aTempString.toDouble();
  StartAtChar = aCommaVal + 1;

  ////////// SUBSTRING B //////////
  int bCommaVal = InputString.indexOf(",", StartAtChar);
  String bTempString = InputString.substring(StartAtChar, bCommaVal);
  s.B = bTempString.toDouble();
  StartAtChar = bCommaVal + 1;

  ////////// SUBSTRING C //////////
  int cCommaVal = InputString.indexOf(",", StartAtChar);
  String cTempString = InputString.substring(StartAtChar, cCommaVal);
  s.C = cTempString.toDouble();
  StartAtChar = cCommaVal + 1;

  ////////// SUBSTRING GRIPPER //////////
  int gripCommaVal = InputString.indexOf(",", StartAtChar);
  String gripTempString = InputString.substring(StartAtChar, gripCommaVal);
  s.Grip = gripTempString.toDouble();
  StartAtChar = gripCommaVal + 1;

  return s;
}

void moveStepper(int StepPin, int StepDir, int sFinal, int sInitial) {
  // Move section for X stepper motor
  if (sFinal < sInitial) {
    digitalWrite(StepDir, LOW);  // High = clockwise
    sFinal = abs(sFinal - sInitial);

    for (int k = 0; k < sFinal * StepsPerDegree * DegreesOverSteps; k++) {
      digitalWrite(StepPin, HIGH);
      delay(1);
      digitalWrite(StepPin, LOW);
      delay(1);
    }
  } else {
    digitalWrite(StepDir, HIGH);  // High = clockwise
    sFinal = abs(sFinal - sInitial);

    for (int k = 0; k < sFinal * StepsPerDegree * DegreesOverSteps; k++) {
      digitalWrite(StepPin, HIGH);
      delay(1);
      digitalWrite(StepPin, LOW);
      delay(1);
    }
  }
}

State UpdateStates(State sFinal) {
  State s;
  s.X = sFinal.X;
  s.Y = sFinal.Y;
  s.Z = sFinal.Z;
  s.A = sFinal.A;
  s.B = sFinal.B;
  s.C = sFinal.C;
  s.Grip = sFinal.Grip;

  return s;
}

void HomeMotors(int MotorPin) {
  digitalWrite(XDir, HIGH);  // High = clockwise
  int t = 0;
  while (t < 360 * 8 * 0.555555555555) {
    digitalWrite(MotorPin, HIGH);
    delay(1);
    digitalWrite(MotorPin, LOW);
    delay(1);
    t += 1;
  }
}
