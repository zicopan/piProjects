#include <wiringPi.h>
#include <stdio.h>
#include <sys/time.h>

#define trigPin 4       
#define echoPin 5
#define MAX_DISTANCE 220        // define the maximum measured distance
#define timeOut MAX_DISTANCE*60 // calculate timeout according to the maximum measured distance
#define ledPin 0                // define the LED

// from FreeNove #url: 
int pulseIn(int pin, int level, int timeout);
float getSonar(){   
    // get the measurement results of ultrasonic module in units of cm
    long pingTime;
    float distance;
    digitalWrite(trigPin,HIGH); //trigPin send 10us high level 
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    pingTime = pulseIn(echoPin,HIGH,timeOut);   //read plus time of echoPin
    distance = (float)pingTime * 340.0 / 2.0 / 10000.0; // since the sound speed is 340m/s
    return distance;
}
// Program Start
int main(){
    if(wiringPiSetup() == -1){
        printf("setup failed");
        return 1; 
    }
    pinMode(ledPin, OUTPUT);
    float distance;
    pinMode(trigPin,OUTPUT);
    pinMode(echoPin,INPUT);
    while(1){
        distance = getSonar();
        if(distance < 2){
			printf("Someone (or something?) is behind you!\n");
			printf("%.1f m\n",distance/100);
			printf("LED on\n\n");
			digitalWrite(ledPin, HIGH);
			}
		else{digitalWrite(ledPin, LOW);}
        delay(1000);
    }   
    return 1;
}

// from FreeNove #url: 
int pulseIn(int pin, int level, int timeout)
{
   struct timeval tn, t0, t1;
   long micros;
   gettimeofday(&t0, NULL);
   micros = 0;
   while (digitalRead(pin) != level)
   {
      gettimeofday(&tn, NULL);
      if (tn.tv_sec > t0.tv_sec) micros = 1000000L; else micros = 0;
      micros += (tn.tv_usec - t0.tv_usec);
      if (micros > timeout) return 0;
   }
   gettimeofday(&t1, NULL);
   while (digitalRead(pin) == level)
   {
      gettimeofday(&tn, NULL);
      if (tn.tv_sec > t0.tv_sec) micros = 1000000L; else micros = 0;
      micros = micros + (tn.tv_usec - t0.tv_usec);
      if (micros > timeout) return 0;
   }
   if (tn.tv_sec > t1.tv_sec) micros = 1000000L; else micros = 0;
   micros = micros + (tn.tv_usec - t1.tv_usec);
   return micros;
}
