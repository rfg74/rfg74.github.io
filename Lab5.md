# Lab 5

For this lab, I have opted to implement a PID controller into my system. 

## Prelab

In order to simplify debugging, I first determined the method I would use to send and recieve data over Bluetooth. To accomplish this, I initialized two Arduino cases. The first case, named PID, starts and stops my PID controller. The second case, named PID_DATA, sends the collected PID data to Jupyter Notebook. For the PID case, the command inputs consist of a start/stop value (1 and 0 respectively), the desired end distnace from the wall, as well as the three controller values K_p, K_i, and K_d. The PID controllers function stops when the first value is set to 0. 

While designing the PID_DATA case, I decided that the relevant information that I needed to send to Jupyter Notebook consisted of: time, distance (inches), error, P, I, D, and PID values, as well as the motor input.


## Lab Tasks

Before implementing my PID controller, I ran some preliminary tests to see how my robot would perform with the task. To do so, I combined my codes from labs 3 and 4 to create a simple code which would drive the car forward at a set speed until the front ToF sensor detected a distance of 1 foot from the wall. To test the error, I placed a ruler next to my car to quantify how close it got to the wall before actually coming to a full stop. Starting at a base speed of 75, I ran this test until my robot got uncomfortably close to making a full speed collision with the wall. 

