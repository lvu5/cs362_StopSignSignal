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
//

const int N_red = 2;
const int E_red = 4;
const int S_red = 6;
const int W_red = 8;

void setup() {
    pinMode(N_red, OUTPUT);
    pinMode(E_red, OUTPUT);
    pinMode(S_red, OUTPUT);
    pinMode(W_red, OUTPUT);

    // digitalWrite(N_red, HIGH);
    // digitalWrite(E_red, HIGH);
    // digitalWrite(S_red, HIGH);
    // digitalWrite(W_red, HIGH);

    digitalWrite(N_red, LOW);
    digitalWrite(E_red, LOW);
    digitalWrite(S_red, LOW);
    digitalWrite(W_red, LOW);

    Serial.begin(BAUD_RATE);
    // remoteSerial.begin(BAUD_RATE);

    irrecv.enableIRIn();  // Start the IR receiver
    irrecv.blink13(true);

    // Timer1.initialize(1000000);  // initialize timer1, and set a 1/2 second period
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

    // if (direction == 'n')
    //     redPin = N_red;
    // if (direction == 'e')
    //     redPin = E_red;
    // if (direction == 's')
    //     redPin = S_red;
    // if (direction == 'w')
    //     redPin = W_red;

    if (redPin != 0) {
        digitalWrite(redPin, HIGH);
        delay(1000);  // Duration for which the light remains off
        digitalWrite(redPin, LOW);
    }
    redPin = 0;
}

void buttonClick() {
    if (irrecv.decode(&results)) {
        // Serial.println(results.value, HEX);
        delay(1000);
        isOn = !isOn;
        // if (results.value == 0xFF30CF) {  // Change this value to the remote control "1" button code
        blinkAllRedLights();
        // }
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
