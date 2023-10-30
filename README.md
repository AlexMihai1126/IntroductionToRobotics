# IntroductionToRobotics (2023-2024)

This repository will contain my work for the "Introduction to Robotics" course taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest. Each homework will include requirements, code, photos, videos and details about the implementation.

## Homework 1
Task given by teacher: "Use a separate potentiometer for controlling each color of the RGB LED:Red,Green, andBlue.  This control must leveragedigital electronics.  Specifically,you  need  to  read  the  potentiometer’s  value  with  Arduino  and  then  write  amapped value to the LED pins"

Only one .ino file was necessary to implement the task, found in the folder "Homework 1".

Image of the setup: ![](https://github.com/AlexMihai1126/IntroductionToRobotics/blob/67317ad8fd8e71dceb1ffa385d8b0d63d7c1d8ac/Homework%201/images/hw1_img1.jpg)
Video of the setup in action: https://youtu.be/7HbvjDEhaHs

## Homework 2
Task: "This assignment involves simulating a 3-floor elevator control system using LEDs, buttons, and a buzzer with Arduino. By the end of this task, you will gain experience in using button state change, implementing debouncing techniques, and coordinating multiple components to represent real-world scenarios. Design a control system that simulates a 3-floor elevator using the Arduino platform."

Components used: 4 LEDs (3 blue LEDs, one for each floor, and one yellow LED for indicating the elevator's state), 3 buttons, one for each floor to call the elevator, 4 330 ohm resistors, one for each LED, one buzzer to make a sound when state changes happen, a 100 ohm resistor for the buzzer, breadboard and necessary wires.

The code file for the setup is located in the folder "Homework 2".

Image of the setup: ![](https://github.com/AlexMihai1126/IntroductionToRobotics/blob/main/Homework%202/images/IMG_4800.jpg)

Video of the setup: UPCOMING (when the setup will be fully functional, after the deadline if possible, see note below).

Notes about HW2: I have not been able to get the setup fully working, and won't be able to do so until the deadline, I accept losing A LOT of points on this homework because it's not working properly, but I want to understand the millis function better and why my implementation here is flawed and does not work properly. Last updated (HW2) 30.10.2023 22:40
