#include<Adafruit_CircuitPlayground.h>
#include<Wire.h>
#include<SPI.h>
#include<Keyboard.h>

char* VIM[] = {
  "#include<stdio.h>\n#include<stdlib.h>\n\nint main(int argc, char* argv[]) {\n\n\treturn 0;\n\b}" ,
  "#include<iostream>\nusing namespace std;\n\nint main(int argc, char* argv[]) {\n\n\treturn 0;\n\b}" ,
  "if(argc == 1) {\n\tprintf(\"Error: no arguments given\\n\");\nreturn 2;\n\b}\n" ,
  "Train Drop\n\tDip Drop\n\t\tAnd I want that grass with cheese drop\n" ,
  "" ,
  "" ,
  "" ,
  ":w" ,
  ":wq" ,
  ":q!"
} ;

int SIZE = 4;   // SIZE of the PIN
int PIN[] = {0, 1, 2, 3};
char* PWD[] = {
  "Train" ,
  "Drop" ,
  "Dip" ,
  "Drop" ,
  "I" ,
  "want" ,
  "that" ,
  "ham" ,
  "with" ,
  "cheese"
} ;

int vimIndex = 0;  // Current VIM index choosen
int pwdIndex = 0;  // Current PWD index choosen
int getIndex = 0;  // Get the index for vimIndex and pwdIndex
int begState = 0;  // Read the beginning state of the slider
int curState = 0;  // Read what current state the slider is at
int grnColor = 0;  // Used for indicating which state slider is at
int redColor = 0;  // Used for indicating which state slider is at
int slider = 0; 

void LED_OFF();
void LED_ON(int c);

void setup() {
  Keyboard.begin();
  CircuitPlayground.begin();

  for(int i = 0; i < 3; i++) {
    LED_ON(7);
    LED_OFF();
  }
  
  CircuitPlayground.setBrightness(15);
  CircuitPlayground.clearPixels();
  begState = CircuitPlayground.slideSwitch();
}

void loop() {
  /*
   * I am going to have two uses for the slider;
   * State_1 : Vim
   * State_2 : Passwords
   */

  // State_1 : VIM 
  if(CircuitPlayground.slideSwitch() == begState) { LED_ON(2); curState = 0; grnColor = 255; redColor = 0; }
  // State_2 : PWD 
  else { LED_ON(4); curState = 1; grnColor = 0; redColor = 255; }

  // Indicate what vimIndex the user is on by making the LED white
  getIndex = (curState) ? vimIndex : pwdIndex;
  CircuitPlayground.setPixelColor(getIndex, 255, 255, 255);

  slider = CircuitPlayground.slideSwitch();
  while(slider == CircuitPlayground.slideSwitch()) {
    if(CircuitPlayground.rightButton()) {
      while(CircuitPlayground.rightButton()) {}
      getIndex = (curState) ? ++vimIndex : ++pwdIndex;
      CircuitPlayground.setPixelColor(getIndex - 1, redColor, grnColor, 0);
      if(vimIndex > 9) { vimIndex = 0; getIndex = 0; }
      if(pwdIndex > 9) { pwdIndex = 0; getIndex = 0; }
      CircuitPlayground.setPixelColor(getIndex, 255, 255, 255);
    } else if(CircuitPlayground.leftButton()) {
      while(CircuitPlayground.leftButton()) {}
      if(curState) {
        /* 
         * Currently working on to make it password protected before it prints out the password
         * Adafruit does a really cool way of doing this:
         * https://learn.adafruit.com/circuit-playground-password-vault/password-vault-coding
         * I am working on making a sequence of LED's user has to push
         */
        int i = 0;
        int pin = 0;
        int pinCnt = 0;
        LED_OFF();
        LED_ON(5);
        CircuitPlayground.setPixelColor(pin, 255, 255, 255);
        while(i != SIZE) {
          if(CircuitPlayground.rightButton()) {
            while(CircuitPlayground.rightButton()) {}
            pin++;
            CircuitPlayground.setPixelColor(pin - 1, 255, 0, 255);
            CircuitPlayground.setPixelColor(pin, 255, 255, 255);
            if(pin > 9) { pin = 0; }
          }
          if(CircuitPlayground.leftButton()) {
            while(CircuitPlayground.leftButton()) {}
            if(PIN[pin] == PIN[i]) { pinCnt++; }
            i++;
          }
        }
        if(pinCnt == SIZE) {
          LED_ON(2);
          Keyboard.print(PWD[getIndex]);
          LED_ON(4);
          CircuitPlayground.setPixelColor(getIndex, 255, 255, 255);
        } else { LED_OFF(); LED_ON(4); LED_OFF(); LED_ON(4); }
        CircuitPlayground.setPixelColor(getIndex, 255, 255, 255);
      } else {
        if(getIndex > 6) { Keyboard.write(KEY_ESC); }
        Keyboard.print(VIM[getIndex]);
        if(getIndex > 6) { Keyboard.write(KEY_RETURN); }
      }
    }
  }  
}

void LED_OFF() {
  for(int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0, 0, 0);
    delay(50);
  }
  delay(100);
}

void LED_ON(int c) {
  int r = (c & 4) ? 255 : 0;
  int g = (c & 2) ? 255 : 0;
  int b = (c & 1) ? 255 : 0;
  
  for(int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, r, g, b);
    delay(50);
}
  delay(100);
}
