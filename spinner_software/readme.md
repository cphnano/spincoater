FAQ
-----
 - Q: How do I use the spinner software?
 
   A: Download [processing](https://processing.org/) and then open spinner_software/spinner_software.pde

 - Q: The spinner software won't connect! What am I doing wrong? 
 
   A: Make sure that you are connected to the right COM port and using the correct baudrate. This can be set on lines 8 and 9 in spinner_software.pde. Ensure that the arduino is connected and is running the correct firmware.
   
 - Q: How do I make a spin profile?
   
   A: See "Creating spin profiles" below.
   

Creating spin profiles:
------
You can create a spin profile in a text editor (such as notepad). A spin profile consists of multiple linear ramps in succession. See 'ramp.txt' for an example. This profile does the following: 
 - At 2 seconds, start ramp up to 1500RPM over 3 seconds. 
 - At 7 seconds, ramp up from 1500RPM to 3000RPM over 5 seconds.
 - At 15 seconds, ramp down to 500RPM over 2 seconds. 
 
 As a spin profile this is written as:
 ```
 2.0 3.0 1500.0
 7.0 5.0 3000.0
 15.0 2.0 500.0
 ```
 Each line consists of 3 numbers that describes the ramp: The first number is the time (in seconds) to start the ramp, second number is the duration of the ramp (in seconds) and the last number is the target spin speed (in RPM). 
 
 After each ramp is completed the spinner will hold its speed until the next ramp (or stopped). The starting spin speed of each successive ramp is the ending spin speed of the previous ramp (0RPM for the first ramp). 
 
 This way, a simple spin profile consisting of a linear ramp-up and then holding the speed can be written as a single line. 
 
 Note that the lines must be in the order that the ramps should be performed! 
