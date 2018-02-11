# Multitool
Using the Circuit Playground from Adafruit for commands and passwords manager. There are currently two states:
- State_1: commands (CMD)
- State_2: passwords (PWD)

The states are indicated from the slide switch. To know what state you are on, the LED's will be:
- State_1: GREEN
- State_2: RED
- Entering Password: PURPLE

# Currently working on:
I am going to add more buttons to the circuit playground to add more states:
- State_3: temperature (TMP)
- State_4: compass (CMP)

# Initialize
- Enter in the commands in CMD[]
- Enter the pin number you want in PIN[].
  - This pin number can be from 0 - 9 and be any size you want
- Enter in the passwords in PWD[]

### Vim
- If you **ARE** going to use Vim with the circuit playground, put your commands in CMD[] starting from the end in CMD[]
  - Make vimStart equal to the first index of your vim command from CMD[]
- If you **ARE NOT** going to use Vim with the circuit playground, comment out lines:
   - Line 61
   - Line 130
   - Line 148

### Github
- If you **ARE** going to use Github with the circuit playground, put your username in CMD[] and password in PWD[]
  - Make gitAccnt equal to the index where your username is in CMD[]
  - Make gitPsswd equal to the index where your password is in PWD[]
- If you **ARE NOT** going to use Github with the circuit playground, comment out:
  - Line[ 62 - 63 ]
  - Line[ 138 - 145 ]

# How-to
Using the slide switch, it will indicate which state you are on. When the LED's are green, you're on vim mode. When it the LED's are red, you're on password mode. The right button will move through the index of the arrays. The left button will print out the index you are on. The rigtht button is on the side of the temperature sensor and the sound sensor. The right button is on the side next to the light sensor and the mini speaker.

### Password
Choose the password you want and then enter the pin number you set up in PIN[]. Use the right button to move through the index, where 0 is on the top left and 9 is on the top right. Use the left button to enter the pin selected. If you entered the correct pin number, the LED's will go green. If not, the LED's will go red.

### Github
When you are going to push your commits on the command line, make sure you are in State_1: CMD. Move to your username you put in CMD[] and press the left button. The LED's will then turn purple indicating you to enter your password for your github account you put in PWD[].

