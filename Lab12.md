# Lab 12

## Lab Tasks

I figured the best way to approach this lab was to start with the simplest solution possible, and then increase system complexity as necessary until achieving a satisfactory solution. 

### ORIENTATION + MOTOR PID CONTROL

My initial plan was to implement a code which would have my robot follow the outlined path, using distance measurements from the front ToF sensor to know when to stop itself and turning to specific angles using orientation PID control. The primary goal here was to find out how well I could navigate across the points without implementing localization. I initially considered full open loop control and using timing to control 

While figuring out the necessary distances, I decided to pivot from the most direct path (shown below). 

*insert path here*

During the beginning, instead of turning at 45 and 60 degree angles to follow the optimal path, I opted to instead use the trajectory below:

*insert path here*

By implementing this path, open loop navigation becomes much more simple as all the necessary rotations become 90 degrees and the robot theoretically only moves in four fixed directions (up, down, left, and right).

I implemented my code using a new case called FINAL_OPEN_LOOP which takes in a total of 14 inputs. The code is shown below:

*insert case code snippet below*

The first input corresponds to the variable OPEN_LOOP_on. Similar to other labs, this serves as a flag which intiiates the necessary functions inside the void loop when set to 1. The next 7 inputs correspond to the ToF sensor measurements at which the robot should stop. The following 6 inputs correspond to the angles of rotation for the robot in between each direct movement. I chose to have these values as inputs instead of hard coding them into my script to make adjusting values easier as I would not have to reupload my code each time. 

Given these inputs, when OPEN_LOOP_on is turned on, it triggers the following code to begin inside my void loop:

*insert code snippet here*

This code uses functions openAngle and driveForward which can be found below: 

*insert code snippets here*

I fine tuned the values 








## References 

I worked on this lab by myself. 

Thank you Professor Helbling and the TA team for a great semester! All the office hours were greatly appreciated. This class has been one of my favorites I've ever taken at Cornell (even if it did take years off my life between the sleep deprivation and solder fumes lol)
