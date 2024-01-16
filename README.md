# IntroductionToRobotics (2023-2024)

This repository will contain my work for the "Introduction to Robotics" course taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest. Each homework will include requirements, code, photos, videos and details about the implementation.

<details>
<summary>Homework 1</summary>
  
Task given by teacher: "Use a separate potentiometer for controlling each color of the RGB LED:Red,Green, andBlue.  This control must leveragedigital electronics.  Specifically,you  need  to  read  the potentiometer’s  value  with  Arduino  and  then  write  amapped value to the LED pins"
<br>

Only one .ino file was necessary to implement the task, found in the folder "Homework 1".
<br>

Image of the setup: ![](https://github.com/AlexMihai1126/IntroductionToRobotics/blob/67317ad8fd8e71dceb1ffa385d8b0d63d7c1d8ac/Homework%201/images/hw1_img1.jpg)
<br>

Video of the setup in action: https://youtu.be/7HbvjDEhaHs
</details>

<details>
<summary>Homework 2</summary>
  
Task: "This assignment involves simulating a 3-floor elevator control system using LEDs, buttons, and a buzzer with Arduino. By the end of this task, you will gain experience in using button state change, implementing debouncing techniques, and coordinating multiple components to represent real-world scenarios. Design a control system that simulates a 3-floor elevator using the Arduino platform."
<br>

Components used: 4 LEDs (3 blue LEDs, one for each floor, and one yellow LED for indicating the elevator's state), 3 buttons, one for each floor to call the elevator, 4 330 ohm resistors, one for each LED, one buzzer to make a sound when state changes happen, a 100 ohm resistor for the buzzer, breadboard and necessary wires. Last updated (HW2) 03.11.2023 20:12
<br>

The code file for the setup is located in the folder "Homework 2".
<br>

Image of the setup: ![](https://github.com/AlexMihai1126/IntroductionToRobotics/blob/main/Homework%202/images/IMG_4800.jpg)
<br>

Video of the setup: https://youtu.be/Nxbz4RIpOkA
</details>

<details>
<summary>Homework 3</summary>

Task: "You will use the joystick to control the position ofthe segment and ”draw” on the display.  The movement between segmentsshould be natural, meaning they should jump from the current positiononly to neighbors, but without passing through ”walls”."
<br>

How it should operate: "The initial position should be on the DP. The current position always blinks (irrespective of the fact that the segment is on or off). Use the joystick to move from one position to neighbors (see table for corresponding movement). Short pressing the button toggles the segment state from ON to OFF or from OFF to ON. Long pressing the button resets the entire display by turning all the segments OFF and moving the current position to the decimal point."
<br>

<details>
<summary>How the segments move</summary>
Image: ![](https://github.com/AlexMihai1126/IntroductionToRobotics/blob/main/Homework%203/images/Screenshot%202023-11-05%20204835.png)
</details>
 
<br>

Components used: one 7-segment display, one red LED, 9 330 ohm resistors for each LED (the 8 in the display and the additional LED), joystick, breadboard and the necessary wires
<br>

The code file for the setup is located in the folder "Homework 3".
<br>

Image of the setup: ![](https://github.com/AlexMihai1126/IntroductionToRobotics/blob/main/Homework%203/images/IMG_4923.jpeg)
<br>

Video of the setup: https://youtu.be/UzxzS_WTsHg
<br>

Notes: an extra red LED was used to let the user know if the current segment has been clicked or not (as it blinks when it's been selected and hard to know what state it is currently in). Interrupts were not used and long pressing the joystick does not reset the board.
</details>

<details>
<summary>Homework 4</summary>
Task given was to implement a stopwatch just like the app on a phone using an Arduino. It counts with a precision of tenths of a second. It starts at 000.0, pressing the start button (1st button) begins the timer. Pressing the 2nd button saves a lap (the current time at the moment the button was pressed) and saves up to 4 values, overwriting them when we reach the limit. The reset button (3rd button) puts the time back at 000.0. The start/stop button and lap button use interrupts for added measuring precision. At 999.9 the timer will go back to 000.0 and start again automatically. It should also allow the user to view the laps after the timer has been reset manually, and pressing the reset button while in the lap view mode should erase the laps and start over (not working on my setup however).
<br>
  
Components used: 4-digit 7-segment display, 8x 330 ohm resistors for each of the LEDs, 3 pushbuttons and a 74HC595 shift register, along with a breadboard and necessary wires.
<br>

The code file for the setup is located in the folder "Homework 4".
<br>

Image of the setup: ![](https://github.com/AlexMihai1126/IntroductionToRobotics/blob/main/Homework%204/images/20231113_164438551_iOS.jpg)
<br>

Video of the setup: https://youtu.be/aYq58uCmPIw
</details>

<details>
<summary>Homework 5</summary>
Task given was to implement a data logger and monitor for a few sensors (distance, light, and bonus temperature and humidity) and control an RGB LED (if it's set to Auto it's green if all sensors are within the set ranges, but turns red if any of the sensors are out of range and of set to Manual you can set the desired color via 3 parameters (Red,Green and Blue). All configuration data is stored in the EEPROM of the Arduino. You can configure the update rate of the sensors (in seconds), the maximum threshold (in my implementation) of the sensors and the LED mode.
<br>
  
Components used: Ultrasonic distance sensor (1x), photocell (1x) along with a 10K resistor for it, an RGB LED (and 1x 330 ohm resistors for each of the channels - for a total of 3) and a combined temperature/humidity sensor I had from a previous project (DHT11).
<br>

The code file is in the folder "Homework 5/hw5_code2". Please note that the first iteration was meant to be able to choose both a minimum and maximum value for each sensor, but the input method did not work as expected so I made a clone of the project and reverted to using just max threshold values.
<br>

Image of the setup: ![](https://github.com/AlexMihai1126/IntroductionToRobotics/blob/main/Homework%205/image/20231120_200857510_iOS.jpg)
<br>

Video of the setup: https://youtu.be/LGYc3vCyYwg
  
</details>

<details>
<summary>Homework 6 (Matrix checkpoint 1)</summary>
  
Image of the setup: ![](https://github.com/AlexMihai1126/IntroductionToRobotics/blob/main/Homework%206/20231128_104707003_iOS.jpg)
</details>

<details>
<summary>Homework 7 (Matrix checkpoint 2)</summary>
Task was to implement the menu of the matrix game project.
<br>
  
Image: ![](https://github.com/AlexMihai1126/IntroductionToRobotics/blob/main/Homework%207/img/20231212_091119332_iOS.jpg)
</details>

