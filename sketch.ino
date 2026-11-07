
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define BUTTON_FACE 19
#define BUTTON_SLEEP 18
#define BUZZER 4

Adafruit_SSD1306 display(128, 64, &Wire, -1);

int happiness = 80;
int energy = 100;
int mood = 0;

bool sleeping = false;
bool lastFace = HIGH;
bool lastSleep = HIGH;

unsigned long lastUpdate = 0;
unsigned long lastDraw = 0;

// MEOW SOUND!
void meow() {
  tone(BUZZER, 650);
  delay(100);

  tone(BUZZER, 950);
  delay(120);

  tone(BUZZER, 750);
  delay(120);

  tone(BUZZER, 500);
  delay(150);

  noTone(BUZZER);
}

// Draw cute cat
void drawCat() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Happiness and energy
  display.setTextSize(1);
  display.setCursor(2, 0);
  display.print("H:");
  display.print(happiness);

  display.setCursor(85, 0);
  display.print("E:");
  display.print(energy);

  // Cat head outline
  display.drawLine(18, 39, 18, 12, WHITE);
  display.drawLine(18, 12, 24, 7, WHITE);
  display.drawLine(24, 7, 45, 21, WHITE);
  display.drawLine(45, 21, 83, 21, WHITE);
  display.drawLine(83, 21, 104, 7, WHITE);
  display.drawLine(104, 7, 110, 12, WHITE);
  display.drawLine(110, 12, 110, 39, WHITE);
  display.drawLine(110, 39, 117, 46, WHITE);
  display.drawLine(117, 46, 105, 58, WHITE);
  display.drawLine(105, 58, 23, 58, WHITE);
  display.drawLine(23, 58, 11, 46, WHITE);
  display.drawLine(11, 46, 18, 39, WHITE);

  // Inner ears
  display.drawLine(26, 15, 28, 32, WHITE);
  display.drawLine(28, 32, 40, 23, WHITE);

  display.drawLine(102, 15, 100, 32, WHITE);
  display.drawLine(100, 32, 88, 23, WHITE);

  if (sleeping) {
    // Sleeping eyes
    display.drawLine(35, 36, 51, 36, WHITE);
    display.drawLine(77, 36, 93, 36, WHITE);

    display.setCursor(91, 11);
    display.print("Zz");

    display.drawCircle(64, 48, 3, WHITE);
  }
  else if (mood == 0) {
    // Sparkly eyes
    display.fillCircle(43, 36, 7, WHITE);
    display.fillCircle(85, 36, 7, WHITE);

    display.fillCircle(45, 33, 2, BLACK);
    display.fillCircle(87, 33, 2, BLACK);

    // Cute smile
    display.drawLine(64, 47, 60, 51, WHITE);
    display.drawLine(60, 51, 56, 48, WHITE);
    display.drawLine(64, 47, 68, 51, WHITE);
    display.drawLine(68, 51, 72, 48, WHITE);
  }
  else {
    // Surprised eyes
    display.drawCircle(43, 36, 7, WHITE);
    display.drawCircle(85, 36, 7, WHITE);

    display.fillCircle(43, 36, 3, WHITE);
    display.fillCircle(85, 36, 3, WHITE);

    display.drawCircle(64, 49, 4, WHITE);
  }

  // Nose
  display.fillTriangle(61, 44, 67, 44, 64, 48, WHITE);

  // Whiskers
  display.drawLine(13, 43, 33, 46, WHITE);
  display.drawLine(12, 49, 33, 49, WHITE);
  display.drawLine(95, 46, 115, 43, WHITE);
  display.drawLine(95, 49, 116, 49, WHITE);

  display.display();
}

void setup() {
  Wire.begin(21, 22);

  pinMode(BUTTON_FACE, INPUT_PULLUP);
  pinMode(BUTTON_SLEEP, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  drawCat();
}

void loop() {
  unsigned long now = millis();

  bool faceButton = digitalRead(BUTTON_FACE);
  bool sleepButton = digitalRead(BUTTON_SLEEP);

  // Play with cat
  if (lastFace == HIGH && faceButton == LOW) {
    if (!sleeping) {
      mood = (mood + 1) % 2;

      happiness += 10;
      if (happiness > 100) happiness = 100;

      meow();
      drawCat();
    }
    delay(35);
  }

  // Sleep and wake
  if (lastSleep == HIGH && sleepButton == LOW) {
    sleeping = !sleeping;

    meow();
    drawCat();

    delay(35);
  }

  lastFace = faceButton;
  lastSleep = sleepButton;

  // Update energy every 5 seconds
  if (now - lastUpdate >= 5000) {
    lastUpdate = now;

    if (sleeping) {
      energy += 5;
      if (energy > 100) energy = 100;
    } else {
      energy -= 2;
      if (energy < 0) energy = 0;
    }

    if (!sleeping && happiness > 0) {
      happiness--;
    }
  }

  // Automatic sleep
  if (energy == 0 && !sleeping) {
    sleeping = true;
  }

  // Refresh display
  if (now - lastDraw >= 250) {
    lastDraw = now;
    drawCat();
  }
}
