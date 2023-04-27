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

Adafruit_NeoPixel neoPixelRing = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
Servo servoMotor;

int buttonPressCount = 0;
int condClose = 0;
const int minAngle = 3; // Servo minAngle
const int maxAngle = 87; // Servo maxAngle

Bounce button1 = Bounce(); // Instantiate a Bounce object

void setup() {
  Serial.begin(9600);
  button1.attach(BUTTON_PIN,INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
  button1.interval(25); // Use a debounce interval of 25 milliseconds
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  servoMotor.attach(SERVO_PIN);
  neoPixelRing.begin();
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





  // BLINKING RED NEOPIXEL RING: 
  // Turn on all NeoPixels to red
  neoPixelRing.fill(neoPixelRing.Color(255, 0, 0)); // Red color
  neoPixelRing.show();

  // Delay for the desired duration (in milliseconds)
  delay(500); // 500ms (half a second)

  // Turn off all NeoPixels
  neoPixelRing.clear();
  neoPixelRing.show();

  // Delay before starting the next blink
  delay(500); // 500ms (half a second)






  // Move servo to minAngle
  servoMotor.write(minAngle);
  delay(1000);

  // Move servo to maxAngle
   servoMotor.write(maxAngle);
  delay(1000);



  // Play descending tones quietly
  for (int frequency = 1000; frequency >= 200; frequency -= 100) {
    analogWrite(BUZZER_PIN, 1);  // Set buzzer volume to a low value (adjust as needed)
    tone(BUZZER_PIN, frequency, 250);  // Play the tone for 250 milliseconds
    delay(300);  // Delay between each tone
    noTone(BUZZER_PIN);  // Stop the tone
    delay(20); // Delay between each tone to reduce overlap
  }
  
  delay(1000);  // Delay before starting the sequence again



}