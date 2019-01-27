const int GREEN_LED = 5;
const int YELLOW_LED = 6;
const int RED_LED = 9;

void setup() {
  pinMode (GREEN_LED, OUTPUT);
  pinMode (YELLOW_LED, OUTPUT);
  pinMode (RED_LED, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  // Red LED is on, all other LEDs are off
  digitalWrite (GREEN_LED, LOW);
  digitalWrite (YELLOW_LED, LOW);
  digitalWrite (RED_LED, HIGH);
  Serial.println("Red");
  delay (1000);

  // Yellow LED is on, all other LEDs are off
  digitalWrite (GREEN_LED, LOW);
  digitalWrite (YELLOW_LED, HIGH);
  digitalWrite (RED_LED, LOW);
  Serial.println("Yellow");
  delay (1000);

  // Green LED is on, all other LEDs are off
  digitalWrite (GREEN_LED, HIGH);
  digitalWrite (YELLOW_LED, LOW);
  digitalWrite (RED_LED, LOW);
  Serial.println("Green");
  delay (1000);
}

