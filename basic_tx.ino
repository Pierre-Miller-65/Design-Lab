//Basic code for TX (transmitter) side. 
//This code will send the position of the joystick and the status of the button.
//John Soupir, 11-09-2020
//adapted by Pierre Miller 11/15/2020
//completed on 03/20/2021

//Include RF24 library for radio. Must be installed. (Use library manager)
#include <SPI.h>
#include <RF24.h>

//Pins for stick and button
int xaxis = A2;
int yaxis = A3;
int button = 8;

//Pins for CE and CSN connection.
RF24 radio(9,10);
const byte address[6] = "1452"; //address (must be unique for each pair)

void setup() {
      pinMode(button, INPUT);
      Serial.begin(9600);
      radio.begin();
      Serial.println("Checking radio connection");
      bool check = radio.isChipConnected();
      Serial.print("check");
      Serial.println(check);
      radio.openWritingPipe(address);
      radio.setPALevel(RF24_PA_HIGH);
      radio.stopListening();
}
void loop() {
      int message[4]; //Array to store data
      //Store data in array
      message[0] = analogRead(xaxis);
      message[1] = analogRead(yaxis);
      message[2] = digitalRead(button);
      message[3] = digitalRead(button);//Extra position in array for adding another control. Using button as place holder.
      //Send the array
      radio.write(&message,sizeof(message));
      delay(100);       //Time between transmissions
}
