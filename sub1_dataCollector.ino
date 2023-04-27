/*
   1- Author:   Hoang Bao Nghi Bien - hbien2
                Linh Vu - lvu5
   2- Project Title: Stop sign crossing priority
   3- Description: Subsystem 1
    This system will be used as a sender.
    This system will be the data collector to detect whenever the car arrives at the stop sign intersection 
    and collect the direction of the arriving car (East, West, North or South). 
    It will send this data to the receiver to handle the lights and cars' priorities. 
    An ultrasonic sensor will be the input device to measure the object's movement. 
    The sensor will detect when the car arrives. The output is the piezo speaker which will ring when the sensor detects a car.
    This Arduino will process and save the data and send them over to the second Arduino.
    References:
    https://www.arduino.cc/reference/en/libraries/newping/
    CS 362 labs
*/


#include <SoftwareSerial.h>
#include <NewPing.h>


// Define the transmission pins and baud rate
#define NUM_SONAR 4
const int RX_PIN = A0;
const int TX_PIN = A1;
const int BAUD_RATE = 9600;
const int threshold = 1; // in centimeters

const int buzzer_pin = 10;
unsigned long previousMillis = 0;
const long interval = 500;

const int N_trigPin = 2;
const int N_echoPin = 3;

const int E_trigPin = 4;
const int E_echoPin = 5;

const int S_trigPin = 6;
const int S_echoPin = 7;

const int W_trigPin = 8;
const int W_echoPin = 9;

const int north = 0;
const int west = 1;
const int south = 2;
const int east = 3;

const char direction_arr[4] = {'n','w','s','e'};

NewPing sonar[NUM_SONAR] = {
    NewPing(N_trigPin, N_echoPin, 30),
    NewPing(W_trigPin, W_echoPin, 30),
    NewPing(S_trigPin, S_echoPin, 30),
    NewPing(E_trigPin, E_echoPin, 30)
};

unsigned int dist_arr[NUM_SONAR] = {0};
unsigned int dist_arr_prev[NUM_SONAR] = {0};

const int N_buzzer = 10;
const int E_buzzer = 11;
const int S_buzzer = 12;
const int W_buzzer = 13;

int dist_n = INT32_MAX;
int dist_e = INT32_MAX;
int dist_s = INT32_MAX;
int dist_w = INT32_MAX;
int distance = 0;

String SerialData = "";
char data;
char curr;
char prev = 'a';


void setup() {
    Serial.begin(BAUD_RATE);
    pinMode(buzzer_pin, OUTPUT);
}

unsigned int calc_distance(NewPing sensor) {
    return sensor.ping() / US_ROUNDTRIP_CM;
}

int min_distance(){
    // take the index
    int min = INT16_MAX;
    int min_index = -1;
    for (int i = 0; i < NUM_SONAR; i++) {
        if (min > dist_arr[i] && dist_arr[i] > 0){
            min_index = i;
            min = dist_arr[i];
        }
    }
    return min_index;
}

void loop() {
    delay(50);
    unsigned long currentMillis = millis();
    for(int i = 0; i < NUM_SONAR; i++){
        dist_arr[i] = calc_distance(sonar[i]);
        if (dist_arr[i] == 0)
            dist_arr_prev[i] = 0;
    }
    int min_index = min_distance();     

    // we send if it is less than 5 cm
    if (dist_arr[min_index] < 5 && dist_arr[min_index] > 0) {
        // object detected
        // then we send the direction to the controller (subsystem2)
        sendData(direction_arr[min_index], min_index);
        if (currentMillis - previousMillis >= interval) {
            // to make the buzzer rings concurrently
            previousMillis = currentMillis;
            tone(buzzer_pin, 1000);
            delay(500);
            noTone(buzzer_pin);
        }
    }
}

void sendData(char direction, int min_index) {
    // this if statement is for preventing re-sending
    // for example if the car keep staying there, it won't resend a signal twice.
    if (direction != prev || abs(dist_arr_prev[min_index] - dist_arr[min_index]) > threshold) {
            dist_arr_prev[min_index] = dist_arr[min_index];
            prev = direction;
            Serial.write(direction);
    }
}
