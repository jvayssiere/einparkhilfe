#include <Wire.h>               // the library for I2C communication between the microcontroller and the LCD display
#include <LiquidCrystal_I2C.h>  // The library for controlling the LCD display
#include "DHT.h"                // The library for the DHT11 temperature sensor 

#define DHTTYPE DHT11           // This #define is required by the DHT library so that it knows which type of DHT sensor we have (in our case: DHT 11)

// Initialises the library for talking to the LCD display.
// On the Arduino Uno, SCL is connected to pin A5, SDA to pin A4
// From a programming point of view, the line of code below calls the constructor of the C++ class LiquidCrystal_I2C
// Parameters:
//   - 0x27 is the address of the LCD display on the I2C bus. This is hardwired into the LCD display and cannot be changed.
//   - 16 is the number of columns of the display, 2 is the number of rows.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// The variables below represent the pins of the Arduino that we use for connecting to the various external components.
// Since the value of these variables will not change during the execution of the program,
// we declare them as 'const'. Any attempt at modifying them will throw a compiler error
const int ECHO = 10;
const int TRIGGER = 11;
const int BUZZER = 7;
const int POTI = A1;
const int PUSH_BUTTON = 2;
const int TEMP_SENSOR = 12;
const int GREEN_LED = 5;
const int YELLOW_LED = 6;
const int RED_LED = 9;

// The min and max frequencies that our buzzer can produce
const int MIN_FREQ = 20;
const int MAX_FREQ = 2000;

// Is the measured distance below the minimum distance specified below? Then we issue a continuous beep.
// Is the distance above the maximum? Then no beep at all
// And in-between min and max: beep between 1 and 9 times a second, depending on the distance (the smaller the distance, the faster the beeping)
// In the shortest case, we have 9*100 = 900ms beeps, leaves 10 break of 10ms.
const int MAX_DISTANCE_RED = 10;
const int MAX_DISTANCE_YELLOW = 50;

// The current frequency our buzzer outputs, if it outputs anything.
int freq = MIN_FREQ;          // Initialises this variable to the value MIN_FREQ even before the program starts running.
int temperature = 0;                 // In degrees Celcius
int humidity = 0;             // In % hunidity
int distance = 0;             // In centimeters
boolean isSoundOn = true;     // By default, sound is on

// Initialises the library used for talking to the temperature sensor
// We created an instance of the C++ class DHT and assign it to variable dht
DHT dht(TEMP_SENSOR, DHTTYPE);

void buttonPressed() {
  isSoundOn = !isSoundOn; // The symbol "!" is the logical negation, i.e. it turns TRUE into FALSE and FALSE into TRUE.
}


void printLCDBackground() {
  // This function prints out the part of the text on the LCD display
  // That does not change with the values measured
  lcd.clear();

  // First line
  lcd.setCursor (0, 0); // First number is column number, than line number (0 is first line, 1 is second line)
  lcd.print("D    cm   F");

  // Second line
  lcd.setCursor (0, 1); // First position on the second line
  lcd.print ("T   C H   %");
}

int performDistanceMeasurement() {

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

  int distanceMeasured = lengthOfThePulseInMicroseconds / 58;

  return distanceMeasured;
}

void setup() {

  // First we initialize all the pins used in this program as either input (INPUT), output (OUTPUT),
  // or as input with a pull-up resistor built into the microcontroller (INPUT_PULLUP)
  // For more background information on pin configuration, see https://www.arduino.cc/en/Tutorial/DigitalPins

  // The constant LED_BUILTIN always refers to the pin connected to the on-board LED (on the Arduino Uno, this is pin 13).
  // If you are curious: the constant LED_BUILTIN is not defined in this program but in another file
  // located elsewhere in the Arduino environment: file pins_arduino.h which you can find (for the Arduino Uno)
  // at https://github.com/arduino/ArduinoCore-avr/blob/master/variants/standard/pins_arduino.h
  pinMode (LED_BUILTIN, OUTPUT);

  pinMode (TRIGGER, OUTPUT);            // Pin on which we request a distance measurement from the ultrasound distance sensor
  pinMode (ECHO, INPUT);                // Pin on which we receive a distance measurement from the ultrasound distance sensor
  pinMode (BUZZER, OUTPUT);             // Pin to which the buzzer is connected.
  pinMode (POTI, INPUT);                // Analog input for reading the position of the potentiometer
  pinMode (PUSH_BUTTON, INPUT_PULLUP);  // Reading this pin using digitalRead() will return HIGH when the button is NOT pressed!
  pinMode (GREEN_LED, OUTPUT);          // The pin the green LED is connected to
  pinMode (YELLOW_LED, OUTPUT);         // Same for the yellow LED...
  pinMode (RED_LED, OUTPUT);            // ... and for the red LED.

  // Note: pin TEMP_SENSOR is not initialised here, this is done by the DHT library when it is initiated (see "dht.begin()" further below)
  // The pin TEMP_SENSOR is anyway used alternatively as input AND output since
  // communication to and from the sensor happens on only one wire. See the sensor's datasheet for more details.

  // Initialise the output pins that need to have a default state
  digitalWrite (LED_BUILTIN, LOW);      // Let's not waste power and turn the built-in LED off
  digitalWrite (TRIGGER, LOW);          // Trigger is low by default
  // noTone(BUZZER);            )           // No sound
  digitalWrite (GREEN_LED, LOW);        // All LEDs are off
  digitalWrite (YELLOW_LED, LOW);
  digitalWrite (RED_LED, LOW);

  // Registers an interrupt for the push button: the function buttonPressed will be called everytime the button is pressed,
  // i.e. when the value of the digital input for pin PUSH_BUTTON goes from HIGH to LOW (which is why we use FALLING as the interrupt mode)
  // This happens regardless of where the program is at when the interrupt comes in
  // For more information on interrupts, see https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
  attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON), buttonPressed, FALLING);

  // Initialise the library for accessing the temperature and humidity sensor
  // See tutorial at https://learn.adafruit.com/dht/connecting-to-a-dhtxx-sensor
  // See also the examples under File->Examples->DHT sensor library
  dht.begin();

  // Now let's initialize the LCD display
  lcd.begin();          // Initialises the library for the LCD display
  lcd.backlight();      // Turns on the backlight of the LCD display
  printLCDBackground(); // Prints static text of the display

  // Initialise the serial port communication with the laptop
  // This is only here for debuging purpose
  // 115200 is a good speed that should work everywhere
  Serial.begin(115200);
}

void printValuesOnSerial () {

  Serial.println("--- Measured values ---");
  Serial.print("Time: ");
  Serial.print(millis());
  Serial.println(" ms since start.");
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm.");
  Serial.print("Frequency: ");
  Serial.print(freq);
  Serial.println(" Hz.");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" degrees C.");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %.");
  Serial.print("Sound: ");
  Serial.println(isSoundOn);
  Serial.println();
}

void updateLEDs() {
  if (distance < MAX_DISTANCE_RED) {
    digitalWrite (GREEN_LED, LOW);
    digitalWrite (YELLOW_LED, LOW);
    digitalWrite (RED_LED, HIGH);
  } else if ((distance >= MAX_DISTANCE_RED) && (distance < MAX_DISTANCE_YELLOW)) {
    digitalWrite (GREEN_LED, LOW);
    digitalWrite (YELLOW_LED, HIGH);
    digitalWrite (RED_LED, LOW);
  } else { // If sound is turned off
    digitalWrite (GREEN_LED, HIGH);
    digitalWrite (YELLOW_LED, LOW);
    digitalWrite (RED_LED, LOW);
  }
}

void updateLCDDisplay() {

  // Updates the LCD display
  // Displays distance
  lcd.setCursor(2, 0);  // Sets the cursor on the leftmost character of the 3 characters set aside for displaying the distance
  lcd.print("   "); // Clears the part of the display where we want to display distance
  // Depending on the number of characters in the value, set the cursor further to the right so that the value is printed left-aligned
  if (distance < 10) {
    lcd.setCursor (4, 0);
  } else if (distance < 100) {
    lcd.setCursor (3, 0);
  } else {
    lcd.setCursor(2, 0);
  }
  lcd.print (distance);

  // Displays frequency
  lcd.setCursor(12, 0);
  lcd.print("    ");
  lcd.setCursor(12, 0);
  lcd.print(freq);

  // Displays whether sound is on or off
  lcd.setCursor(13, 1);
  if (isSoundOn) {
    lcd.print(" ON"); // Note the leading space character so that overwriting "OFF" with "ON" does not display "OON".
  } else {
    lcd.print("OFF");
  }

  // Displays temperature
  lcd.setCursor(2, 1);
  lcd.print(temperature);

  // Displays humidity
  lcd.setCursor(8, 1);
  lcd.print(humidity);
}


void loop() {

  // Reads the position of the potentiometer (poti) and updates the frequency of the buzzer accordingly somewhere between MIN_FREQ and MAX_FREQ
  // The function analogRead returns a value between 0 and 1023. That's because the ADC (Analog Digital Converter)
  // of the microcontroller has a resolution of 10 bits hence 2^10 = 1024 possible values.
  // The function "map" scales the value between 0 and 1023 linearly to a value between MIN_FREQ and MAX_FREQ
  freq = map (analogRead(POTI), 0, 1023, MIN_FREQ, MAX_FREQ);

  // Reads the distance
  // This can take some time, depending on the distance measured
  // Typical values: if a few centimeters: <1ms. For a few meters: up to 20ms.
  distance = performDistanceMeasurement();
  Serial.println(distance);
  
  if (distance < 0) {
    // In some situations, experience shows that the value returned is negative.
    // In this case, simply set it to zero. We assume that "0" means "error has happened"
    distance = 0;
  }
  
  // The humidity sensor is really slow
  // This call can typically takes about 270ms to complete
  // Need to find a way to not do this too often
  humidity = (int) dht.readHumidity();

  // This call is really fast, typically 12us (microseconds)
  temperature = (int) dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  }

  if (distance > 0) {

    // Prints out values on LCD display
    updateLCDDisplay();
    // Send values to the laptop for debugging purpose
    printValuesOnSerial();

    // Update the 'traffic lights' LEDs
    updateLEDs();

    // Beep if distance is smaller than MAX_DISTANCE_RED and the sounds is on
    if ((distance < MAX_DISTANCE_RED) && (isSoundOn)) {
      tone(BUZZER, freq);
    } else {
      noTone(BUZZER);
    }
  }
  
  delay(2000);

  return;
}


