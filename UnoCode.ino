#include <Servo.h>
Servo servo1;

const int PinServo = 5;
//
const int isrPin = 2;
//
const int PinTrig = 11;
const int PinEcho = 12;
//
const int GreenPin = 10;
const int RedPin = 8;
const int BluePin = 9;
//
int volumeCounter = 0;
int appCounter = 0;
float distance;

void setup(){
        servo1.attach(PinServo);
        //
        pinMode(isrPin, INPUT);
        //SensorProximidad
        pinMode(PinTrig, OUTPUT);
        pinMode(PinEcho, INPUT);
        //Leds
        pinMode(RedPin, OUTPUT);
        pinMode(BluePin, OUTPUT);
        pinMode(GreenPin, OUTPUT);
        //Serial print
        Serial.begin(9600);
        //Pocisión inicial de servo en caso de haberla movido
        servo1.write(90);
        //
        digitalWrite(BluePin, HIGH);
        Serial.println(volumeCounter);
}

void loop(){
        distance = MeasureDistance();
        if (distance <= 9.0){
                dispenserMode();
                volumeCounter ++;
                Serial.print("Usos: ");
                Serial.println(volumeCounter);
                delay(500);
        }
        else delay(100);
        if(volumeCounter == 5) emptyDispenserMode();
}
//
float MeasureDistance(){
        digitalWrite(PinTrig, HIGH);
        delay(1);
        digitalWrite(PinTrig, LOW);
        float time = pulseIn(PinEcho, HIGH);
        float lenght = time / 58.2;
        return lenght;
}
void dispenserMode(){
        Serial.println("Dispensando...");
        digitalWrite(GreenPin, HIGH);
        servo1.write(270);
        delay(1500);
        digitalWrite(GreenPin, LOW);
        servo1.write(90);
        float pivot = MeasureDistance();
        if(pivot <= 9.0) noRepeatMode();
}
void noRepeatMode(){
        digitalWrite(GreenPin, LOW);
        Serial.println("Modo repeticion");
        float pivot = MeasureDistance();
        while (pivot <= 9.0)
        {
                pivot = MeasureDistance();
                delay(200);
        }
        digitalWrite(GreenPin, HIGH);
        delay(100);
        digitalWrite(GreenPin, LOW);
        delay(250);
}
void emptyDispenserMode(){
        Serial.println("Por favor, revise el dispensador");
        while (volumeCounter == 5)
        {
                if (digitalRead(isrPin) == HIGH) volumeCounter = 0;
                digitalWrite(RedPin, HIGH);
                delay(1000);
                if (digitalRead(isrPin) == HIGH) volumeCounter = 0;
                digitalWrite(RedPin, LOW);
                delay(1000);
                if (digitalRead(isrPin) == HIGH) volumeCounter = 0;
                digitalWrite(RedPin, HIGH);
                delay(1000);
                if (digitalRead(isrPin) == HIGH) volumeCounter = 0;
                digitalWrite(RedPin, LOW);
                delay(1000);
                if (digitalRead(isrPin) == HIGH) volumeCounter = 0;
        }
        digitalWrite(RedPin, LOW);
        digitalWrite(GreenPin, HIGH);
        delay(100);
        digitalWrite(GreenPin, LOW);
}