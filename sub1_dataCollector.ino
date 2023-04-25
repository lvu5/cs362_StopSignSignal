#include <SoftwareSerial.h>
#include <NewPing.h>
// #include <Tone.h>

// Define the transmission pins and baud rate
#define NUM_SONAR 4
const int RX_PIN = A0;
const int TX_PIN = A1;
const int BAUD_RATE = 9600;
const int threshold = 1; // in centimeters

// SoftwareSerial receiverSerial(RX_PIN, TX_PIN);



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

// void playTone(int duration) {
//   digitalWrite(buzzer_pin, HIGH);
//   delay(duration);
//   digitalWrite(buzzer_pin, LOW);
//   delay(duration);
// }

void setup() {
    Serial.begin(BAUD_RATE);
    pinMode(buzzer_pin, OUTPUT);
    // receiverSerial.begin(BAUD_RATE);
    // for(int i = 2; i < 10; i++){
    //     pinMode(i, OUTPUT);
    // }
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

    // Serial.println(min_index);
    // Serial.println(dist_arr[2]);
    // Serial.print("The shortest object detect from ");
    // Serial.println(direction_arr[min_index]);
    // Serial.println(min_index);
    // Serial.println(dist_arr[min_index]);


    // Serial.println(calc_distance(sonar[0]));


    if (dist_arr[min_index] < 5 && dist_arr[min_index] > 0) {
        // Serial.println("Hello sent");
        // object detected
        
        sendData(direction_arr[min_index], min_index);
        
        // playTone(100);
        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            tone(buzzer_pin, 1000);
            delay(500);
            noTone(buzzer_pin);
        }
    }
}

void sendData(char direction, int min_index) {
    if (direction != prev || abs(dist_arr_prev[min_index] - dist_arr[min_index]) > threshold) {
            dist_arr_prev[min_index] = dist_arr[min_index];
            prev = direction;
            Serial.write(direction);
    }
}
