#include "Keyboard.h"
#define mode usb
#define debug 1
enum modes {serial,usb};
bool newPress;
//               s6,s5,s4,s3,s2,s1,ENT,DEL
byte pinMap[] = {4 ,12, 6, 7, 8,9, 11, 10 };
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
  if(debug)
    Serial.begin(115200);
  if(mode==usb)
    Keyboard.begin();
  for(int i=0;i<=7;i++){
    pinMode(pinMap[i],INPUT);
    digitalWrite(pinMap[i],HIGH);
  }
}

void loop() {
  char character = readKeys();
  if(newPress && character!=0){
    if(mode==usb)
      Keyboard.print(character);
    else if(mode==serial)
      Serial.print(character);
    newPress=0;
  }
  if(newPress) newPress=0;
}

char readKeys(){
  byte keys = waitForRelease();
  char c = convert(keys);
  if(debug)Serial.println(c);
  if(c!='#')
    return c;
  //handle numbers
  newPress=0;
  keys = waitForRelease();
  c = convert(keys) - 16;//ascii conversion from A to 1 
  if(c==58) c = '0'; //0 at end not begining
  if(debug)Serial.println(c);
  return c;
}

byte waitForRelease(){
  byte a= B000000;
  byte x= B000000;
  while(1){
    x=0;
    for(int i=0;i<6;i++){
      if(digitalRead(pinMap[i])==LOW){    //if button is GND then follow
        x=1;           
        x<<=i;
        a=a|x;
        delay(50);
        if(debug)Serial.println(a,BIN);
      }
    }
    if(a && !newPress)
      newPress=1;
    if(!x && newPress)
      {
        delay(150);
        return a;
      }
    if((digitalRead(10)==LOW || digitalRead(11)==LOW) && !a)
      FnKeys();
  }
}

char convert(byte keys){
  for(int i=0;i<=54;i+=2){
    if(keyMap[i]==keys)
      return keyMap[i-1];
  }
  return 0;
}

void FnKeys(){
  if(digitalRead(pinMap[6])==LOW){//ENTER
    delay(50);
    if(digitalRead(pinMap[7])==LOW && mode == usb)
      Keyboard.press(SPACE);
    else if(mode==usb)
      Keyboard.press(KEY_RETURN);
    if(mode==serial)
      Serial.println("RETURN");
    while(digitalRead(pinMap[6])==LOW);
    delay(150);
    Keyboard.releaseAll();
  }
  
  if(digitalRead(pinMap[7])==LOW){//DELETE
    if(mode==usb)
      Keyboard.press(KEY_BACKSPACE);
    if(mode==serial)
      Serial.println('BACKSPACE');
    while(digitalRead(pinMap[7])==LOW);
    delay(150);
    Keyboard.releaseAll();
  } 
}

