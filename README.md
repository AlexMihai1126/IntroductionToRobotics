# IntroductionToRobotics (2023-2024)

This repository will contain my work for the "Introduction to Robotics" course taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest. Each homework will include requirements, code, photos, videos and details about the implementation.

## Homework 1
Task given by teacher: "Use a separate potentiometer for controlling each color of the RGB LED:Red,Green, andBlue.  This control must leveragedigital electronics.  Specifically,you  need  to  read  the  potentiometer’s  value  with  Arduino  and  then  write  amapped value to the LED pins"

Only one .ino file was necessary to implement the task, found in the folder "Homework 1".

Image of the setup: ![](https://github.com/AlexMihai1126/IntroductionToRobotics/blob/67317ad8fd8e71dceb1ffa385d8b0d63d7c1d8ac/Homework%201/images/hw1_img1.jpg)
Video of the setup in action: https://youtu.be/7HbvjDEhaHs

## Homework 2
Task: "This assignment involves simulating a 3-floor elevator control system using LEDs, buttons, and a buzzer with Arduino. By the end of this task, you will gain experience in using button state change, implementing debouncing techniques, and coordinating multiple components to represent real-world scenarios. Design a control system that simulates a 3-floor elevator using the Arduino platform."

Components used: 4 LEDs (3 blue LEDs, one for each floor, and one yellow LED for indicating the elevator's state), 3 buttons, one for each floor to call the elevator, 4 330 ohm resistors, one for each LED, one buzzer to make a sound when state changes happen, a 100 ohm resistor for the buzzer, breadboard and necessary wires. Last updated (HW2) 03.11.2023 20:12

The code file for the setup is located in the folder "Homework 2".

Image of the setup: ![](https://github.com/AlexMihai1126/IntroductionToRobotics/blob/main/Homework%202/images/IMG_4800.jpg)

Video of the setup: https://youtu.be/Nxbz4RIpOkA

## Homework 3

Task: "You will use the joystick to control the position ofthe segment and ”draw” on the display.  The movement between segmentsshould be natural, meaning they should jump from the current positiononly to neighbors, but without passing through ”walls”."

How it should operate: "The initial position should be on the DP. The current position always blinks (irrespective of the fact that the segment is on or off). Use the joystick to move from one position to neighbors (see table for corresponding movement). Short pressing the button toggles the segment state from ON to OFF or from OFF to ON. Long pressing the button resets the entire display by turning all the segments OFF and moving the current position to the decimal point."

How the segments move: ![](https://github.com/AlexMihai1126/IntroductionToRobotics/blob/main/Homework%203/images/Screenshot%202023-11-05%20204835.png)

Components used: one 7-segment display, one red LED, 9 330 ohm resistors for each LED (the 8 in the display and the additional LED), joystick, breadboard and the necessary wires

The code file for the setup is located in the folder "Homework 3".

Image of the setup: ![](https://github.com/AlexMihai1126/IntroductionToRobotics/blob/main/Homework%203/images/IMG_4923.jpeg)

Video of the setup: https://youtu.be/UzxzS_WTsHg

Notes: an extra red LED was used to let the user know if the current segment has been clicked or not (as it blinks when it's been selected and hard to know what state it is currently in). Interrupts were not used and long pressing the joystick does not reset the board.

## Homework 4
Task given was to implement a stopwatch just like the app on a phone using an Arduino. It counts with a precision of tenths of a second. It starts at 000.0, pressing the start button (1st button) begins the timer. Pressing the 2nd button saves a lap (the current time at the moment the button was pressed) and saves up to 4 values, overwriting them when we reach the limit. The reset button (3rd button) puts the time back at 000.0. The start/stop button and lap button use interrupts for added measuring precision. At 999.9 the timer will go back to 000.0 and start again automatically. It should also allow the user to view the laps after the timer has been reset manually, and pressing the reset button while in the lap view mode should erase the laps and start over (not working on my setup however).

Components used: 4-digit 7-segment display, 8x 330 ohm resistors for each of the LEDs, 3 pushbuttons and a 74HC595 shift register, along with a breadboard and necessary wires.

The code file for the setup is located in the folder "Homework 4".

Image of the setup: ![](https://github.com/AlexMihai1126/IntroductionToRobotics/blob/main/Homework%204/images/20231113_164438551_iOS.jpg)

Video of the setup: https://youtu.be/aYq58uCmPIw

## Homework 5
