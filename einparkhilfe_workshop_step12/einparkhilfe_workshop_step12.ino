const int PUSH_BUTTON = 2;
const int GREEN_LED = 5;
const int YELLOW_LED = 6;
const int RED_LED = 9;

boolean isSoundOn = true;     // By default, sound is on

// This function is called everytime the push button is pressed down
void buttonPressed() {
  // The symbol "!" is the logical negation,
  // i.e. it turns TRUE into FALSE and FALSE into TRUE.
  isSoundOn = !isSoundOn;
}

void setup() {

  // Reading this pin using digitalRead() will return
  // HIGH when the button is NOT pressed!
  pinMode (PUSH_BUTTON, INPUT_PULLUP);

  pinMode (GREEN_LED, OUTPUT);          // The pin the green LED is connected to
  pinMode (YELLOW_LED, OUTPUT);         // Same for the yellow LED...
  pinMode (RED_LED, OUTPUT);            // ... and for the red LED.

  // Registers an interrupt for the push button: the function buttonPressed
  // will be called everytime the button is pressed, i.e. when the value of
  // the digital input for pin PUSH_BUTTON goes from HIGH to LOW
  // (which is why we use FALLING as the interrupt mode)
  // This happens regardless of where the program is at when the interrupt comes in
  attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON), buttonPressed, FALLING);

  Serial.begin(115200);
}

void loop() {

  // Print value of variable to the serial output
  Serial.print("Sound is: ");
  if (isSoundOn) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }

  // Update LEDs in one go
  digitalWrite(GREEN_LED, isSoundOn);
  digitalWrite(YELLOW_LED, isSoundOn);
  digitalWrite(RED_LED, isSoundOn);

  delay(100);

  return;
}


