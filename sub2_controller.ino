
/*
   1- Author:   Hoang Bao Nghi Bien - hbien2
                Linh Vu - lvu5
   2- Project Title: Stop sign crossing priority
   3- Description: Subsystem 2
    This system will be used as a receiver. 
    The controller will receive the data from the collector and process them to handle the lights according to the laws of the road for traffic lights. 
    The lights will be off initially, it will only turn on to let the car know when it is safe to cross the intersection. 
    There will also be a remote control for emergency vehicles or person who control the traffic (prioritized objects) 
    so when they press the button, all 4 lights will blink repeatedly to let drivers know that they must stop.
    The input devices are the IR receiver and the remote control. 
    The Arduino will receive the input device to receive the data collected from the Arduino above and handle the LEDs. 
    The LEDs are the output. The LED will turn on to notify the driver that it is good to cross the intersection.
    References:
    https://www.arduino.cc/reference/en/libraries/irremote/
    https://circuitdigest.com/microcontroller-projects/interfacing-ir-sensor-module-with-arduino#:~:text=Arduino%20IR%20Sensor%20Interfacing,can%20only%20measure%20infrared%20radiation.
    
*/

#include <IRremote.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>

// Define the transmission pins and baud rate
const int RX_PIN = A0;
const int TX_PIN = A1;
const int BAUD_RATE = 9600;
// const int IR_PIN = 2;  // IR receiver pin

// SoftwareSerial remoteSerial(RX_PIN, TX_PIN);
// IRrecv irrecv(IR_PIN);
// decode_results results;

// Remote
const int RECV_PIN = 10;
IRrecv irrecv(RECV_PIN);
decode_results results;
bool isOn = false;


const int N_red = 2;
const int E_red = 4;
const int S_red = 6;
const int W_red = 8;

void setup() {
    pinMode(N_red, OUTPUT);
    pinMode(E_red, OUTPUT);
    pinMode(S_red, OUTPUT);
    pinMode(W_red, OUTPUT);


    digitalWrite(N_red, LOW);
    digitalWrite(E_red, LOW);
    digitalWrite(S_red, LOW);
    digitalWrite(W_red, LOW);

    Serial.begin(BAUD_RATE);

    irrecv.enableIRIn();  // Start the IR receiver
    irrecv.blink13(true);

    Timer1.attachInterrupt(buttonClick);  // attaches callback() as a timer overflow interrupt
}

void loop() {
    if (Serial.available() > 0) {
        char received = Serial.read();
        controlRedLights(received);
        Serial.println(received);
    }
    if (isOn) {
        blinkAllRedLights();
    } else {
        digitalWrite(N_red, LOW);
        digitalWrite(E_red, LOW);
        digitalWrite(S_red, LOW);
        digitalWrite(W_red, LOW);
    }
}

void controlRedLights(char direction) {
    int redPin = 0;

    switch (direction) {
        case 'n':
            redPin = N_red;
            break;
        case 'e':
            redPin = E_red;
            break;
        case 's':
            redPin = S_red;
            break;
        case 'w':
            redPin = W_red;
            break;
    }
    // redPin is the pin that we will turn on to let car go

    if (redPin != 0) {
        digitalWrite(redPin, HIGH);
        delay(1000);  // Duration for which the light remains off
        digitalWrite(redPin, LOW);
    }
    redPin = 0;
}

void buttonClick() {
    if (irrecv.decode(&results)) {
        delay(1000);
        isOn = !isOn;
        blinkAllRedLights();
        irrecv.resume();  // Receive the next value
    }
}

void blinkAllRedLights() {
    // blink 4 LEDs
    digitalWrite(N_red, LOW);
    digitalWrite(E_red, LOW);
    digitalWrite(S_red, LOW);
    digitalWrite(W_red, LOW);
    delay(200);
    digitalWrite(N_red, HIGH);
    digitalWrite(E_red, HIGH);
    digitalWrite(S_red, HIGH);
    digitalWrite(W_red, HIGH);
    delay(200);
}
