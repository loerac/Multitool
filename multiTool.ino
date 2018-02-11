/*
 * Using the Adafruit Circuit Playground Classic as a multitool.
 * This will print out commands I use a lot and as a password
 * manager. Store the commands in CMD[] and the passwords in PWD[].
 */
#include<Adafruit_CircuitPlayground.h>
#include<Wire.h>
#include<SPI.h>
#include<Keyboard.h>

/*
 * Commands: Put your daily commands in CMD[]. You can use it for your
 * email(s), programming syntax, Linux commands, etc. The last three 
 * are reserver for Vim use. If you don't want to use those commands,
 * you can remove it and comment out the lines on line 138 & 144.
 */
char* CMD[] = {
  "#include<stdio.h>\n#include<stdlib.h>\n\nint main(int argc, char* argv[]) {\n\n\treturn 0;\n\b}" ,
  "#include<iostream>\nusing namespace std;\n\nint main(int argc, char* argv[]) {\n\n\treturn 0;\n\b}" ,
  "if(argc == 1) {\n\tprintf(\"Error: no arguments given\\n\");\nreturn 2;\n\b}\n" ,
  "" ,
  "" ,
  "" ,
  "" ,
  ":w" ,
  ":wq" ,
  ":q!"
} ;

/* 
 * Password Manager: Store your passwords in Circuit Playground The
 * only way to access the passwords is with a pin number, the pin
 * number needs to be between 0 -9. 0 is on the top left by "ON" LED
 * and 9 is on te top right by LED #13. Store your pin in PIN[] and
 * make sure the SIZE is the same size as PIN[].
 */
int SIZE = 4;   // SIZE of the PIN
int PIN[] = {0, 1, 2, 3};
char* PWD[] = {
  "NULL" ,
  "NULL" ,
  "NULL" ,
  "NULL" ,
  "NULL" ,
  "NULL" ,
  "NULL" ,
  "NULL" ,
  "NULL" ,
  "NULL"
} ;

int cmdIndex = 0;  // Current CMD index choosen
int pwdIndex = 0;  // Current PWD index choosen
int getIndex = 0;  // Get the index for cmdIndex and pwdIndex
int begState = 0;  // Read the beginning state of the slider
int curState = 0;  // Read what current state the slider is at
int grnColor = 0;  // Used for indicating which state slider is at
int redColor = 0;  // Used for indicating which state slider is at
int slider = 0; 

int const vimStart = 7;  // Knowing when the vim commands start
int const gitAccnt = 0;  // Knowing where the Github username is
int const gitPsswd = 0;  // Knowing where the Github password is

void setup() {
  Keyboard.begin();
  CircuitPlayground.begin();

  for(int i = 0; i < 3; i++) { LED_ON(7); LED_OFF(); }
  
  CircuitPlayground.setBrightness(15);
  CircuitPlayground.clearPixels();

  /*
   *  Grabbing initial state of the slider
   *  The initial state is used for CMD[].
   */
  begState = CircuitPlayground.slideSwitch();
}

void loop() {
  /*
   * I am going to have two uses for the slider;
   * State_1 : Commands
   * State_2 : Passwords
   */

  // State_1 : CMD
  if(CircuitPlayground.slideSwitch() == begState) { LED_ON(2); curState = 0; grnColor = 255; redColor = 0; }
  // State_2 : PWD 
  else { LED_ON(4); curState = 1; grnColor = 0; redColor = 255; }

  /* Indicate what LED the user is on by making the LED white */
  getIndex = (curState) ? cmdIndex : pwdIndex;
  CircuitPlayground.setPixelColor(getIndex, 255, 255, 255);

  /* Find out when the slider has changed */
  slider = CircuitPlayground.slideSwitch();
  while(slider == CircuitPlayground.slideSwitch()) {

    /* The right button is used for moving through the index */
    if(CircuitPlayground.rightButton()) {
      while(CircuitPlayground.rightButton()) {}
      getIndex = (curState) ? ++cmdIndex : ++pwdIndex;
      CircuitPlayground.setPixelColor(getIndex - 1, redColor, grnColor, 0);
      if(cmdIndex > 9) { cmdIndex = 0; getIndex = 0; }
      if(pwdIndex > 9) { pwdIndex = 0; getIndex = 0; }
      CircuitPlayground.setPixelColor(getIndex, 255, 255, 255);
    } 
    /* The left button is used for printing out CMD[getIndex] or PWD[getIndex] */
    else if(CircuitPlayground.leftButton()) {
      while(CircuitPlayground.leftButton()) {}
      if(curState) {
         
        /* 
         * If the pin number entered is correct,
         * print out the password and press the return key
        */
        if(enterPin()) {
          LED_ON(2);
          Keyboard.print(PWD[getIndex]);
          Keyboard.write(KEY_RETURN);
        } 
        /* Make the LED's red if the pin is incorrect */
        else { LED_OFF(); LED_ON(4); LED_OFF(); }
        LED_ON(4); 
        CircuitPlayground.setPixelColor(getIndex, 255, 255, 255);
      } else {
        // If you aren't going to use Vim, comment this line
        if(getIndex >= vimStart) { Keyboard.write(KEY_ESC); }
        
        Keyboard.print(CMD[getIndex]);

        /* 
         * Github use, if you aren't useing this for github
         * Comment the KEY_RETURN out and the if/else if out
        */
        Keyboard.write(KEY_RETURN);
        if(getIndex == gitAccnt && enterPin()) {
          LED_ON(2);
          Keyboard.print(PWD[gitPsswd]);
          Keyboard.write(KEY_RETURN);
        } else if(getIndex == gitAccnt) { Keyboard.write(KEY_RETURN); LED_OFF(); LED_ON(4); LED_OFF(); }
        LED_ON(2);
        CircuitPlayground.setPixelColor(getIndex, 255, 255, 255);
        
        // If you aren't going to use Vim, comment this line
        if(getIndex >= vimStart) { Keyboard.write(KEY_RETURN); }
      }
    }
  }  
}

bool enterPin() {
  /*
   * Adafruit does a really cool way of doing this:
   * https://learn.adafruit.com/circuit-playground-password-vault/password-vault-coding
   * I don't have copper tape so I just used a pin sequence with the two buttons
   */
  int i = 0;
  int pin = 0;
  int pinCnt = 0;
  LED_OFF();
  LED_ON(5);
  CircuitPlayground.setPixelColor(pin, 255, 255, 255);
  
  /* This is not the best way to store a password */
  while(i != SIZE) {
    if(CircuitPlayground.rightButton()) {
      while(CircuitPlayground.rightButton()) {}
      pin++;
      CircuitPlayground.setPixelColor(pin - 1, 255, 0, 255);
      if(pin > 9) { pin = 0; }
      CircuitPlayground.setPixelColor(pin, 255, 255, 255);
    }
    if(CircuitPlayground.leftButton()) {
      while(CircuitPlayground.leftButton()) {}
      if(pin == PIN[i]) { pinCnt++; }
      i++;
    }
  }

  if(pinCnt == SIZE) { return true; }
  return false;
}

void LED_OFF() {
  for(int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0, 0, 0);
    delay(50);
  }
}

/*
 * Send a bitwise number to display the LED's
 * Send one of these value through LED_ON
 * to get the color you want
 * White  = 7
 * Yellow = 6
 * Purple = 5
 * Red    = 4
 * Cyan   = 3
 * Green  = 2
 * Blue   = 1
 * None   = 0
 */
void LED_ON(int c) {
  int r = (c & 4) ? 255 : 0;
  int g = (c & 2) ? 255 : 0;
  int b = (c & 1) ? 255 : 0;
  
  for(int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, r, g, b);
    delay(50);
  }
}
