#include <Arduino.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>

#define TRIGGER_PIN 2
#define ECHO_PIN 3
#define BUTTON_PIN 4
#define BUZZER_PIN 5
#define SERVO_PIN 6
#define NEOPIXEL_PIN 7
#define NUM_PIXELS 24

Adafruit_NeoPixel neoPixelRing = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
Servo servoMotor;

int buttonPressCount = 0;
int condClose = 0;
const int servoMinAngle = 3;   // Servo minAngle
const int servoMaxAngle = 87;  // Servo maxAngle

Bounce button1 = Bounce();  // Instantiate a Bounce object

void setup() {
  Serial.begin(9600);
  button1.attach(BUTTON_PIN, INPUT_PULLUP);  // Attach the debouncer to a pin with INPUT_PULLUP mode
  button1.interval(25);                      // Use a debounce interval of 25 milliseconds
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  servoMotor.attach(SERVO_PIN);
  neoPixelRing.begin();
  neoPixelRing.setBrightness(32);  // Set dim brightness
  neoPixelRing.show();
}

void loop() {
  long duration, distance;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) * 0.0344;

    if (distance <= 50 || condClose == 1) {
      condClose = 1;
      if (buttonPressCount == 0) {
      tenseMicrowave();
      }
      else if (buttonPressCount > 0 && buttonPressCount < 10) {
      calmerMicrowave();
      }
      else {
      //happyMicrowave();
      }
    } else {
      distressedMicrowave();
    }
}

void updateButtonPressCount() {
  button1.update();
  if (button1.fell()) {
    buttonPressCount++;
    Serial.println(buttonPressCount);
  }
}

void ringBlinkRed() {
  neoPixelRing.clear();
  neoPixelRing.fill(neoPixelRing.Color(255, 0, 0));  // Red color
  neoPixelRing.show();
  delay(100);  // Adjust delay between on and off as needed
  neoPixelRing.clear();
  neoPixelRing.show();
  delay(100);  // Adjust delay between on and off as needed
}

void distressedMicrowave() {
  for (int i = 4000; i > 1000; i -= 100) {
    tone(BUZZER_PIN, i);
    delay(50);

    if (i % 1000 == 0) {
      int currentAngle = servoMotor.read();
      if (currentAngle == servoMinAngle) {
        servoMotor.write(servoMaxAngle);
      } else {
        servoMotor.write(servoMinAngle);
      }
    }
    updateButtonPressCount(); // Add this line to update button press count
  }
  noTone(BUZZER_PIN);
  ringBlinkRed();
  int currentAngle = servoMotor.read();
  if (currentAngle == servoMinAngle) {
    servoMotor.write(servoMaxAngle);
  } else {
    servoMotor.write(servoMinAngle);
  }
}


void tenseMicrowave() {
  const int servoIntermediateAngle = 10;
  int colorIndex = 0;
  const uint32_t colors[] = {
    neoPixelRing.Color(8, 0, 0),  // Dim red
    neoPixelRing.Color(0, 8, 0),  // Dim green
    neoPixelRing.Color(0, 0, 8),  // Dim blue
    neoPixelRing.Color(8, 8, 0),  // Dim yellow
    neoPixelRing.Color(0, 8, 8),  // Dim cyan
    neoPixelRing.Color(8, 0, 8),  // Dim magenta
  };
  const int numColors = sizeof(colors) / sizeof(colors[0]);

  unsigned long previousMillis = millis();
  int pixelDelay = 5;
  int colorDelay = 250;

  for (int i = 0; i < numColors * 2; i++) {
    uint32_t currentColor = colors[colorIndex];

    for (int j = 0; j < NUM_PIXELS; j++) {
      neoPixelRing.setPixelColor(j, currentColor);
      neoPixelRing.show();

      while (millis() - previousMillis < pixelDelay) {
        updateButtonPressCount();
      }
      previousMillis = millis();
    }

    if (colorIndex == 0) {  // Bright red color
      neoPixelRing.clear();
      neoPixelRing.fill(neoPixelRing.Color(255, 0, 0));
      neoPixelRing.show();
      tone(BUZZER_PIN, 1000, 250);  // Play the tone for 250 milliseconds
      servoMotor.write(servoMaxAngle);
      delay(1000);
      servoMotor.write(servoMinAngle);
    } else {
      noTone(BUZZER_PIN);
      servoMotor.write(servoIntermediateAngle);
    }

    colorIndex = (colorIndex + 1) % numColors;

    while (millis() - previousMillis < colorDelay) {
      updateButtonPressCount();
    }
    previousMillis = millis();
  }
}

void calmerMicrowave() {
  noTone(BUZZER_PIN);  // Buzzer is silent

  // Servo motor behavior
  int initialAngle = 27;
  int angleIncrease = 6;

  // NeoPixel Ring behavior
  uint32_t pinkColor = neoPixelRing.Color(255, 20, 147);
  unsigned long breathingStartTime = millis();
  unsigned long previousMillis = millis();

  while (millis() - breathingStartTime < 19000) {  // 4s + 7s + 8s = 19s
    unsigned long currentMillis = millis();
    unsigned long elapsedTime = currentMillis - breathingStartTime;
    uint8_t brightness;

    if (elapsedTime < 4000) {
      brightness = map(elapsedTime, 0, 4000, 0, 255);
    } else if (elapsedTime < 11000) {
      brightness = 255;
    } else {
      brightness = map(elapsedTime, 11000, 19000, 255, 0);
    }

    neoPixelRing.setBrightness(brightness);
    neoPixelRing.fill(pinkColor);
    neoPixelRing.show();

    // Update servo angle and button press count every 100ms
    if (currentMillis - previousMillis >= 100) {
      previousMillis = currentMillis;
      updateButtonPressCount();

      // Update servo angle based on button presses
      int targetAngle = initialAngle + buttonPressCount * angleIncrease;
      targetAngle = constrain(targetAngle, servoMinAngle, servoMaxAngle);
      servoMotor.write(targetAngle);
    }
  }
}
