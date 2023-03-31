#include <Queue.h>

Queue<String> directionsQueue; // Declare a queue of strings

const int N_ledPin = 2;
const int E_ledPin = 3;
const int S_ledPin = 4;
const int W_ledPin = 5;

void setup()
{
  pinMode(N_ledPin, OUTPUT);
  pinMode(E_ledPin, OUTPUT);
  pinMode(S_ledPin, OUTPUT);
  pinMode(W_ledPin, OUTPUT);
  
  // Turn on all LEDs initially
  digitalWrite(N_ledPin, HIGH);
  digitalWrite(E_ledPin, HIGH);
  digitalWrite(S_ledPin, HIGH);
  digitalWrite(W_ledPin, HIGH);

  // Set up serial communication
  Serial.begin(9600);
}

void loop()
{
  // Check if there are any directions in the serial buffer
  while (Serial.available()) {
    // Read the next direction from the serial buffer
    String direction = Serial.readStringUntil('\n');

    // Add the direction to the queue
    directionsQueue.push(direction);
  }

  // Process the next direction in the queue
  if (!directionsQueue.isEmpty()) {
    // Get the first direction in the queue
    String direction = directionsQueue.pop();

    // Turn off the corresponding LED based on the direction
    if (direction == "North") {
      digitalWrite(N_ledPin, LOW);
    } else if (direction == "East") {
      digitalWrite(E_ledPin, LOW);
    } else if (direction == "South") {
      digitalWrite(S_ledPin, LOW);
    } else if (direction == "West") {
      digitalWrite(W_ledPin, LOW);
    }
  }
}