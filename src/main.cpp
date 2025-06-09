#include <Arduino.h>
#include <SoftwareSerial.h>

void SendMessage();

// Globals
SoftwareSerial SIM900A(8, 7); // Connect TX and RX
String phone_number = "+63##########"; // Phone number
bool message_sent = false;

// Constants
const int gas_sensor = A0;
const int buzzer = 10;
const int LED_1 = 13;
const int LED_2 = 11;
const int safety_lim = 60; // Sets smoke density safe limit
const int time = 1000;

void setup(){
    pinMode(gas_sensor, INPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);

    digitalWrite(buzzer, LOW);
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);

    // Initializing
    Serial.begin(9600);
    Serial.println("Fire Alarm System Initialized");
    delay(time);
    Serial.println("AT");
    delay(time);
}

void loop(){
    int gas_level = analogRead(gas_sensor);
    Serial.print("Gas Sensor Reading: ");
    Serial.println(gas_level);

    if(gas_level > safety_lim){
    // Fire detected
        tone(buzzer, 500, 1000);
        digitalWrite(LED_1, HIGH);
        digitalWrite(LED_2, HIGH);
        if(!message_sent){
            SendMessage();
            message_sent = true;
        }
        delay(time);
        noTone(buzzer);
        digitalWrite(LED_1, LOW);
        digitalWrite(LED_2, LOW);
    } 
    else{
    // No Fire Detected
        noTone(buzzer);
        digitalWrite(LED_1, LOW);
        digitalWrite(LED_2, LOW);
        message_sent = false;
    }
    delay(time);
}

void SendMessage(){
    Serial.println("AT+CMGF=1");
    delay(time);
    Serial.print("AT+CMGS=\"");
    Serial.print(phone_number);
    Serial.println("\"");
    delay(time);
    Serial.println("ALERT: fire detected in your home!");
    Serial.write(26);
    delay(3000);
}