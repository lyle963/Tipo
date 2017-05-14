#include "Keyboard.h"

bool newPress;

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
'Z', B101001 //z
};


void setup() {
  Keyboard.begin();
  for(int i=4;i<=9;i++)
    pinMode(i,INPUT_PULLUP);

}

void loop() {
 
  
  char character = readKeys();
  if(newPress){
    Keyboard.print(character);
    newPress=0;
  }
    
}

char readKeys(){
  byte keys = waitForRelease();
  return convert(keys);
}

byte waitForRelease(){
  byte a= B000000;
  byte x= B000000;
  while(1){
    x=0;
    for(int i=0;i<6;i++){
      if(digitalRead(i+4)==LOW){    //if button is GND then follow
        x=1;           
        x<<=i;
        a=a|x;
        delay(50);
      }
    }
    if(a && !newPress)
      newPress=1;
    if(!x && newPress)
      {
        delay(150);
        return a;
      }

  }
}

char convert(byte keys){
  for(int i=0;i<51;i+=2){
    if(keyMap[i]==keys)
      return keyMap[i-1];
  }
}
