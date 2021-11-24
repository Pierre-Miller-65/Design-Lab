 //Basic code for RX (reciver) side. 
//This only controls forward/backward and left/right with no speed control or seering mixing. 
//For mixed steering use somthing like speed = speed +/- steering. The map function is very useful.
//The abs() funciton is also useful depending how you do things.
//John Soupir, 11-09-2020
//adapted by Pierre Miller 11/15/2020
//completed 03/20/2021

//Include RF24 library for radio. Must be installed. (Use library manager)
#include <SPI.h>
#include <RF24.h>

//set pins for CE and CSN connection
RF24 radio(9, 10); // CE, CSN pins
const byte address[6] = "1452"; //address (must be unique for each pair)

//Pins for motors
//Motor 1 is IN1 and IN2
//Motor 2 is IN3 and IN4
int IN1 = 7;
int IN2 = 8;
int IN3 = 4;
int IN4 = 6;


//Pins for speed control
int EN1 = 3; //note, you must use PWM pins if you want speed control.
int EN2 = 5;

//set to center to prevent car from running away
int throttle = 128;
int steering = 128;

void setup() {
      //set pins to output
      pinMode(IN1,OUTPUT);
      pinMode(IN2,OUTPUT);
      pinMode(IN3,OUTPUT);
      pinMode(IN4,OUTPUT);
      pinMode(EN1,OUTPUT);
      pinMode(EN2,OUTPUT);
      //open serial and print message
      Serial.begin(9600);
      Serial.print("POWER ON");
      //Start radio stuff
      radio.begin();
      Serial.println("Checking radio connection");
      bool check = radio.isChipConnected();
      Serial.print("Status:");
      Serial.println(check);
      radio.openReadingPipe(0, address);
      radio.setPALevel(RF24_PA_MIN);
      radio.startListening();
}
void loop() {
      if (radio.available()) {
            Serial.print("Hello World");
            int message[4]; //Array to hold input
            radio.read(&message, sizeof(message));
            //values sent as array are printed via serial
            int i;
            for (i = 0; i < 4; i = i + 1) {
                  Serial.print(message[i]);
                  Serial.print(throttle);
                  Serial.print(steering);
            }
            Serial.println();

//map values to 0-255 for use with PWM later
throttle = map(message[1],0,1023,0,255);
steering = map(message[0],0,1023,0,255);

//set speed to full (For speed control you would use analogWrite(EN1, blah) where blah is 0-255. 
digitalWrite(EN1, HIGH);
digitalWrite(EN2, HIGH);

//backing up
if (throttle < 120) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

  //turning left
  if (steering < 120) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

  } //turning right
  else if (steering > 130) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    }     
 }

  //going forward
  else if (throttle > 130) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

  //turning left
  if (steering < 120) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

  } //turning right
  else if (steering > 130) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    }  
  }
//Stop
  else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
   }

 }
}
