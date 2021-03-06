#include <MeetAndroid.h>

#include <QueueArray.h>

#define FORWARD 1 
#define BACK 2
#define LEFT 3
#define RIGHT 4
#define CLOCKWISE 5
#define COUNTER_CLOCKWISE 6
#define STOP 0

enum Mood {
  Sad,
  Normal,
  Excited
};

const int EXCITED_SPEED = 255;
const int NORMAL_SPEED = 150;
const int SAD_SPEED = 50;

int E1 = 6; //M1 Speed Control
int E2 = 5; //M2 Speed Control
int M1 = 8; //M1 Direction Control
int M2 = 7; //M2 Direction Control
byte stopInput = false;
byte currentDirection = STOP;
int currentSpeed = 0;
Mood currentMood = Normal;
MeetAndroid meetAndroid;
int onboardLed = 13;

struct ItineraryStep {
  byte direction;
  int speed;
  int time;
};

QueueArray<ItineraryStep> movementQueue;
int nextStepTime = 0;

void startMovement(byte movementDirection, int movementSpeed) {
  if (movementDirection != currentDirection 
    || movementSpeed != currentSpeed) {
      currentDirection = movementDirection;  
      currentSpeed = movementSpeed;  
      changeDirection();
  }
}

void changeDirection() {
  switch (currentDirection) {
    case LEFT:
      analogWrite(E1, currentSpeed);
      digitalWrite(M1, HIGH);
      analogWrite(E2, currentSpeed);
      digitalWrite(M2, LOW);      
      break;
    case RIGHT:
      analogWrite(E1, currentSpeed);
      digitalWrite(M1, LOW);
      analogWrite(E2, currentSpeed);
      digitalWrite(M2, HIGH);
      break;
    case FORWARD:
      analogWrite(E1, currentSpeed);
      digitalWrite(M1, LOW);
      analogWrite(E2, currentSpeed);
      digitalWrite(M2, LOW);
      break;
    case BACK:
      analogWrite(E1, currentSpeed);
      digitalWrite(M1, HIGH);
      analogWrite(E2, currentSpeed);
      digitalWrite(M2, HIGH);
      break;
    case STOP:
      analogWrite(E1, 0);
      analogWrite(E2, 0);
      break;
    case CLOCKWISE:
      analogWrite(E1, currentSpeed);
      digitalWrite(M1, LOW);
      analogWrite(E2, currentspeed / 2);
      digitalWrite(M2, LOW);
      break;
    case COUNTER_CLOCKWISE:
      analogWrite(E1, currentSpeed / 2);
      digitalWrite(M1, LOW);
      analogWrite(E2, currentSpeed);
      digitalWrite(M2,LOW);
      break;
  }
}

/*
void move(byte movementDirection, int movementSpeed, int time) {
  startMovement(movementDirection, movementSpeed);
  delay(time);
}*/


void stopMoving() {
  startMovement(STOP, 0);
}

void executeItineraryStep() {
  if (movementQueue.isEmpty()) {  
    stopMoving();
  } else {
    int now = millis();
    if (now >= nextStepTime) {
      ItineraryStep itineraryStep = movementQueue.pop();
      nextStepTime = now + itineraryStep.time;
      startMovement(itineraryStep.direction, itineraryStep.speed);
    }
  }
}

void queueMovement(byte movementDirection, int movementSpeed, int time) {
  ItineraryStep step;
  step.direction = movementDirection;
  step.speed = movementSpeed;
  step.time = time;
  movementQueue.push(step);
}

class LightControl{
  private: 
    int baseline;
    int threshold;
    
  public:
    LightControl() {
      threshold = 150;
    }
  
    void calibrate(int cycles, int interval){
      int sum = 0;
      int reading = 0;
      int sumOfSquares = 0;
            
      for (int i = 0;i < cycles;i++) {
        reading = analogRead(A0);     
        sum += reading;      
        delay(interval);      
      }
      
      baseline = sum / cycles;
    }
    
    byte isOn() {
      int reading = analogRead(A0);
      return reading - baseline > threshold;      
    }
};

LightControl lightControl;

void setup() {
  
  Serial.begin(9600); 
  
  meetAndroid.registerFunction(testEvent, 'A');  

  //pinMode(onboardLed, OUTPUT);
  //digitalWrite(onboardLed, HIGH);
  for(int i=5;i<=8;i++) {
    pinMode(i, OUTPUT);
  }
  
  lightControl.calibrate(10, 10);
}


void queueDefaultBehavior() {
  int turnDirection = random(0,2);
  int turnDuration = random(700, 3000);
  
  int speed = getMoodSpeed();

  queueMovement(FORWARD, speed, 5000);
  queueMovement(STOP, 0, 500);
  queueMovement(turnDirection ? LEFT: RIGHT, speed, turnDuration);
  queueMovement(FORWARD, speed, 2000);
}

void clearMovementQueue() {
  while (!movementQueue.isEmpty()) {
    movementQueue.pop();
  }
  nextStepTime = millis();
}

void stun() {
  clearMovementQueue();
  queueMovement(STOP, 0, 5000);
}

void playful () {
  queueMovement(FORWARD,255, 5000 );
  queueMovement(turnDirection ? LEFT: RIGHT, 255, 2000);
  queueMovement(FORWARD, 255, 3000);
  queueMovement(turnDirection, 255, 900 );
  queueMovement(FORWARD, 255, 5000);
}

void confused() {
  queueMovement(turnDirection ? LEFT : RIGHT, 255, 3000 );
  queueMovement(turnDirection ? LEFT : RIGHT, 255, 2000 );
  queueMovement(turnDirection ? LEFT : RIGHT, 255, 3000 );
  queueMovement(turnDirection ? LEFT : RIGHT, 255, 2000 );
}


void recoil() {
  stopMoving();
  clearMovementQueue();
  queueMovement(BACK, getMoodSpeed(), 300);
}

void attack() {
  stopMoving();
  clearMovementQueue();

  queueMovement(BACK, 255, 100);
  queueMovement(STOP, 0, 500);
  
  for (int i=0;i<5;i++) {
    queueMovement(LEFT, 255, 200);
    queueMovement(RIGHT, 255, 200);
  }
  
  queueMovement(STOP, 0, 100);
  queueMovement(FORWARD, 255, 1000);
}

int getMoodSpeed() {
  switch(currentMood) {
    case Sad : return SAD_SPEED;
    case Normal: return NORMAL_SPEED;
    case Excited: return EXCITED_SPEED;
    default: return 0;
  }
}


void loop() {
  
  meetAndroid.receive();
  
  if (movementQueue.isEmpty()) {
    queueDefaultBehavior();
    stopInput = false;
  }
  executeItineraryStep();
  
  if (lightControl.isOn() && stopInput == false) {
    stopInput = true;
    attack();
    
  }
  

}

void testEvent(byte flag, byte numOfValues)
{
  char command[255];
  
  meetAndroid.getString(command);

  if (!strcmp(command, "sad")) {
    currentMood = Sad;
  } else if (!strcmp(command, "normal")) {
    currentMood = Normal;
  } else if (!strcmp(command, "excited")) {
    currentMood = Excited;
  } else if (!strcmp(command, "attack")) {
    attack(); 
  } else if (!strcmp(command, "stun")) {
    stun();
  }

}
  



