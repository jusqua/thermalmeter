#define NOTE_B7 3951

typedef struct {
  int g, f, a, b, e, d, c;
} sseg;

static int timelapsed = 0;
const int buzzerPin = A0;
const int thermistorPin = A1;
const sseg tenPin = { 13, 12, 11, 10, 9, 8, 7 };
const sseg onePin = { 6, 5, 4, 3, 2, A5, A4 };

// Get tension at point * from thermistor WZP-PT100 
// 5V----v^v^--*--v^v^----GND
double getTemperature() {
  // Get bits from point
  double thermalRead = analogRead(thermistorPin);
  // A and B coefficient from WZP-PT100, i.e. platinum
  double A = 0.0039083;
  double B = -0.0000005775;
  // Convert to voltage
  double V = 5 * thermalRead / 1023.0;
  // Get resistance
  double R = 220 * (5 - V) / V;
  // Callendar-Van Dusen Equation
  double celsius = ((-A + sqrt(pow(A, 2) - 4 * B * (1 - R / 100))) / (2 * B));
  Serial.print("Temperatura em Celsius: ");
  Serial.println(celsius);
  return celsius;
}

// Set sseg from number, don't laugh at me
void setSsegFromNumber(sseg pins, int number) {
  switch (number) {
    case 0:
      digitalWrite(pins.g, 0);
      digitalWrite(pins.f, 1);
      digitalWrite(pins.a, 1);
      digitalWrite(pins.b, 1);
      digitalWrite(pins.e, 1);
      digitalWrite(pins.d, 1);
      digitalWrite(pins.c, 1);
      break;
    case 1:
      digitalWrite(pins.g, 0);
      digitalWrite(pins.f, 0);
      digitalWrite(pins.a, 0);
      digitalWrite(pins.b, 1);
      digitalWrite(pins.e, 0);
      digitalWrite(pins.d, 0);
      digitalWrite(pins.c, 1);
      break;
    case 2:
      digitalWrite(pins.g, 1);
      digitalWrite(pins.f, 0);
      digitalWrite(pins.a, 1);
      digitalWrite(pins.b, 1);
      digitalWrite(pins.e, 1);
      digitalWrite(pins.d, 1);
      digitalWrite(pins.c, 0);
      break;
    case 3:
      digitalWrite(pins.g, 1);
      digitalWrite(pins.f, 0);
      digitalWrite(pins.a, 1);
      digitalWrite(pins.b, 1);
      digitalWrite(pins.e, 0);
      digitalWrite(pins.d, 1);
      digitalWrite(pins.c, 1);
      break;
    case 4:
      digitalWrite(pins.g, 1);
      digitalWrite(pins.f, 1);
      digitalWrite(pins.a, 0);
      digitalWrite(pins.b, 1);
      digitalWrite(pins.e, 0);
      digitalWrite(pins.d, 0);
      digitalWrite(pins.c, 1);
      break;
    case 5:
      digitalWrite(pins.g, 1);
      digitalWrite(pins.f, 1);
      digitalWrite(pins.a, 1);
      digitalWrite(pins.b, 0);
      digitalWrite(pins.e, 0);
      digitalWrite(pins.d, 1);
      digitalWrite(pins.c, 1);
      break;
    case 6:
      digitalWrite(pins.g, 1);
      digitalWrite(pins.f, 1);
      digitalWrite(pins.a, 1);
      digitalWrite(pins.b, 0);
      digitalWrite(pins.e, 1);
      digitalWrite(pins.d, 1);
      digitalWrite(pins.c, 1);
      break;
    case 7:
      digitalWrite(pins.g, 0);
      digitalWrite(pins.f, 0);
      digitalWrite(pins.a, 1);
      digitalWrite(pins.b, 1);
      digitalWrite(pins.e, 0);
      digitalWrite(pins.d, 0);
      digitalWrite(pins.c, 1);
      break;
    case 8:
      digitalWrite(pins.g, 1);
      digitalWrite(pins.f, 1);
      digitalWrite(pins.a, 1);
      digitalWrite(pins.b, 1);
      digitalWrite(pins.e, 1);
      digitalWrite(pins.d, 1);
      digitalWrite(pins.c, 1);
      break;
    case 9:
      digitalWrite(pins.g, 1);
      digitalWrite(pins.f, 1);
      digitalWrite(pins.a, 1);
      digitalWrite(pins.b, 1);
      digitalWrite(pins.e, 0);
      digitalWrite(pins.d, 1);
      digitalWrite(pins.c, 1);
      break;
    default:
      digitalWrite(pins.g, 0);
      digitalWrite(pins.f, 0);
      digitalWrite(pins.a, 0);
      digitalWrite(pins.b, 0);
      digitalWrite(pins.e, 0);
      digitalWrite(pins.d, 0);
      digitalWrite(pins.c, 0);
  }
}

// Spaghetti
void displayIntoSseg(double celsius) {
  int temp = celsius;
  // LO into sseg
  if (temp < 0) {
    digitalWrite(tenPin.g, 0);
    digitalWrite(tenPin.f, 1);
    digitalWrite(tenPin.a, 0);
    digitalWrite(tenPin.b, 0);
    digitalWrite(tenPin.e, 1);
    digitalWrite(tenPin.d, 1);
    digitalWrite(tenPin.c, 1);
    setSsegFromNumber(onePin, 0);
  }
  // HI into sseg
  else if (temp > 99) {
    digitalWrite(tenPin.g, 1);
    digitalWrite(tenPin.f, 1);
    digitalWrite(tenPin.a, 0);
    digitalWrite(tenPin.b, 1);
    digitalWrite(tenPin.e, 1);
    digitalWrite(tenPin.d, 0);
    digitalWrite(tenPin.c, 1);
    setSsegFromNumber(onePin, 1);
    // I
  }
  // Set pin normally
  else {
    setSsegFromNumber(onePin, temp % 10);
    setSsegFromNumber(tenPin, temp / 10);
  }
}

void toneBuzzer(double celsius) {
  if (timelapsed > 5) {
    for (int i = 0; i < 3; i++) {
      tone(buzzerPin, NOTE_B7);
      delay(200);
      noTone(buzzerPin);
      delay(200);
    }
    delay(300);
  }
  else {
    noTone(buzzerPin);
    delay(1500);
  }
}

void setSsegPins(sseg pins) {
  pinMode(pins.a, 1);
  pinMode(pins.b, 1);
  pinMode(pins.c, 1);
  pinMode(pins.d, 1);
  pinMode(pins.e, 1);
  pinMode(pins.f, 1);
  pinMode(pins.g, 1);
}

void setup() {
  Serial.begin(9600);
  setSsegPins(tenPin);
  setSsegPins(onePin);
}

void loop() {
  double celsius = getTemperature();
  if (celsius > 50.0)
    timelapsed++;
  else
    timelapsed = 0;
  displayIntoSseg(celsius);
  toneBuzzer(celsius);
}
