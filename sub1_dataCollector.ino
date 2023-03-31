#include <QueueList.h>

// Using NESW ask a direction of sensor
const int N_trigPin = 2; // Trigger Pin of Ultrasonic Sensor
const int N_echoPin = 3; // Echo Pin of Ultrasonic Sensor

const int E_trigPin = 4; 
const int E_echoPin = 5; 

const int S_trigPin = 6; 
const int S_echoPin = 7; 

const int W_trigPin = 8; 
const int W_echoPin = 9; 

// Initialize a queue to store the sensor readings
QueueList<String> readingsQueue;

// Calculate distance between object and sensor
float getDistance(int trigPin, int echoPin) {
  // Send a trigger pulse to start the measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the duration of the echo pulse
  long duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance using the speed of sound (343 meters per second)
  float distance = duration / 2.0 * 0.0343; // distance in meters
  
  return distance;
}

// Calculating the distance to the object
// Returns the time it takes for the ultrasonic 
// waves to travel from the sensor to the object and back
long getTimeOfArrival(int trigPin, int echoPin) {
  // Send a trigger pulse to start the measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the duration of the echo pulse
  long duration = pulseIn(echoPin, HIGH);
  
  return duration;
}

void setup() {
   Serial.begin(9600); // Starting Serial Terminal
}

void loop() {
  // Measure the time of arrival for each sensor
  long N_duration = getTimeOfArrival(N_trigPin, N_echoPin);
  long E_duration = getTimeOfArrival(E_trigPin, E_echoPin);
  long S_duration = getTimeOfArrival(S_trigPin, S_echoPin);
  long W_duration = getTimeOfArrival(W_trigPin, W_echoPin);

  // Determine which sensor detected the object first
  String firstSensor = "";
  long firstDuration = 0;

  if (N_duration < E_duration && N_duration < S_duration && N_duration < W_duration) {
    firstSensor = "North";
    firstDuration = N_duration;
  } else if (E_duration < S_duration && E_duration < W_duration) {
    firstSensor = "East";
    firstDuration = E_duration;
  } else if (S_duration < W_duration) {
    firstSensor = "South";
    firstDuration = S_duration;
  } else {
    firstSensor = "West";
    firstDuration = W_duration;
  }

  // Add the result to the readings queue
  String reading = firstSensor + "," + String(firstDuration);
  readingsQueue.push(reading);

  // Print the readings queue
  Serial.println("Readings queue:");
  for (int i = 0; i < readingsQueue.size(); i++) {
    Serial.println(readingsQueue.get(i));
  }

//   // Send the queue of directions over serial
//   while (!directionsQueue.isEmpty()) {
//     String direction = directionsQueue.pop();
//     Serial.println(direction);
//   }

  // Wait for a brief period before taking the next reading
  delay(100);
}

//long microsecondsToInches(long microseconds) {
   //return microseconds / 74 / 2;
//}
//
//long microsecondsToCentimeters(long microseconds) {
   //return microseconds / 29 / 2;
//}