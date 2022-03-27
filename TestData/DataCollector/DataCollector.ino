#define DATA_PIN A1

void setup() {
  Serial.begin(115200);
  pinMode(DATA_PIN, INPUT);
}

void loop() {
  uint16_t dataHit = analogRead(DATA_PIN);
  Serial.println(dataHit);
}
