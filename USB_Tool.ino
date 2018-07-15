#include <TinyWireM.h>
#include <USI_TWI_Master.h>
#include "DigiKeyboard.h"
#include <EEPROM.h>
//#include <DigiUSB.h>

#define ATM32_ADDR     0x28
#define MyPort     0x01 //0x02 //0x03
int State = 0; //0:read; 1:write; 2:both;
int Port = 0;
int Program = 0;
int Mode = 0;
int Read = 0;

void setup() {
  TinyWireM.begin();
  State = EEPROM.read(0); // |State/Mode|Program|Num.OfPrograms|Start2|Start3|Start4|Start5|Start6|Start7|Start8 (EEPROM 0-9)
  pinMode(0,INPUT);
  pinMode(1,OUTPUT);
  digitalWrite(1,LOW);

  if(digitalRead(0) == 1){
    State = 0;
  }
}


void loop() {

  if(State == 0){
    TinyWireM.beginTransmission(ATM32_ADDR);
    TinyWireM.send(0x01);
    TinyWireM.endTransmission();
    delay(10);
    TinyWireM.requestFrom(ATM32_ADDR,1);
    Read=TinyWireM.receive();
    
    if(Read==MyPort){  //Check Port (=me?)
      digitalWrite(1,HIGH);
      Read = 0;
      TinyWireM.beginTransmission(ATM32_ADDR); //Show ready
      TinyWireM.send(0x00);
      TinyWireM.send(0x01);
      TinyWireM.endTransmission();
      delay(10);
      
      TinyWireM.beginTransmission(ATM32_ADDR); //New Data available?
      TinyWireM.send(0x04);
      TinyWireM.endTransmission();
      delay(10);
      TinyWireM.requestFrom(ATM32_ADDR,1);
      Read=TinyWireM.receive();

      if(Read == 1){        
        TinyWireM.beginTransmission(ATM32_ADDR); //get R/W Mode
        TinyWireM.send(0x02);
        TinyWireM.endTransmission();
        delay(10);
        TinyWireM.requestFrom(ATM32_ADDR,1);
        Mode=TinyWireM.receive();
        delay(10);

        TinyWireM.beginTransmission(ATM32_ADDR); //get Program
        TinyWireM.send(0x03);
        TinyWireM.endTransmission();
        delay(10);
        TinyWireM.requestFrom(ATM32_ADDR,1);
        Program=TinyWireM.receive();
        delay(10);

        TinyWireM.beginTransmission(ATM32_ADDR); //Set "New Data available" = 0
        TinyWireM.send(0x04);
        TinyWireM.send(0x00);
        TinyWireM.endTransmission();

        Read = 0;   
        EEPROM.write(0, Mode);
        EEPROM.write(1, Program);     
      }
    }
    digitalWrite(1,LOW);    
  }
  
  else if (State == 1){
    int Start = 0;
    int Stop = 0;
    int Key, Key2, Key3 = 0;
    Program = EEPROM.read(1);
    EEPROM.write(0,0);
    EEPROM.write(1,0);

    
    if(Program = 1){
      Start = 10;
      Stop = EEPROM.read(3) - 1;
    }
    else{
      Start = EEPROM.read(Program + 1);
      Stop = EEPROM.read(Program + 2) - 1;
    }
    digitalWrite(1,HIGH);
    for (int i=Start; i<=Stop; i++){
      Key = EEPROM.read(i);
      
      if (Key == 172){ //"¬" + following = delay
        Key = EEPROM.read(i+1);
        DigiKeyboard.delay(10^Key);
        i = i + 1
      }
      else if (Key == 174){ //"®" = 2 Keys Combination
        Key = EEPROM.read(i+1);
        Key2 = EEPROM.read(i+2);
        DigiKeyboard.sendKeyStroke(Key, Key2);
        i = i + 2;
      }
      else if (Key == 175){ //"¯" = 3 Keys Combination
        Key = EEPROM.read(i+1);
        Key2 = EEPROM.read(i+2);
        Key3 = EEPROM.read(i+3);
        DigiKeyboard.sendKeyStroke(Key, Key2, Key3);
        i = i + 3;
      }
      else{
        DigiKeyboard.sendKeyStroke(Key);
      }    
      //optional: DigiKeyboard.delay(500);
    }
    digitalWrite(1,LOW);
    
  }
  
  else if (State == 2){
    Program = EEPROM.read(1);
  }

  
  /*DigiKeyboard.sendKeyStroke(0);  
  //DigiKeyboard.println("mode con>cols)18 lines)1");
  DigiKeyboard.sendKeyStroke(KEY_R,MOD_GUI_LEFT);
  DigiKeyboard.delay(500);
  DigiKeyboard.println("cmd");
  DigiKeyboard.delay(500);
  DigiKeyboard.println("color A");
  DigiKeyboard.println("cd %Desktop%");
  DigiKeyboard.println("dir");
  DigiKeyboard.delay(5000);*/

  
}



