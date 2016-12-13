#include <Average.h>
#include <math.h>

/*
 HC-SR04 Ping distance sensor:
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino pin 7 
 Trig to Arduino pin 8
 
 This sketch originates from Virtualmix: http://goo.gl/kJ8Gl
 Has been modified by Winkle ink here: http://winkleink.blogspot.com.au/2012/05/arduino-hc-sr04-ultrasonic-distance.html
 And modified further by ScottC here: http://arduinobasics.blogspot.com.au/2012/11/arduinobasics-hc-sr04-ultrasonic-sensor.html
 on 10 Nov 2012.
 */


#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin
#define LEDPin 13 // Onboard LED

// Number of distance reads to build the average on
#define DIST_RDS_TO_AVG 10
// Multiplier of standard deviation used as an absolute distance from stddev to trim distance measure outliars
#define DIST_OUTL_STDDEV_MULT 2

#define AirTemp 23.2 // Degrees celsius

int maximumRange = 3000; // Maximum range needed
int minimumRange = 40; // Minimum range needed

float dist, soundSpeed = 331.3 + 0.606 * AirTemp; // Sound speed in meters per second

void setup() {
 Serial.begin (9600);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
}

void loop() {
  

// if (distance >= maximumRange || distance <= minimumRange){
//   /* Send a negative number to computer and Turn LED ON 
//   to indicate "out of range" */
//   Serial.println("-1");Mmm
//   digitalWrite(LEDPin, HIGH); 
// } else {
   /* Send the distance to the computer using Serial protocol, and
   turn LED OFF to indicate successful reading. */
//   Serial.print("Duration ");
//   Serial.print(duration);
//   Serial.print(" Distance ");

   dist = getDistanceAvg();
//   delay(50);
//   dist = getDistanceRead();
   
   if (dist < minimumRange) {
     Serial.println(0);
   } else if(dist > maximumRange) {
     Serial.println("INFINITE");
   } else {
     Serial.println(dist);
   }
  
   digitalWrite(LEDPin, LOW); 
// }
 
 //Delay 50ms before next reading.
 //delay(50);
}

float getDistanceAvg() {
  Average<float> ave(DIST_RDS_TO_AVG), aveClean(DIST_RDS_TO_AVG);
  float average, absStddevLimit;
  
  for (int i=0; i < DIST_RDS_TO_AVG; i++) {
    ave.push(getDistanceRead());
    delay(50);
  }
  
  average = ave.mean();
  absStddevLimit = ave.stddev() * DIST_OUTL_STDDEV_MULT;
  for (int i=0; i < ave.getCount(); i++) {
    if (abs(ave.get(i) - average) <= absStddevLimit) {
      aveClean.push(ave.get(i));
    }
  }
  
  return aveClean.mean();
}

float getDistanceRead() {
 long duration, distance; // Duration used to calculate distance
 
 
 /* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
// Calculate the distance (in mm) based on the speed of sound.
// distance = duration/2.913752914/2;
 distance = duration * (soundSpeed/1000) * 0.5;
 
 return distance;
}
