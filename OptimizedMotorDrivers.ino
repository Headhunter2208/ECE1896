#include <math.h>
#include <digitalWriteFast.h>
#include <Encoder.h>

// Declare constant multipliers and variables used for testing
const int StepsPerDegree = 8;
const float DegreesOverSteps = 0.55555555555555555;
unsigned long TimeDelayMilliBEFORE;
unsigned long TimeDelayMilliAFTER;
unsigned long TimeDelayMilliMOVE;

// Declare Direction pins for each motor
const int XDir = 5;
const int YDir = 6;
const int ZDir = 0;
const int ADir = 0;
const int BDir = 0;
const int CDir = 0;
const int GripDir = 0;

// Declare driving step pins for all motors
const int XStepPin = 2;
const int YStepPin = 3;
const int ZStepPin;
const int AStepPin;
const int BStepPin;
const int CStepPin;
const int GripStepPin;

// Declare constant gear ratio multipliers for moving motors
const float xGearRatioMult = 13.5;
const float yGearRatioMult = 150;
const float zGearRatioMult = 150;
const float aGearRatioMult = 48;
const float bGearRatioMult = 67.82;
const float cGearRatioMult = 67.82;

// Create struct that holds all joint angle positions
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
  // sets up motors functions and directions as outputs
  pinMode(XStepPin, OUTPUT);
  pinMode(XDir, OUTPUT);
  pinMode(YStepPin, OUTPUT);
  pinMode(YDir, OUTPUT);
  pinMode(ZStepPin, OUTPUT);
  pinMode(ZDir, OUTPUT);
  pinMode(AStepPin, OUTPUT);
  pinMode(ADir, OUTPUT);
  pinMode(BStepPin, OUTPUT);
  pinMode(BDir, OUTPUT);
  pinMode(CStepPin, OUTPUT);
  pinMode(CDir, OUTPUT);
  pinMode(GripStepPin, OUTPUT);
  pinMode(GripDir, OUTPUT);

  // Init starting states for each struct
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

  // Init serial data connections
  Serial.begin(115200);
  Serial.println("Begin Serial");
}

void loop() {
  // Checking serial buffer has available space
  if (Serial.available() > 0) {
    // Used for Latency Testing
    TimeDelayMilliBEFORE = millis();

    ReadInSerial = Serial.readStringUntil('\n');

    // Parse Incoming joint data via serial connection
    sFinal = ParseString(ReadInSerial);

    // Output latency time from 
    TimeDelayMilliAFTER = millis();
    Serial.println(TimeDelayMilliAFTER - TimeDelayMilliBEFORE);

    // Move motors according to new states
    moveStepper(XStepPin, XDir, sFinal.X, sInitial.X, xGearRatioMult);
    moveStepper(YStepPin, YDir, sFinal.Y, sInitial.Y, yGearRatioMult);
    moveStepper(ZStepPin, ZDir, sFinal.Z, sInitial.Z, zGearRatioMult);
    moveStepper(AStepPin, ADir, sFinal.A, sInitial.A, aGearRatioMult);
    moveStepper(BStepPin, BDir, sFinal.B, sInitial.B, bGearRatioMult);
    moveStepper(CStepPin, CDir, sFinal.C, sInitial.C, cGearRatioMult);
    moveStepper(GripStepPin, GripDir, sFinal.Grip, sInitial.Grip, 1.0);

    //Update states for next iterations of movements
    sInitial = UpdateStates(sFinal);

    // Prints joint locations of robot arm to serial
    PrintLocation(sFinal);
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

void moveStepper(int StepPin, int StepDir, int sFinal, int sInitial, float GearRatioMult) {
  if (sFinal < sInitial) {
    digitalWrite(StepDir, LOW);  // low = counter-clockwise
    sFinal = abs(sFinal - sInitial);
    sFinal = sFinal * StepsPerDegree * DegreesOverSteps * GearRatioMult;

    for (int k = 0; k < sFinal; k++) {
      digitalWriteFast(StepPin, HIGH);
      delayMicroseconds(500);
      digitalWriteFast(StepPin, LOW);
      delayMicroseconds(500);
    }
  } else {
    digitalWrite(StepDir, HIGH);  // High = clockwise
    sFinal = abs(sFinal - sInitial);
    sFinal = sFinal * StepsPerDegree * DegreesOverSteps * GearRatioMult;

    for (int k = 0; k < sFinal; k++) {
      digitalWriteFast(StepPin, HIGH);
      delayMicroseconds(500);
      digitalWriteFast(StepPin, LOW);
      delayMicroseconds(500);
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

void PrintLocation(State s){
  Serial.print(s.X);
  Serial.print(", ");
  Serial.print(s.Y);
  Serial.print(", ");
  Serial.print(s.Z);
  Serial.print(", ");
  Serial.print(s.A);
  Serial.print(", ");
  Serial.print(s.B);
  Serial.print(", ");
  Serial.print(s.C);
  Serial.print(", ");
  Serial.print(s.Grip);
  Serial.print("\n");
}

void HomeMotors(int MotorPin, int DirPin) {
  digitalWrite(DirPin, LOW);  // low = counter-clockwise
  int t = 0;
  while (t < 360 * StepsPerDegree * DegreesOverSteps) {
    digitalWriteFast(MotorPin, HIGH);
    delay(1);
    digitalWriteFast(MotorPin, LOW);
    delay(1);
    t += 1;
  }
}
