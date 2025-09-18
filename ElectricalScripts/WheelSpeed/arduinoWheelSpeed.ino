// Hall Effect Sensor RPM Measurement
// Arduino Uno, Hall sensor on pin 2
// 1 magnet on the rotating shaft
// Prints RPM to Serial Monitor

# include "SevSeg.h"
SevSeg sevseg;

const int hallPin = 2;   // Hall effect sensor pin
const int analogPin = A0;
const int rpmAvgTime = 2000; //ms
volatile int timedCount = rpmAvgTime;

volatile unsigned long lastPulseTime = 0;
volatile unsigned long pulseInterval = 0;

volatile unsigned long risingTime = 0;
volatile unsigned long fallingTime = 0;
volatile int pulseCount  = 0;

volatile int check = 0;

const unsigned long debounceDelay = 27;  // ms (adjust if needed)

byte numDigits = 4;
byte digitPins[] = {1,3,4,5};
byte segmentPins[] = {6,7,8,9,10,11,12,13};
bool resistorsOnSegments = 0;

void setup() {
  // Serial.begin(9600);
  pinMode(hallPin, INPUT);

  // Trigger on rising edge (magnet detected)
  attachInterrupt(digitalPinToInterrupt(hallPin), hallRising, 2);
  // attachInterrupt(digitalPinToInterrupt(hallPin), hallFalling, FALLING);

  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
}

void loop() {
  unsigned long interval;

  // Copy volatile safely
  // noInterrupts();
  interval = pulseInterval;
 
  // interrupts();

  // if (interval > 0) {
  // Compute RPM: 60,000 ms/min / interval (ms)
  float rpm = 60000.0 / interval;
  
  // bool check = digitalRead(hallPin);

  // if(check == 1){
  //   Serial.print(analogRead(analogPin));
  //   Serial.print(",");
  //   Serial.println(digitalRead(hallPin));
  // }
  // Serial.print("Pulse Interval (ms): ");
  // Serial.print(interval);
  // Serial.print("  -> Instant RPM: ");
  // Serial.println(rpm);
  

  timedCount = rpmAvgTime - 500;
  // delay(500); // update twice per second
  // }
  sevseg.setNumber(rpm,0);
  sevseg.refreshDisplay();
  // delay(1000);
}

void hallRising() {
  unsigned long now = millis();
  check = 1;
  // Serial.print(".");
  // Debounce: ignore pulses that are too close together
  if (now - fallingTime > debounceDelay) {
    // Serial.print("update");
    pulseInterval = now - risingTime;
    
    lastPulseTime = now;
    risingTime = now;
    fallingTime = now;
  }
}


