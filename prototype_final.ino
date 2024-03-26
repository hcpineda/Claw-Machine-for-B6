/* --------------------------------------------------------------------------------------
    Course Code:
    APSC 101
    
    Studio Section:
    L2B
    
    Names: 
    Hector Cortez Pineda
    Diana Hartt
    Simas Babeckas
    Kazu Nakane
    Vrinda Grover
    Nathan Zhang
    
    Date Created : January 17, 2024
    
    ***Code adapted from Arduino Video 04/05 from resources.***
    
   Videos located at:
      https://learning.video.ubc.ca/media/Arduino+05_+Using+Sonar+%26+Serial+Monitor+%28for+detecting+distance%29/0_cn8ntxyu    
    
    Purpose:
    This program uses an ultrasonic distance sensor (sonar)
    and shows its readings on the screen of an attached
    computer, via the Arduino Serial Monitor.
    
    If the distance detected is below a set threshold value, it
    prompts the servo motor to rotate to an "open" or "closed" 
    position until the threshold value is detected once again.
    
    This code uses sample from "Ultrasonic Sensor HC-SR04 and Arduino Tutorial"
    by Dejan Nedelkovski,
    www.HowToMechatronics.com   
      https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
     
-------------------------------------------------------------------------------------- */

#include <Servo.h>

#define VCC_PIN 13
#define TRIGGER_PIN 12    // sonar trigger pin will be attached to Arduino pin 12
#define ECHO_PIN 11       // sonar echo pint will be attached to Arduino pin 11
#define GROUND_PIN 10     //
#define MAX_DISTANCE 200  // maximum distance set to 200 cm
#define THRESHOLD 30      // threshold value for switch detection

// defines variables
long duration;

int distance;

int pos = 0;        // variable to store the servo position

bool open = true;   // boolean variables indicating if the servo is open or closed

bool closed = false;

bool first = true;  // boolean variable indicating if it's the first time loop() has been run

Servo myservo;  // create servo object to control a servo

void setup() {
  
  Serial. begin(9600);            // set data transmission rate to communicate with computer
  
  myservo.attach(9);              // attaches the servo on pin 9 to the servo object
  
  pinMode(GROUND_PIN, OUTPUT);    // tell pin 10 it is going to be an output
  
  pinMode(VCC_PIN, OUTPUT);       // tell pin 13 it is going to be an output
  
  digitalWrite(GROUND_PIN,LOW);   // tell pin 10 to output LOW (OV, or ground)
  
  digitalWrite(VCC_PIN, HIGH) ;   // tell pin 13 to output HIGH (+5V)
  
  pinMode(ECHO_PIN, INPUT) ;  
  
  pinMode(TRIGGER_PIN, OUTPUT) ;

}

//the main looping part of the program, which detects a distance using the sonar and displays it on the Serial Monitor
void loop() {
  
  digitalWrite(TRIGGER_PIN, LOW);   // Clears the trigPin
  
  delayMicroseconds(2);
  
  digitalWrite(TRIGGER_PIN, HIGH);  // Sets the trigPin on HIGH state for 10 micro seconds
  
  delayMicroseconds(20);
  
  digitalWrite(TRIGGER_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  
  distance = duration * 0.034/2;       // Calculating the distance

  // if the distance measurement becomes extremely high, it is likely an error.  
  // Default to a maximum value of MAX_DISTANCE to identify this condition.
  if (distance > MAX_DISTANCE)   
    {distance = MAX_DISTANCE ;}

  //if loop() is being run for the first time since booting up, First() is called
  if(first == true)
    First();
  
  //calls ClosedServo() if the distance detected is less than the threshold and the switch is open
  if(Sense() == 0 && open == true)
    ClosedServo();
  
  //calls OpenServo() if the distance detected is less than the threshold and the switch is open
  else if(Sense() == 0 && closed == true)
    OpenServo();
  
  
  Serial.print("Distance: ") ;      // Prints the distance on the Serial Monitor, for reference.
  Serial.println(distance);
  
}

//this function causes the servo motor to move to its closed state, once prompted.
void ClosedServo() {
  
  Serial.println("Waiting...");   //  indicating message, for tests.
  
  delay(5000);  //  pauses all functions for 5 seconds.
  
  myservo.write(180);   // puts servo in the "closed" position
  closed = true;        // boolean variables "open" and "closed" are set to T and F respectively.
  open = false;
  
  Serial.println("Checkpoint B - Closed");  //  indicating message, for tests.
  
  delay(1000);  //  pauses all function for 1 second.
  
  Serial.println("Ready...");   //  indicating message, for tests.
  
  // while Sense() detects anything, this loop runs (infinite loop)
  while(Sense() < 5) {
    
    //if Sense() detects any distance greater than the threshold, the while loop is broken.
    if(Sense() == 1)
      break;
    
  }
  
}

//this function causes the servo motor to move to its open state, once prompted.
void OpenServo() {
  
  Serial.println("Waiting...");   //  indicating message, for tests.
  
  delay(5000);    //  pauses all functions for 5 seconds.
  
  myservo.write(0);   // puts servo in the "closed" position
  closed = false;        // boolean variables "open" and "closed" are set to T and F respectively.
  open = true;
  
  Serial.println("Checkpoint C - Open");    //  indicating message, for tests.
  
  delay(1000);  //  pauses all function for 1 second.
  
  Serial.println("Ready...");   //  indicating message, for tests.
  
  //  while Sense() detects anything, this loop runs (infinite loop)
  while(Sense() < 5) {
    
    //  if Sense() detects any distance greater than the threshold, the while loop is broken.
    if(Sense() == 1)
      break;
    
  }
  
}

// this function detects distance and returns 0 if it is smaller than the threshold or vice versa if greater.
int Sense() {
  
  digitalWrite(TRIGGER_PIN, LOW);   // Clears the trigPin
  
  delayMicroseconds(2);
  
  digitalWrite(TRIGGER_PIN, HIGH);  // Sets the trigPin on HIGH state for 10 micro seconds
  
  delayMicroseconds(20);
  
  digitalWrite(TRIGGER_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  
  distance = duration * 0.034/2;       // Calculating the distance 
  
  //  returns 1 if the distance is greater than the set threshold value...
  if(distance > THRESHOLD)
    return(1);

  //  ...or 0 is the distance is less than the threshold.
  else if(distance <= THRESHOLD)
    return(0);
  
}

// this function only runs if loop() is running for the first time.
void First() {
  
  Serial.println("Checkpoint A - Starting...");   //  indicating message, for tests.
  
  myservo.write(0); //  sets the servo to its "open" position.
  
  delay(5000);  //  delays all functions for 5 seconds.
  
  first = false;  //  sets the boolean variable "first" to false; it stays like this for the lifetime of the program.
  
  Serial.println("Go!");   //  indicating message, for tests.

}