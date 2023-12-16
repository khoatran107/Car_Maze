const int motor1pin1 = 4;
const int motor1pin2 = 5;
const int motor1en = 10;

const int motor2pin1 = 6;
const int motor2pin2 = 7;
const int motor2en = 9;

#define PIN_DO_LEFT 3
#define PIN_DO_RIGHT 2
#define HOLES_DISC 20

volatile unsigned int leftPulses;
volatile unsigned int rightPulses;

volatile unsigned int turnHoleCount;

const int holeRequired = 28;

const int targetSpeed = 200;
const int tolerance = 5;
int oldTime;
bool turnYet = false;

enum Mode {
  GO_STRAIGHT,
  TURN_LEFT,
  TURN_RIGHT,
};

Mode currentMode = GO_STRAIGHT;


void leftCounter() {
  leftPulses++;
  turnHoleCount++;
  if (turnHoleCount == holeRequired) currentMode = GO_STRAIGHT;
}

void rightCounter() {
  rightPulses++;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  pinMode(motor1en, OUTPUT);
  pinMode(motor2en, OUTPUT);
  Serial.begin(9600);
  pinMode(PIN_DO_LEFT, INPUT);
  pinMode(PIN_DO_RIGHT, INPUT);
  leftPulses = 0;
  rightPulses = 0;
  turnHoleCount = holeRequired + 1;
  attachInterrupt(digitalPinToInterrupt(PIN_DO_LEFT), leftCounter, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_DO_RIGHT), rightCounter, FALLING);
  oldTime = 0;
}

void loop() {
  if (millis() - oldTime >= 2000 && !turnYet) {
    setTurnLeft();
    turnYet = true;
    Serial.println("Hello world");
  }

  if (millis() - oldTime >= 6000 && !turnYet) {
    setTurnLeft();
    turnYet = true;
    Serial.println("Hello world");
  }

  Serial.print(currentMode);
  int difference = abs(leftPulses - rightPulses);
  if (difference > tolerance) {
    if (leftPulses < rightPulses) {
      analogWrite(motor2en, targetSpeed - difference);
    } else {
      analogWrite(motor1en, targetSpeed - difference);
    }
  } else {
    analogWrite(motor1en, targetSpeed);
    analogWrite(motor2en, targetSpeed);
  }
  leftPulses = 0;
  rightPulses = 0;
  if (currentMode == GO_STRAIGHT) {
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
  } else if (currentMode == TURN_LEFT) {
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
  } else if (currentMode == TURN_RIGHT) {
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
  }
  delay(10);
}

void setTurnLeft() {
  turnHoleCount = 0;
  currentMode = TURN_LEFT;
}

void setTurnRight() {
  turnHoleCount = 0;
  currentMode = TURN_RIGHT;
}
