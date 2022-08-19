#define DATA_PIN 2

void setup()
{
    Serial.begin(115200);
    pinMode(DATA_PIN, INPUT);
}

void loop()
{
    uint16_t dataHit = digitalRead(DATA_PIN);
    Serial.println(dataHit);
}
