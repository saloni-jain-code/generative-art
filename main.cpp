#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

const char* words[] = {
  "look at the stars", // 0:33
  "look how they shine for you", // 0:36
  "and every thing you do", // 0:41
  "yeah they were all", //0:47
  "yellow",  //0:48
  "i came along", // 0:50
  "i wrote a song for you", // 0:52
  "and all the things you do", //0:57
  "and it was called", //1:04
  "yellow", //1:05
  "so, then i took my turn", //1:09
  "oh, what a thing to have done", //1:15
  "and it was all", //1:20
  "yellow", //1:21
  "your skin, oh yeah, your skin, and bones", //1:28
  "turn into something beautiful", // 1:33
  "and you know, you know i love you so", //1:39
  "you know i love you so", //1:47
  "i swam across", //2:12
  "i jumped across for you", //2:15
  "oh, what a thing to do", //2:20
  "'cause you were all", //2:27
  "yellow", //2:28
  "i drew a line", //2:29
  "i drew a line for you", //2:31
  "oh, what a thing to do", //2:37
  "and it was all", //2:43
  "yellow",//2:44
  "and your skin, oh yeah, your skin, and bones", // 2:51
  "turn into something beautiful", //2:56
  "and you know, for you, i'd bleed myself dry", //3:02
  "for you i'd bleed myself dry", //3:10
  "it's true", // 3:34
  "look how they shine for you", //3:38
  "look how they shine for you", // 3:43
  "look how they shine for-", //3:48
  "look how they shine for you", //3:54
  "look how they shine for you", //4:01
  "look how they shine", //4:05
  "look at the stars", //4:09
  "look how they shine for you", //4:11
  "and all the things that you do" //4:16
};

unsigned long lyricTimes[] = {
  0,        // 0:33
  3000,     // 0:36
  8000,     // 0:41
  14000,    // 0:47
  15000,    // 0:48
  17000,    // 0:50
  19000,    // 0:52
  24000,    // 0:57
  31000,    // 1:04
  32000,    // 1:05
  36000,    // 1:09
  42000,    // 1:15
  47000,    // 1:20
  48000,    // 1:21
  55000,    // 1:28
  60000,    // 1:33
  66000,    // 1:39
  74000,    // 1:47
  99000,    // 2:12
  102000,   // 2:15
  107000,   // 2:20
  114000,   // 2:27
  115000,   // 2:28
  116000,   // 2:29
  118000,   // 2:31
  124000,   // 2:37
  130000,   // 2:43
  131000,   // 2:44
  138000,   // 2:51
  143000,   // 2:56
  149000,   // 3:02
  157000,   // 3:10
  181000,   // 3:34
  185000,   // 3:38
  190000,   // 3:43
  195000,   // 3:48
  201000,   // 3:54
  208000,   // 4:01
  212000,   // 4:05
  216000,   // 4:09
  218000,   // 4:11
  223000    // 4:16
};

const int WORD_COUNT = sizeof(words) / sizeof(words[0]);
unsigned long songStartTime = 0;
unsigned long lyricStartTime = 0;

int currentWord = 0;
unsigned long lastSwitch = 0;
const unsigned long interval = 2000;

uint16_t deepPink = tft.color565(255, 20, 147);
bool yellowMode = false;
uint16_t bgColor = TFT_NAVY;

const int lineDrawDuration = 2000; // 2 seconds for the line to draw

struct Star {
  float x, y;
  float vx, vy;
};

const int STAR_COUNT = 60;
Star stars[STAR_COUNT];

uint16_t randomBrightColor() {
    uint8_t r = 16 + random(16);  // 16–31 → bright red
    uint8_t g = 32 + random(32);  // 32–63 → bright green
    uint8_t b = 16 + random(16);  // 16–31 → bright blue
    return (r << 11) | (g << 5) | b;
}

uint16_t randomYellow() {
  // RGB565: R=5 bits, G=6 bits, B=5 bits
  // Yellow = red + green, small blue
  uint8_t r = 25 + random(31);    // 25–55 (out of 31)
  uint8_t g = 50 + random(63);    // 50–63 (out of 63)
  uint8_t b = random(8);           // 0–7 (small blue)
  return (r << 11) | (g << 5) | b;
}

uint16_t randomSoftColor() {
  uint8_t r = 20 + random(12);   // 20–31 → medium-high red
  uint8_t g = 25 + random(30);   // 25–54 → medium green
  uint8_t b = random(8);         // 0–7 → low blue
  return (r << 11) | (g << 5) | b;
}

uint16_t goldenYellow() {
  uint8_t r = 28 + random(4);  // 28–31
  uint8_t g = 40 + random(11); // 40–50
  uint8_t b = random(4);       // 0–3
  return (r << 11) | (g << 5) | b;
}

void drawWord(const char* str, uint16_t bgColor) {

  uint16_t defaultColor = TFT_WHITE;
  tft.setTextWrap(false);

  int maxWidth = tft.width() - 20;
  int size = 3;

  // ---- Find largest size that fits ----
  while (size > 0) {
    tft.setTextSize(size);

    String s = String(str);
    String word = "";
    String line = "";
    int widest = 0;

    for (int i = 0; i <= s.length(); i++) {

      if (s[i] == ' ' || s[i] == '\0') {

        String testLine = (line.length() == 0) ? word : line + " " + word;

        if (tft.textWidth(testLine) <= maxWidth) {
          line = testLine;
        } else {
          int w = tft.textWidth(line);
          if (w > widest) widest = w;
          line = word;
        }

        word = "";
      } else {
        word += s[i];
      }
    }

    int w = tft.textWidth(line);
    if (w > widest) widest = w;

    if (widest <= maxWidth) break;
    size--;
  }

  tft.setTextSize(size);

  // ---- Build wrapped lines ----
  String s = String(str);
  String word = "";
  String line = "";
  String lines[10];
  int lineCount = 0;

  for (int i = 0; i <= s.length(); i++) {

    if (s[i] == ' ' || s[i] == '\0') {

      String testLine = (line.length() == 0) ? word : line + " " + word;

      if (tft.textWidth(testLine) <= maxWidth) {
        line = testLine;
      } else {
        lines[lineCount++] = line;
        line = word;
      }

      word = "";
    } else {
      word += s[i];
    }
  }

  lines[lineCount++] = line;

  // ---- Vertical centering ----
  int lineHeight = 8 * size;
  int totalHeight = lineCount * lineHeight;
  int y = (tft.height() - totalHeight) / 2;

  uint16_t color565 = bgColor;
  uint8_t r = (color565 >> 11) & 0x1F;  // 5 bits
  uint8_t g = (color565 >> 5) & 0x3F;   // 6 bits
  uint8_t b = color565 & 0x1F;          // 5 bits

  r = (r * 255) / 31;
  g = (g * 255) / 63;
  b = (b * 255) / 31;

  uint8_t rComp = 255 - r;
  uint8_t gComp = 255 - g;
  uint8_t bComp = 255 - b;

  float factor = 0.8; // slightly darker complement
  rComp = rComp * factor;
  gComp = gComp * factor;
  bComp = bComp * factor;

  uint16_t complement = tft.color565(rComp, gComp, bComp);

  // ---- Draw lines centered, with highlights ----
  for (int i = 0; i < lineCount; i++) {

    int textW = tft.textWidth(lines[i]);
    int x = (tft.width() - textW) / 2;

    tft.setCursor(x, y);

    // Split line into words for highlighting
    String tempLine = lines[i];
    String currentWord = "";
    for (int j = 0; j <= tempLine.length(); j++) {
      char c = (j < tempLine.length()) ? tempLine[j] : ' ';

      if (c == ' ' || j == tempLine.length()) {
        // Check for highlight
        if (currentWord.equalsIgnoreCase("love") || currentWord.equalsIgnoreCase("bleed")) {
          tft.setTextColor(TFT_RED, bgColor);
        } else if (currentWord.indexOf("you") >= 0) {
          tft.setTextColor(complement, bgColor);
        } else {
          tft.setTextColor(defaultColor, bgColor);
        }

        if (currentWord.equalsIgnoreCase("you")) {
          tft.setCursor(x + 1, y);
          tft.print("you");
          tft.setCursor(x, y);
          tft.print("you");
        } else if (currentWord.equalsIgnoreCase("shine")) {
          for (int k = 0; k < currentWord.length(); k++) {
            tft.setTextColor(randomBrightColor(), bgColor);
            tft.print(currentWord[k]);
          }
        } else {
          tft.print(currentWord);
        }
        tft.print(' ');  // keep spaces
        currentWord = "";
      } else {
        currentWord += c;
      }
    }

    y += lineHeight;
  }
}

void setup() {
  tft.init();
  tft.setRotation(2);   // portrait
  tft.setTextSize(2);
  tft.setTextWrap(false);

  randomSeed(esp_random());
  songStartTime = millis();

  // Initialize stars
  for (int i = 0; i < STAR_COUNT; i++) {
    stars[i].x = random(tft.width());
    stars[i].y = random(tft.height());
    stars[i].vx = random(-10, 11) / 20.0;  // slow drift
    stars[i].vy = random(-10, 11) / 20.0;
  }

  tft.fillScreen(bgColor);
  drawWord(words[currentWord], bgColor);
}


void loop() {

  unsigned long now = millis();

  // ----- Switch lyric -----
  unsigned long songTime = millis() - songStartTime;

  if (currentWord < WORD_COUNT - 1 &&
      songTime >= lyricTimes[currentWord + 1]) {

    currentWord++;

    yellowMode = strstr(words[currentWord], "yellow") != nullptr;
    if (yellowMode) {
      bgColor = goldenYellow();
    } else {
      bgColor = randomSoftColor();
    }
    lyricStartTime = now;
  } else if (currentWord == WORD_COUNT - 1 &&
           songTime >= lyricTimes[WORD_COUNT - 1] + 5000) { // optional extra pause
    // Reset to start
    currentWord = 0;
    songStartTime = millis();
    yellowMode = strstr(words[0], "yellow") != nullptr;
    if (yellowMode) {
      bgColor = goldenYellow();
    } else {
      bgColor = randomSoftColor();
    }
    lyricStartTime = songStartTime;
  }

  // ----- Clear whole screen -----
  tft.fillScreen(bgColor);

  // ----- Update & draw stars -----
  for (int i = 0; i < STAR_COUNT; i++) {

    stars[i].x += stars[i].vx + sin(millis()/1000.0 + i) * 0.2;
    stars[i].y += stars[i].vy + cos(millis()/1200.0 + i) * 0.2;

    if (stars[i].x < 0) stars[i].x = tft.width();
    if (stars[i].x > tft.width()) stars[i].x = 0;
    if (stars[i].y < 0) stars[i].y = tft.height();
    if (stars[i].y > tft.height()) stars[i].y = 0;

    tft.fillCircle((int)stars[i].x, (int)stars[i].y, 1, TFT_WHITE);
  }

  // ----- Draw text on top -----
  drawWord(words[currentWord], bgColor);

  if (strcmp(words[currentWord], "i drew a line") == 0 ||
      strcmp(words[currentWord], "i drew a line for you") == 0) {

    // Compute fraction of the line to draw
    float progress = float(now - lyricStartTime) / lineDrawDuration;
    if (progress > 1.0) progress = 1.0;

    int lineLength = tft.width() * progress;
    int yLine = tft.height() * 5 / 6;

    tft.drawLine(0, yLine, lineLength, yLine, TFT_WHITE);

  } 
  
  delay(30);
}

