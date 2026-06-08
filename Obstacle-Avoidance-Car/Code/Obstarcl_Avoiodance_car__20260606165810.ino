#include <Servo.h>

// Ultrasonic
#define TRIG_PIN 12
#define ECHO_PIN 13

// Servo
#define SERVO_PIN 11

// L298N
#define ENA 5
#define ENB 6

#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10

Servo myservo;

// Motor speeds
int forwardSpeed = 100;
int turnSpeed = 120;
int reverseSpeed = 150;

void setup()
{
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  myservo.attach(SERVO_PIN);

  myservo.write(90);
  delay(1000);
}

void loop()
{
  long distance = getDistance();

  Serial.print("Front: ");
  Serial.println(distance);

  if (distance <= 35)
  {
    avoidObstacle();
  }
  else
  {
    moveForward();
  }
}

// ==========================
// OBSTACLE AVOIDANCE
// ==========================

void avoidObstacle()
{
  stopMotors();
  delay(200);

  moveBackward();
  delay(500);

  stopMotors();
  delay(200);

  long leftDistance = scanLeft();
  long rightDistance = scanRight();

  Serial.print("Left: ");
  Serial.println(leftDistance);

  Serial.print("Right: ");
  Serial.println(rightDistance);

  if (leftDistance < 20 && rightDistance < 20)
  {
    moveBackward();
    delay(800);

    turnRight();
    delay(700);
  }
  else if (leftDistance > rightDistance)
  {
    turnLeft();
    delay(600);
  }
  else
  {
    turnRight();
    delay(600);
  }

  stopMotors();
  delay(100);
}

// ==========================
// ULTRASONIC FUNCTIONS
// ==========================

long getDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0)
    return 250;

  long distance = duration * 0.034 / 2;

  return distance;
}

// ==========================
// SERVO SCAN
// ==========================

long scanLeft()
{
  myservo.write(15);
  delay(600);

  long distance = getDistance();

  myservo.write(90);
  delay(300);

  return distance;
}

long scanRight()
{
  myservo.write(165);
  delay(600);

  long distance = getDistance();

  myservo.write(90);
  delay(300);

  return distance;
}

// ==========================
// MOTOR FUNCTIONS
// ==========================

void moveForward()
{
  analogWrite(ENA, forwardSpeed);
  analogWrite(ENB, forwardSpeed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward()
{
  analogWrite(ENA, reverseSpeed);
  analogWrite(ENB, reverseSpeed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft()
{
  analogWrite(ENA, turnSpeed);
  analogWrite(ENB, turnSpeed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight()
{
  analogWrite(ENA, turnSpeed);
  analogWrite(ENB, turnSpeed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}