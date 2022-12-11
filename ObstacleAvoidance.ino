#include "TimerOne.h"
#include "NewPing.h"
#include <DHT.h>
#define Type DHT11

#define MAX_DISTANCE 400
#define TRIGGER_PIN 3
#define ECHO_PIN 4
int led = 6;
int M1a = 11 ; // motor pin
int M1b = 12 ; // motor pin 
intpotPin = A0; // potentiometer to vary moter speed
intsensePin = 8;

float humidity;
float temperature;
DHT HT(sensePin,Type);

float g=9.8; 
intpotvalue;
intmotorspeed ;

int counter=0;
intrps;
float velocity;

float cof ; // coefficient of friction for road and tire 
float breakingDistance ;


float duration; // Stores HC-SR04 pulse duration value
float distance; // Stores calculated distance in cm
float soundsp; // Stores calculated speed of sound in M/S
float soundcm; // Stores calculated speed of sound in cm/ms
int iterations = 5;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void docount() // counts from the speed sensor
{
  counter++; // increase +1 the counter value
} 

void timerIsr()
{
  Timer1.detachInterrupt(); //stop the timer
  Serial.print("Wheel : "); 
  rps = (counter / 20); // divide by number of holes in Disc
  Serial.print(rps,DEC);  
  Serial.print(" Rotation per seconds , "); 

  velocity = rps *2 * 3.14159 * 1.25 ; // v=wr(cm/sec) , r of wheel = 1.25cm 
  Serial.print(" Speed of car : ");  
  Serial.print(velocity);  
  Serial.print(" cm/sec , ");  

  breakingDistance = (velocity * velocity ) / ( 2 * 9.8 * cof );
  Serial.print(" Breaking Distance : ");  
  Serial.print(breakingDistance);  
  Serial.print(" cm ");  
 
  Serial.print(" , Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if(distance <= breakingDistance)
  {
    Serial.print(" , ALERT !!! ALERT !!!ALERT !!! ");
    digitalWrite(led, HIGH) ;   
    delay(500);
  }

  else
  {
    digitalWrite(led, LOW) ;    
  }

  Serial.println();
  
  counter=0; // reset counter to zero
  Timer1.attachInterrupt( timerIsr ); //enable the timer
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  HT.begin();
  
  pinMode(M1a, OUTPUT); 
  pinMode(M1b, OUTPUT); 
  pinMode(potPin , INPUT);
  pinMode(led, OUTPUT) ;

  delay(2000); // Delay so DHT-22 sensor can stabalize

  humidity = HT.readHumidity();
  temperature = HT.readTemperature();

  if( humidity == 100 ) //coefficient of friction depending on temperature and humidity 
  {
      if( temperature < 0)
      {
          cof = 0.25 ; //snow
      }
      else 
      {
          cof = 0.5 ; //rain 
      }
  }
  else
  {
      cof = 0.7 ;
  }

  Timer1.initialize(1000000); // set timer for 1sec
  attachInterrupt(0, docount, RISING); // increase counter when speed sensor pin goes High
  Timer1.attachInterrupt( timerIsr ); // enable the timer
}

void loop() {
  // put your main code here, to run repeatedly:
  potvalue= analogRead(potPin);
  motorspeed = //(255./1023.)*potvalue;
  map(potvalue, 0, 1023,80, 255);
  analogWrite(M1a, motorspeed); // set speed of motor (0-255)
  digitalWrite(M1b, 0);

  soundsp = 331.4 + (0.606 * temperature) + (0.0124 * humidity); // Calculate the Speed of Sound in M/S
  soundcm = soundsp / 10000; // Convert to cm/ms
  duration = sonar.ping_median(iterations); 
  distance = (duration / 2) * soundcm; // Calculate the distance


}
