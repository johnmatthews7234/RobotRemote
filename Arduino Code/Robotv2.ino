#include <Servo.h>
Servo myservo;

unsigned int Lcounter = 0;
unsigned int Rcounter = 0;

#define echo 12
#define trig 13
#define srvo 9
long duration;
int distance;
int angle;
float safeDistance;

#define lre 2
#define rre 3

#define ena 5
#define enb 6
int Lspeed;
int Rspeed;

#define in1 11
#define in2 10
#define in3 8
#define in4 7


String commandstring = "";
bool stringcomplete = false;

void resetCounters() {
  Lcounter = 0;
  Rcounter = 0;
}

void Ldocount() {
  Lcounter++;
}

void Rdocount() {
  Rcounter++;
}

void setup() {
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  myservo.attach(srvo);
  digitalWrite(trig, LOW);
  safeDistance = 15.0;

  pinMode(lre, INPUT);
  pinMode(rre, INPUT);
  attachInterrupt(digitalPinToInterrupt(lre),Ldocount,RISING);
  attachInterrupt(digitalPinToInterrupt(rre),Rdocount,RISING);
  resetCounters();
  
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);

  
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(9600);
  commandstring.reserve(80);
}

void moveStep(long step) {
  while ((Lcounter < step) || (Rcounter < step)){
    Serial.print(Lcounter);
    Serial.print(", ");
    Serial.print(Lspeed);
    Serial.print(", ");
    Serial.print(Rcounter);
    Serial.print(", ");
    Serial.println(Rspeed);
    if (PointRadar(90) < safeDistance) {
      setMotorSpeed(0);
    } else if (Lcounter < Rcounter) {
      setLspeed(128);
      setRspeed(64);
    } else if (Lcounter > Rcounter) {
      setLspeed(64);
      setRspeed(128);
    } else {
      setMotorSpeed(96);
    }
    delay(10);
  }

  if (Lcounter < Rcounter) {
    digitalWrite(enb,0);
    while (Lcounter < step) {
      delay(1);
    }
  } else if (Rcounter < Lcounter) {
    digitalWrite(ena,0);
    while (Lcounter < step) {
      delay(1);
    }
  }
}

void setMotorSpeed(int speed) {
  setLspeed(speed);
  setRspeed(speed);
}

void setLspeed(int speed) {
  Lspeed = speed;
  analogWrite(enb, Lspeed);
}

void setRspeed(int speed) {
  Rspeed = speed;
  analogWrite(ena, Rspeed);
}

float PointRadar(int degrees) {
  myservo.write(degrees);
  int deltaAngle = max(angle,degrees) - min(angle,degrees);
  angle = degrees;
  int pause = map(deltaAngle,0,180,200,500);
  delay(pause);
  digitalWrite(trig, HIGH);
  delay(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  Serial.print("PointRadar: ");
  Serial.println(duration * 0.034 / 2);
  return duration * 0.034 / 2;
}


void forward(long step, int speed) {
  if ((PointRadar(45) > safeDistance) && (PointRadar(90) > safeDistance) && (PointRadar(135) > safeDistance)) { 
    resetCounters();
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    setMotorSpeed(speed);
    moveStep(step);
  }
}

void backward(long step, int speed) {
  resetCounters();
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  setMotorSpeed(speed);
  moveStep(step);
}

void clockwise(int degrees, int speed) {
  resetCounters();
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  setMotorSpeed(speed);
  moveStep(degrees);
}

void anticlockwise(int degrees, int speed) {
  resetCounters();
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  setMotorSpeed(speed);
  moveStep(degrees);
}

void FullRadar() {
  for(int i=0;i<=180;i=i+10) {
    Serial.print(i);
    Serial.print(", ");
    float myPoint = PointRadar(i);
    Serial.println(myPoint);
  }
}

void serialEvent() {
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        commandstring += inChar;
        if (inChar == '\n' {
            stringComplete = true;
        }
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (stringComplete) {
    command = commandstring.charAt(0)
    parameter = commandstring.substring(1)
    switch (command) {
        case 'f':
            forward(parameter,96);
            break;
        case 'b':
            backward(parameter, 96);
            break;
        case 'c':
            clockwise(parameter, 96 );
            break;
        case 'a':
            anticlockwise(parameter,96);
            break;
        case 'r':
            FullRadar();
            break;
        default:
            Serial.println("Unknown command")
            break;
    }
    commandstring = "";
    stringComplete = false;
  }
}
