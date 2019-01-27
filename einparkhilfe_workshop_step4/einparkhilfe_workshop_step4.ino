int pinWithLED = 9;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(pinWithLED, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(pinWithLED, HIGH);   // turn the LED on
  delay(100);                        // wait 0.1 second
  digitalWrite(pinWithLED, LOW);    // turn the LED off
  delay(200);                        // wait 0.2 second
}
