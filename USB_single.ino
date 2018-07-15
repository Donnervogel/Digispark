#include "DigiKeyboard.h"
#include <EEPROM.h>


void setup() {
  pinMode(1,OUTPUT);
  digitalWrite(1,LOW);
}


void loop() {
  int Key, Key2 = 0;
  digitalWrite(1,HIGH);
  for (int i=0; i<=511; i++){
    Key = EEPROM.read(i);
    
    if (Key == 172){ //"¬" + following = delay
      Key = EEPROM.read(i+1);
      DigiKeyboard.delay(10^Key);
      i = i + 1;
    }
    else if (Key == 174){ //"®" = 2 Keys Combination
      Key = EEPROM.read(i+1);
      Key2 = EEPROM.read(i+2);
      DigiKeyboard.sendKeyStroke(Key, Key2);
      i = i + 2;
    }
    else{
      DigiKeyboard.sendKeyStroke(Key);
    }    
    //optional: DigiKeyboard.delay(500);
  }
  digitalWrite(1,LOW);
    
}
