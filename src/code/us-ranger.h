void setup() {}

void loop() {
  pinMode(PIN_A, OUTPUT);
  digitalWrite(PIN_A, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_A, HIGH);
  delayMicroseconds(5);
  digitalWrite(PIN_A, LOW);
  pinMode(PIN_A, INPUT);
  long distance = pulseIn(PIN_A, HIGH) / 29 / 2;
  Spine.write(connector, 0, distance);
}
