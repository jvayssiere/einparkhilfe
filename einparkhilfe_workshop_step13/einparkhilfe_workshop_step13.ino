// The library for the DHT11 temperature sensor
#include "DHT.h"

// This #define is required by the DHT library so that it knows
// which type of DHT sensor we have (in our case: DHT 11)
#define DHTTYPE DHT11

// The data line of our DHT11 sensor is connected to pin 12 of the Arduino
const int TEMP_SENSOR = 12;

int temperature = 0;          // In degrees Celcius
int humidity = 0;             // In % humidity

// Initialises the library used for talking to the temperature sensor
DHT dht(TEMP_SENSOR, DHTTYPE);

void setup() {
  // Note: pin TEMP_SENSOR is not initialised here.
  // This is done by the DHT library when it is initialized
  // (see "dht.begin()" further below)
  // The pin TEMP_SENSOR is anyway used alternatively as input
  // AND output since communication to and from the sensor happens
  // on only one wire. See the sensor's datasheet for more details.

  // Initialise the library for accessing the temperature and humidity sensor
  // See tutorial at https://learn.adafruit.com/dht/connecting-to-a-dhtxx-sensor
  // See also the examples under File->Examples->DHT sensor library
  dht.begin();

  Serial.begin(115200);
}

void printValuesOnSerial () {
  Serial.println("--- Measured values ---");
  Serial.print("Time: ");
  Serial.print(millis());
  Serial.println(" ms since start.");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" degrees C.");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %.");
  Serial.println();
}

void loop() {
  // Reads humidity value
  humidity = (int) dht.readHumidity();

  // Reads temperature value
  temperature = (int) dht.readTemperature();

  // Send values to the laptop for debugging purpose
  printValuesOnSerial();

  delay(2000);

  return;
}

