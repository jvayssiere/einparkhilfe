// The variables below represent the pins of the Arduino
// that we use for connecting to the various external components.
// Since the value of these variables will not change during
// the execution of the program, we declare them as 'const'.
const int GREEN_LED = 5;
const int YELLOW_LED = 6;
const int RED_LED = 9;

const int ECHO = 10;
const int TRIGGER = 11;

const int BUZZER = 7;

void setup() {
  // Pin on which we request a distance measurement
  // from the ultrasound distance sensor
  pinMode (TRIGGER, OUTPUT);

  // Pin on which we receive a distance measurement
  // from the ultrasound distance sensor
  pinMode (ECHO, INPUT);

  // Our three LEDs
  pinMode (GREEN_LED, OUTPUT);
  pinMode (YELLOW_LED, OUTPUT);
  pinMode (RED_LED, OUTPUT);

  // Trigger is low by default
  digitalWrite (TRIGGER, LOW);

  // Pin to which the buzzer is connected.
  pinMode (BUZZER, OUTPUT);
  // Buzzer silent by default
  noTone(BUZZER);

  // Initialises the serial connection with the laptop
  Serial.begin(115200);
}

// This function reads the distance from the ultrasound distance
// sensor and returns it as an integer number of centimeters

int readDistance() {
  // Triggers a measurement by holding the trigger line
  // high for 15 microseconds (minimum is 10)
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(15);
  digitalWrite(TRIGGER, LOW);

  // Waits for a high pulse on the echo line
  // (i.e. line going from low to high then back to low)
  // and measure its duration in microseconds
  int lengthOfThePulseInMicroseconds = pulseIn(ECHO, HIGH);

  // According to the datasheet, dividing the length of the pulse in microseconds
  // by 58 gives us the distance measured in centimeters.
  // Since the result of the division operation below is stored in a variable
  // of type 'int', we only keep the integer part of the number
  // (we discard anything after the decimal point)
  int distanceMeasured = lengthOfThePulseInMicroseconds / 58;

  return distanceMeasured;
}

void loop() {
  int distance = readDistance();

  // Prints out distance measured to the serial output
  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.print(" cm. ");

  if (distance < 10) {
    // Red LED is on, all other LEDs are off
    digitalWrite (GREEN_LED, LOW);
    digitalWrite (YELLOW_LED, LOW);
    digitalWrite (RED_LED, HIGH);
    Serial.println ("Buzzer is on. Red is on.");
    tone(BUZZER, 440); // Buzzer is on
  } else if ((distance >= 10) && (distance < 50)) {
    // Yellow LED is on, all other LEDs are off
    digitalWrite (GREEN_LED, LOW);
    digitalWrite (YELLOW_LED, HIGH);
    digitalWrite (RED_LED, LOW);
    Serial.println ("Buzzer is off. Yellow is on.");
    noTone(BUZZER); // Buzzer is off
  } else {
    // Green LED is on, all other LEDs are off
    digitalWrite (GREEN_LED, HIGH);
    digitalWrite (YELLOW_LED, LOW);
    digitalWrite (RED_LED, LOW);
    Serial.println ("Buzzer is off. Green is on.");
    noTone(BUZZER); // Buzzer is off
  }

  delay(500);

  return;
}
