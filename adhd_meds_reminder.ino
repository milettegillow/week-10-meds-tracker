const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int buttonPin = 2;

int state = 0;
bool lastButtonState = HIGH;
unsigned long takenTime = 0;
unsigned long buttonPressedAt = 0;
const unsigned long resetAfter = 12UL * 60 * 60 * 1000;
const unsigned long longPressDuration = 2000; // hold 2 seconds to reset
bool longPressHandled = false;

void setColour(int r, int g, int b) {
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}

void showState() {
  if (state == 0) setColour(255, 40, 0);  // Warmer orange
  if (state == 1) setColour(0, 255, 0);   // Green
  if (state == 2) setColour(255, 0, 0);   // Red
}

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  showState();
}

void loop() {
  bool buttonState = digitalRead(buttonPin);

  // Button just pressed
  if (buttonState == LOW && lastButtonState == HIGH) {
    buttonPressedAt = millis();
    longPressHandled = false;
  }

  // Button held — check for long press
  if (buttonState == LOW && !longPressHandled) {
    if (millis() - buttonPressedAt >= longPressDuration) {
      state = 0;
      showState();
      longPressHandled = true;
    }
  }

  // Button released — short press
  if (buttonState == HIGH && lastButtonState == LOW && !longPressHandled) {
    if (state == 0) {
      state = 1;
      takenTime = millis();
    } else if (state == 1) {
      state = 2;
    }
    showState();
    delay(50);
  }

  lastButtonState = buttonState;

  // Auto reset after 12 hours from going green
  if (state != 0 && millis() - takenTime >= resetAfter) {
    state = 0;
    showState();
  }

  delay(20);
}