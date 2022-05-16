
const int trigPin1 = 4; // d2
const int echoPin1 = 5; // d1 - for the pet detection
// defines variables
long duration1;
int distance1;
int i;
int total_level = 100;
int threshold = 50;

#include "Ubidots.h"

const char* UBIDOTS_TOKEN = "BBFF-OZAd01PVVJswycalTXy6Qp4oBDoIbB";  // Put here your Ubidots TOKEN
//BBFF-OZAd01PVVJswycalTXy6Qp4oBDoIbB
const char* WIFI_SSID = "pk";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "pkpkpkpkpk";      // Put here your Wi-Fi password
Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);
int wifi_connection = 0;


const int trigPin2 = 12; //d6
const int echoPin2 = 14; //d5 - for level detection

long duration2;
int distance2;

//int min_food_level = 50;
int openTime = 3; // input in seconds
int waitingTime = 10; // input in seconds
int pet_detected_distance = 20; // input in centimeter
int open_angle = 150; // input in degree

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(trigPin1, OUTPUT); // Sets the trigPin222 as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input

  pinMode(trigPin2, OUTPUT); // Sets the trigPin222 as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input

  Serial.begin(115200);
  Serial.println("Initializing Serial Communication with baud rate - 115200");
  Serial.println("Trying to connect to wifi");

  for (int a = 0 ; a < 5 ; a++) {
    wifi_connection = ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
    if (wifi_connection == 1) {
      Serial.println("Wifi Connected Successfully");
      Serial.println(wifi_connection);
      a = 6;
    }
    else {
      Serial.println("Wifi not connected try reseting / turn on wifi hotspot");
    }
  }
  //Serial.begin(9600); // Starts the serial communication
  myservo.attach(2); //D4 
}
void loop() {

  // Clears the trigPin
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  // Sets the trigPin1 on HIGH state for 10 micro seconds
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration2 = pulseIn(echoPin2, HIGH);
  // Calculating the distance
  distance2 = duration2 * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Level of Jar: ");
  Serial.println(distance2);
  int food_level = 7 ;
  food_level = total_level - distance2;
  if (distance2 > threshold && wifi_connection == 1 ) {
    Serial.println("Low Food Level");
    ubidots.add("Low_food_level", food_level);
    bool bufferSent = false;
    while (bufferSent == false)
    {
      bufferSent = ubidots.send();
      Serial.println("Sending Data to server");
    }
    Serial.println("Data Sent!");
    Serial.println("Server will send a mail to user");
    }

    // Clears the trigPin
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    // Sets the trigPin1 on HIGH state for 10 micro seconds
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration1 = pulseIn(echoPin1, HIGH);
    // Calculating the distance
    distance1 = duration1 * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance1);
    digitalWrite(LED_BUILTIN, HIGH);

    if (distance1 < pet_detected_distance) {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Pet Detected, Opening food lid...");
      for (pos = 0; pos <= open_angle; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(5);                       // waits 15 ms for the servo to reach the position
      }
      Serial.println("Food lid Open!");
      delay(openTime * 1000);
      Serial.println("Closing food lid...");
      for (pos = open_angle; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15 ms for the servo to reach the position
      }
      Serial.println("Pet Got The food");
      delay(1000);
      Serial.println("Pet has to wait for waiting time to get the food again");
      delay(1000);
      Serial.println("Waiting Time Started");
      for (i = 0 ; i <= waitingTime; i += 1) {
        Serial.println(i);
        delay(1000);
      }
      Serial.println("Waiting Time Ended");
      Serial.println("Now,pet can get the food again!");
      delay(2000);
    }
    delay(1000);
  }   
