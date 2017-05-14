char readKeys() {
  byte keys = waitForRelease();
  char c = convert(keys);
  if (debug)Serial.println(c);
  if (c != '#')
    return c;
  //handle numbers
  newPress = 0;
  keys = waitForRelease();
  c = convert(keys) - 16;//ascii conversion from A to 1, B to 2 etc
  if (c == 58) c = '0'; //0 after 9 not before 1 in braille alphabet
  if (debug)Serial.println(c);
  return c;

}



byte waitForRelease() {
  byte a = B000000;
  byte x = B000000;
  while (1) {
    x = 0;
    for (int i = 0; i < 6; i++) {
      if (digitalRead(pinMap[i]) == LOW) { //if button is GND then follow
        x = 1;
        x <<= i;
        a = a | x;
        delay(bouncetime);
        if (debug)Serial.println(a, BIN);
      }
    }
    if (a && !newPress)
      newPress = 1;
    if (!x && newPress)
    {
      delay(3* bouncetime);
      return a;
    }
    if (!a)
      FnKeys();
  }
}



void FnKeys() {
  byte buttons = 0; // B00(0) - nothing pressed, B01(1) - ent pressed, B10(2) - delete pressed, B11(3) - both pressed
  if (digitalRead(pinMap[6]) == LOW || digitalRead(pinMap[7]) == LOW) {
    delay(bouncetime/2);
    while(digitalRead(pinMap[6]) == LOW || digitalRead(pinMap[7]) == LOW){
      if (digitalRead(pinMap[6]) == LOW)
        buttons |= 1;
      if (digitalRead(pinMap[7]) == LOW)
        buttons |= 2;
    }
    if (mode == usb) {
      switch (buttons) {
        case 1:   Keyboard.press(KEY_RETURN);
          break;
        case 2:   Keyboard.press(KEY_BACKSPACE);
          break;
        case 3:   Keyboard.write(' ');
          break;
      }
      while (digitalRead(pinMap[7]) == LOW || digitalRead(pinMap[6]) == LOW);
      Keyboard.releaseAll();
      delay(bouncetime/2);
    }
    else if(mode == serial){
      Serial.print("FnButtons: ");
      Serial.println(buttons);
    }
  }
}


char convert(byte keys) {
  for (int i = 0; i <= 54; i += 2) {
    if (keyMap[i] == keys)
      return keyMap[i - 1];
  }
  return 0;
}
