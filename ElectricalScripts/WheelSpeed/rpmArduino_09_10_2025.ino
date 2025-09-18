// Hall Effect Sensor RPM Measurement
// Arduino Uno, Hall sensor on pin 2
// 1 magnet on the rotating shaft
// Prints RPM to Serial Monitor

const int hallPin = 2;   // Hall effect sensor pin
const int rpmAvgTime = 2000; //ms
volatile int timedCount = rpmAvgTime;

volatile unsigned long lastPulseTime = 0;
volatile unsigned long pulseInterval = 0;

volatile unsigned long risingTime = 0;
volatile unsigned long fallingTime = 0;
volatile int pulseCount  = 0;

const unsigned long debounceDelay = 60;  // ms (adjust if needed)

void setup() {
  Serial.begin(9600);
  pinMode(hallPin, INPUT);

  // Trigger on rising edge (magnet detected)
  attachInterrupt(digitalPinToInterrupt(hallPin), hallRising, RISING);
  // attachInterrupt(digitalPinToInterrupt(hallPin), hallFalling, FALLING);
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
  

  Serial.print("Pulse Interval (ms): ");
  Serial.print(interval);
  Serial.print("  -> Instant RPM: ");
  Serial.println(rpm);

  if (timedCount == 0) {
    int timedRPM = pulseCount*(60*60000.0/rpmAvgTime);
    pulseCount = 0;
    Serial.print(" -> Timed RPM: ");
    Serial.println(timedRPM);
  }
  

  timedCount = rpmAvgTime - 500;
  delay(500); // update twice per second
  // }
}

void hallRising() {
  unsigned long now = millis();

  // Debounce: ignore pulses that are too close together
  if (now - fallingTime > debounceDelay) {

    pulseInterval = now - risingTime;
    lastPulseTime = now;
    risingTime = now;
    fallingTime = now;
  }
}


