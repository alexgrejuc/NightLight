/******************
 * Lights up if room is dark, varies intensity of light as you get closer
 * Simple solution to problem of coming into room at night while roommate is asleep
 * Provides light while I am moving without waking roommate
 * Turns off it no motion sensed for five seconds
 */

//sonar pins 
const int trigPin = 8;
const int echoPin = 9; 

//used to see if dark in room
const int photoResistorPin = A0;

//max amount of light in room where the LED will be on 
const int minLight = 150; 

const int ledPin = 11; 
int ledIntensity; 

int distance = 0; 
int distanceLast = -1;

//used for calibration
int distanceMax = 0;

long duration;

//used to see if there is movement (which is reflected by a change in sonar data)
int loopTime;
int time1;
int time2; 
int timeSinceMovement = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  //find max distance
  //used to properly vary light as someone gets closer
  //Also used to find the wall so it can turn off if there is not movement 
  while (millis() < 2000){
    Serial.println("."); 
    distance = calcDistance();

     if (distance > distanceMax){
        distanceMax = distance; 
     }
  }
}

void loop() {
  time1 = millis();
  
  if (analogRead(photoResistorPin) < minLight){

    //used to detect movement
    distanceLast = distance;
         
    distance = calcDistance();
       
    //to account for error in sensor reading
    if (distance >= distanceMax){
      distance = distanceMax;
      timeSinceMovement += loopTime;
    }
      else if (abs(distanceLast - distance) <= 5){ //5 is a number I determined that takes the radar imprecision into account
        timeSinceMovement += loopTime;
      }
      else {
        timeSinceMovement = 0;
      }
      
      Serial.println(timeSinceMovement);

      if (timeSinceMovement <= 5000){
        //1(very light glow)to 255 (max value for pulse width modulation on Arduino)
        ledIntensity = map(distance, 0, distanceMax, 1, 255);  
        analogWrite(ledPin, ledIntensity);
      }
      else{
        analogWrite(ledPin, 0);
      }
  }
  else {
     analogWrite(ledPin, 0); 
  }

  time2 = millis();
  loopTime = time2 - time1;
}

//code for sonar sensor found online
int calcDistance() {

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH); //reads echoPin, returns time in microseconds
    distance = duration * (0.034 / 2); //multiples by speed sound ^-4 (to give centimeters as distance) 
    
    return distance; 
}


