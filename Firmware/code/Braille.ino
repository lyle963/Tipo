#include "Keyboard.h"
#define mode usb
#define debug 1
#define bouncetime 50
enum modes {serial, usb};
bool newPress;
//               s6,s5,s4,s3,s2,s1,ENT,DEL
byte pinMap[] = {4 , 12, 6, 7, 8, 9, 11, 10 };
byte keyMap[] = {

  B000000,
  'A', B100000,//a
  'B', B110000,
  'C', B100100,
  'D', B100110,
  'E', B100010,
  'F', B110100,
  'G', B110110,
  'H', B110010,
  'I', B010100,
  'J', B010110,
  'K', B101000,
  'L', B111000,
  'M', B101100,
  'N', B101110,
  'O', B101010,
  'P', B111100,
  'Q', B111110,
  'R', B111010,
  'S', B011100,
  'T', B011110,
  'U', B101001,
  'V', B111001,
  'W', B010111,
  'X', B101101,
  'Y', B101111,
  'Z', B101011, //z
  '#', B001111,
};

void setup() {
  if (debug)
    Serial.begin(115200);
  if (mode == usb)
    Keyboard.begin();
  for (int i = 0; i <= 7; i++) {
    pinMode(pinMap[i], INPUT);
    digitalWrite(pinMap[i], HIGH);
  }
}

void loop() {
  char character = readKeys();

  if (newPress && character != 0) {
    if (mode == usb)
      Keyboard.print(character);
    else if (mode == serial)
      Serial.print(character);
    newPress = 0;
  }
  
  if (newPress) newPress = 0;
}
