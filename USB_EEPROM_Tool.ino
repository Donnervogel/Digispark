#include <EEPROM.h>
#include <DigiUSB.h>
void setup() {
  // put your setup code here, to run once:
  DigiUSB.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  get_input();
}

void get_input() {
  String inString = "";
  int lastRead;
  int ava;
  int i=2;
  char c[2];
  // when there are no characters to read, or the character isn't a newline
  while (1==1) {
    ava = DigiUSB.available();
    DigiUSB.refresh();
    
    if (ava == 1){
      //something to read
      lastRead = DigiUSB.read();
      if (isDigit(lastRead)){
        inString += (char)lastRead;        
      }
      if (lastRead == '\n'){
        EEPROM.write(i,inString.toInt());
        inString = "";
        i++;
      }
      else if (lastRead == 170){ //ª for reading
        int key=0;
        for(int i=2;i<=511,i++){
          key=EEPROM.read(i);
          itoa(key,c,10);
          DigiUSB.write(c[0]);
          DigiUSB.write(c[1]);
          DigiUSB.write(c[2]);
          DigiUSB.write('\n');
          DigiUSB.delay(10); 
        }
      }      
      else if (lastRead == 171){ //« for erasing
        for(int del=2; del<=511; del++){
          EEPROM.write(del,0);
        }
      }
      else if (lastRead == 172){ //¬ for ending
        DigiUSB.write("Ende: ");
        ava = 2;
      }
      itoa(i,c,10);
      DigiUSB.write(c[0]);
      DigiUSB.write(c[1]);
      DigiUSB.write(c[2]);
      DigiUSB.write('\n');
      if (i == 511){
        DigiUSB.write("EEPROM Ende!");
        ava = 2;      
      }
    }

    if (ava == 2){
      EEPROM.write(0,0);
      break;
    }
    // refresh the usb port
    DigiUSB.refresh();
    delay(10);

  }

}

