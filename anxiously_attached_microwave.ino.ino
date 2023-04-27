#include <Arduino.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

// Include the Bounce2 library found here :
// https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>

// Include FastLED library by Daniel Garcia
#include <FastLED.h>

#define TRIGGER_PIN 2
#define ECHO_PIN 3
#define BUTTON_PIN 4
#define BUZZER_PIN 5
#define SERVO_PIN 6
#define NEOPIXEL_PIN 7
#define NUM_PIXELS 24

Adafruit_NeoPixel ring(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
Servo myservo;

int buttonPressCount = 0;
int condClose = 0;

Bounce button1 = Bounce(); // Instantiate a Bounce object

void setup() {
  Serial.begin(9600);
  button1.attach(BUTTON_PIN,INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
  button1.interval(25); // Use a debounce interval of 25 milliseconds
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  myservo.attach(SERVO_PIN);
  ring.begin();
  ring.setBrightness(50);
  ring.show();
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

  // Serial.print("distance: ");
  // Serial.print(distance);
  // Serial.println(" cm");

   button1.update(); // Update the Bounce instance
   
   if (button1.fell()) {  // Call code if button transitions from HIGH to LOW
     buttonPressCount++;
    //  Serial.println(buttonPressCount); // Tests button presses
   }
}