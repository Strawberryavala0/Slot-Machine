#include <ServoTimer2.h>

ServoTimer2 ServoMotor;
ServoTimer2 ServoMotor2;
ServoTimer2 ServoMotor3;

int list[4] = {45,135,225,315};
int list2[4] = {315,225,135,45};

int ServoPosition = 0;
int ServoPosition2 = 0;
int ServoPosition3 = 0;

int potPin = A0;

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//neopixels are connected to digital 3, which is pin 2 on the ATtiny IC
#define PIN 5
#define NUM_LEDS 72 //number of leds

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

const int numReadings = 20;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

void setup() {
  // put your setup code here, to run once:
  // put your setup code here, to run once:
    ServoMotor.detach();
    ServoMotor2.detach();
    ServoMotor3.detach();

    strip.begin();
    strip.show();  // Initialize all pixels to 'off'
    
    Serial.begin(9600);

      // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
      // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(potPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  //theaterChaseRainbow(50);
  Serial.println(average);
  if (490 < average){
    if (average < 492){
    Serial.print("REACHED ");
    Serial.println(average);
    theaterChase(strip.Color(127, 127, 127), 50); // White
    ServoMotor.attach(2);
    ServoMotor2.attach(3);
    ServoMotor3.attach(4);
  
    ServoMotor.write(0); // resets to initial position
    ServoMotor2.write(0); // resets to initial position
    ServoMotor3.write(0); // resets to initial position
    delay(5000);
    
    ServoPosition = random(0,4);
    ServoPosition2 = random(0,4);
    ServoPosition3 = random(0,4);
    ServoMotor.write(list[ServoPosition]);
    ServoMotor2.write(list[ServoPosition2]);
    ServoMotor3.write(list2[ServoPosition3]);
    delay(10000);
  
    Serial.print("Servo1: ");
    Serial.println(ServoPosition);
    Serial.print("Servo2: ");
    Serial.println(ServoPosition2);
    Serial.print("Servo3: ");
    Serial.println(ServoPosition3);
  
    Serial.print("Servo1 Angle: ");
    Serial.println(list[ServoPosition]);
    Serial.print("Servo2 Angle: ");
    Serial.println(list[ServoPosition2]);
    Serial.print("Servo3 Angle: ");
    Serial.println(list2[ServoPosition3]);

  if ( list[ServoPosition] == list[ServoPosition2] ){
    if ( list[ServoPosition2] == list2[ServoPosition3] ){
          for(uint16_t i=strip.numPixels(); i<0; i--) {
        strip.setPixelColor(i, 0, 255, 0); //set to purple, takes RGB vals 0-255
        strip.show();
        delay(500);
    }
    Serial.println("Win");
    }
  } else {
              for(uint16_t i=strip.numPixels(); i<0; i--) {
        strip.setPixelColor(i, 255, 0, 0); //set to purple, takes RGB vals 0-255
        strip.show();
        delay(500);
  }
    Serial.println("Lose");


}
  
    ServoMotor.detach();
    ServoMotor2.detach();
    ServoMotor3.detach();
    delay(5000);
  }               
  }
}
//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
