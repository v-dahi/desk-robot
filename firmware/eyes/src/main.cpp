#include <Arduino.h>
#include <TFT_eSPI.h>

#define TFT_CS_LEFT  5
#define TFT_CS_RIGHT 15

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite eye = TFT_eSprite(&tft);

const int CENTER = 120;
const int IRIS_RADIUS = 75;
const int BIG_HL_RADIUS = 14;
const int SMALL_HL_RADIUS = 7;
const int HL_OFFSET = (int)(IRIS_RADIUS * 0.4);
const int MAX_OFFSET_X = 45;
const int MAX_OFFSET_Y = 30;

int iris_x = 0;
int iris_y = 0;

// Helper: activate one eye for drawing
void selectEye(int csPin) {
  digitalWrite(TFT_CS_LEFT,  csPin == TFT_CS_LEFT  ? LOW : HIGH);
  digitalWrite(TFT_CS_RIGHT, csPin == TFT_CS_RIGHT ? LOW : HIGH);
}

// Activate BOTH eyes (they receive the same data simultaneously)
void selectBoth() {
  digitalWrite(TFT_CS_LEFT,  LOW);
  digitalWrite(TFT_CS_RIGHT, LOW);
}

void deselectAll() {
  digitalWrite(TFT_CS_LEFT,  HIGH);
  digitalWrite(TFT_CS_RIGHT, HIGH);
}

void drawEyeIntoSprite(int offsetX, int offsetY) {
  int cx = CENTER + offsetX;
  int cy = CENTER + offsetY;
  eye.fillSprite(TFT_WHITE);
  eye.fillCircle(cx, cy, IRIS_RADIUS, TFT_BLACK);
  eye.fillCircle(cx - HL_OFFSET, cy - HL_OFFSET, BIG_HL_RADIUS, TFT_WHITE);
  eye.fillCircle(cx - HL_OFFSET + 20, cy - HL_OFFSET + 18, SMALL_HL_RADIUS, TFT_WHITE);
}

void pushToBothEyes() {
  selectBoth();        // both screens listen
  eye.pushSprite(0, 0);
  deselectAll();
}

void lookAtBoth(int targetX, int targetY) {
  int startX = iris_x;
  int startY = iris_y;
  int steps = 8;

  for (int i = 1; i <= steps; i++) {
    float progress = (float)i / steps;
    float eased = progress * progress * (3 - 2 * progress);
    int newX = startX + (int)((targetX - startX) * eased);
    int newY = startY + (int)((targetY - startY) * eased);

    drawEyeIntoSprite(newX, newY);
    pushToBothEyes();

    iris_x = newX;
    iris_y = newY;
    delay(50);
  }
}

void blinkBoth() {
  eye.fillSprite(TFT_BLACK);
  pushToBothEyes();
  delay(80);
  drawEyeIntoSprite(iris_x, iris_y);
  pushToBothEyes();
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Two-eye Totoro starting...");

  // Manually configure both CS pins as outputs, default HIGH (deselected)
  pinMode(TFT_CS_LEFT,  OUTPUT);
  pinMode(TFT_CS_RIGHT, OUTPUT);
  deselectAll();

  // Initialize each display — needs CS active for that display during init
  selectEye(TFT_CS_LEFT);
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_WHITE);

  selectEye(TFT_CS_RIGHT);
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_WHITE);

  deselectAll();

  eye.setColorDepth(8);
  eye.createSprite(240, 240);

  if (eye.created()) {
    Serial.printf("Sprite OK, free heap=%u, largest=%u\n",
                  ESP.getFreeHeap(), ESP.getMaxAllocHeap());
  } else {
    Serial.println("ERROR: Sprite creation failed");
  }

  drawEyeIntoSprite(0, 0);
  pushToBothEyes();
  Serial.println("Both eyes ready");
}

void loop() {
  lookAtBoth(MAX_OFFSET_X, 0);
  delay(700);
  lookAtBoth(-MAX_OFFSET_X, 0);
  delay(700);
  lookAtBoth(0, MAX_OFFSET_Y);
  delay(500);
  lookAtBoth(0, -MAX_OFFSET_Y);
  delay(500);
  lookAtBoth(0, 0);
  delay(1200);
  blinkBoth();
  delay(1500);
}