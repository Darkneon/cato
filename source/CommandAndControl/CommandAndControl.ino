/*
  Receives Test Events from your phone.
  After it gets a test message the led 13 will blink
  for one second.
*/
 
#include <MeetAndroid.h>

MeetAndroid meetAndroid;
int onboardLed = 13;

int E1 = 6; //M1 Speed Control
int E2 = 5; //M2 Speed Control
int M1 = 8; //M1 Direction Control
int M2 = 7; //M2 Direction Control

int defaultSpeed = 200;

void setup()  
{
  // use the baud rate your bluetooth module is configured to 
  // not all baud rates are working well, i.e. ATMEGA168 works best with 57600
  Serial.begin(9600); 
  
  // register callback functions, which will be called when an associated event occurs.
  // - the first parameter is the name of your function (see below)
  // - match the second parameter ('A', 'B', 'a', etc...) with the flag on your Android application
  meetAndroid.registerFunction(testEvent, 'A');  

  pinMode(onboardLed, OUTPUT);
  digitalWrite(onboardLed, HIGH);
  for(int i=5;i<=8;i++)
    pinMode(i, OUTPUT);
}

void loop()
{
  meetAndroid.receive(); // you need to keep this in your loop() to receive events
}

void startRobot() {
  analogWrite (E1, defaultSpeed);
  digitalWrite(M1,LOW);
  analogWrite (E2, defaultSpeed);
  digitalWrite(M2,LOW);  
}

void stopRobot() {
  analogWrite (E1, 0);
  digitalWrite(M1,LOW);
  analogWrite (E2, 0);
  digitalWrite(M2,LOW);  
}

void goCrazy() {
  for (int i=0; i<5; i++) {
    analogWrite (E1, 255);
    digitalWrite(M1,LOW);
    analogWrite (E2, 150);
    digitalWrite(M2,HIGH);  
    delay(200);
    analogWrite (E1, 150);
    digitalWrite(M1,HIGH);
    analogWrite (E2, 255);
    digitalWrite(M2,LOW);  

  }  
}



/*
 * This method is called constantly.
 * note: flag is in this case 'A' and numOfValues is 0 (since test event doesn't send any data)
 */
void testEvent(byte flag, byte numOfValues)
{
  char command[255];
  
  meetAndroid.getString(command);
  
  if (!strcmp(command, "start")) {
    startRobot();
  } else if (!strcmp(command, "stop")) {
    stopRobot();
  } else /*if (!strcmp(command, "go crazy"))*/ {
    goCrazy();
  }
}

void flushLed(int time)
{
  digitalWrite(onboardLed, LOW);
  delay(time);
  digitalWrite(onboardLed, HIGH);
  delay(time);
}

