#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define TRIG_PIN 9
#define ECHO_PIN 10

#define MAX_DISTANCE 400
#define MIN_DISTANCE 2

#define BLUE_PIN 3
#define GREEN_PIN 5
#define RED_PIN 6

#define CLOSE_DISTANCE 2
#define FAR_DISTANCE 30

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  if( !display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println("OLED not Found! Check Wiring.");
    while(true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(20, 25);
  display.println("Distance Meter");
  display.display();
  delay(2000);

  Serial.println("Distance meter Starting...");
}

void setLED(bool r, bool g, bool b){
  digitalWrite(RED_PIN, r);
  digitalWrite(GREEN_PIN, g);
  digitalWrite(BLUE_PIN, b);
}

float readDistance(){
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  float distance = (duration * 0.0343)/2.0;
  return distance;
}
void displayDistance(float distance){
  display.clearDisplay();
  if(distance < MIN_DISTANCE || distance > MAX_DISTANCE){
    display.setTextSize(1);
    display.setCursor(30, 20);
    display.println("Out of range");
    display.setCursor(25, 38);
    display.println("( 2cm - 400cm )");
  }else{
    display.setTextSize(1);
    display.setCursor(40, 4);
    display.println("Distance");

    display.setTextSize(3);
    display.setCursor(18, 22);
    display.println(distance, 1);

    display.setTextSize(2);
    display.setCursor(92, 42);
    display.println("cm");

    display.drawLine(0, 14, 128, 14, SSD1306_WHITE);
  }
  display.display();
}

void updateLED(float distance){
  if(distance < CLOSE_DISTANCE){
    setLED(true, false, false);
    Serial.println("LED: RED");
  } else if(distance > FAR_DISTANCE){
    setLED(false, false, true);
    Serial.println("LED: BLUE");
  } else{
    setLED(false, true, false);
    Serial.println("LED: GREEN");
  }
}

void loop() {
  float distance = readDistance();
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  displayDistance(distance);
  updateLED(distance);
  
  delay(300);
}
 