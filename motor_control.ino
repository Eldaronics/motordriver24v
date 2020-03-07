int potVal = 0;
int PWMOut = 0;
int PWMOut1 = 0;

volatile int motorYonFlag = 0;
volatile int speedControlFlag = 0;
volatile int startStopFlag = 0;

#define inStart 2
#define inYon 3
#define motorDelay  100

#define TLP1 4
#define TLP2 5
#define op1 6
#define op2 7

#define CW digitalWrite(TLP1,0);digitalWrite(op1,1);digitalWrite(TLP2,1);digitalWrite(op2,0)
#define CCW digitalWrite(TLP2,0);digitalWrite(op2,1);digitalWrite(TLP1,1);digitalWrite(op1,0)
#define STP digitalWrite(TLP2,1);digitalWrite(op2,0);digitalWrite(TLP1,1);digitalWrite(op1,0)

void setup() {
  pinMode(TLP1, 1);
  pinMode(TLP2, 1);
  pinMode(op1, 1); 
  pinMode(op2, 1);

  pinMode(inStart, INPUT);
  pinMode(inYon, INPUT);
  attachInterrupt(digitalPinToInterrupt(inYon), prior, CHANGE);
  attachInterrupt(digitalPinToInterrupt(inStart), prior, CHANGE);
  Serial.begin(9600);
}

void loop() {
  if (startStopFlag == 1) {
     analogWrite(11,PWMOut1);
    speedControl(0);
    motorYon();
    delay(30);
  }
  else{
    STP;
  }
}

int speedControl(int pin) {
  potVal = analogRead(pin);
  PWMOut = map(potVal, 0, 1023, 0, 255);

  while ((PWMOut != PWMOut1) && (speedControlFlag == 0)) {
    // int temp = PWMOut;
    PWMOut =  map(analogRead(pin), 0, 1023, 0, 255);
    if (PWMOut > PWMOut1) {
      PWMOut1 += 1;
      analogWrite(11,PWMOut1);
    }
    else {
      PWMOut1 -= 1;
    }
    Serial.println(PWMOut);
    Serial.println(PWMOut1);
    analogWrite(11,PWMOut1);
    delay(motorDelay);
  }
}
void prior() {
  motorYonFlag = 1;
  speedControlFlag = 1;
  if (inStart == 1) {
    startStopFlag = 1;
  }
  else {
    startStopFlag = 0;
  }
  Serial.println("interrupt");
}

void motorYon() {
  if (motorYonFlag == 1) {
    Serial.println("motorYon");
    while (PWMOut1 > 0) {
      PWMOut1--;
      Serial.print("Speed: ");
      Serial.println(PWMOut1);
      analogWrite(11,PWMOut1);
      delay(motorDelay);
    }
    if ((PWMOut1 > 0) && (inYon == 1)) {
      CW;
    }
    else if ((PWMOut1 > 0) && (inYon == 1)) {
      CCW;
    }
    motorYonFlag = 0 ;
    speedControlFlag = 0;
  }
}
