
const int ECHO = 10;
const int TRIGGER = 11;

void setup() {
  pinMode (TRIGGER, OUTPUT);            // Pin on which we request a distance measurement from the ultrasound distance sensor
  pinMode (ECHO, INPUT);                // Pin on which we receive a distance measurement from the ultrasound distance sensor

  digitalWrite (TRIGGER, LOW);          // Trigger is low by default

  Serial.begin(115200);
}

void loop() {

  // Triggers a measurement by holding the trigger line high for 15 microseconds (minimum is 10)
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(15);
  digitalWrite(TRIGGER, LOW);

  // Waits for a high pulse on the echo line (i.e. line going from low to high then back to low)
  // and measure its duration in microseconds
  int lengthOfThePulseInMicroseconds = pulseIn(ECHO, HIGH);

  // According to the datasheet, dividing the length of the pulse in microseconds
  // by 58 gives us the distance measured in centimeters.
  // Since the result of the division operation below is stored in a variable
  // of type 'int', we only keep the integer part of the number (we discard anything after the decimal point)

  int distance = lengthOfThePulseInMicroseconds / 58;

  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.println(" cm.");

  delay(2000);

  return;
}
