# Lab 7

## Lab Tasks

### 1. Estimate Drag and Momentum

I estimated the drag and momentume values necessary for the A and B matricies using a step response. I accomplished this by driving my robot towards a wall at a constant motor input while the ToF sensor collected distance measurements. From this data, I calculated the velocity and plotted it to find the 90% rise time. I selected my step input u(t) based on my calibration factor such that one wheel was operating at 250 PWM. This input was approximately 72% of the maximum PWM. This PWM value allowed for my robot to move as fast as possible while still moving in a straight line for long distances. 

To conduct there trials I implemented the following two cases into my code to initiate the trial and collect data.

#### MAX_SPEED
This case clears all previous data, starts my front ToF sensor, initiates storage of time data, and sets DRIVE_on to 1 (this is initialized as 0 to keep the car off until the command is called). For the MAX_SPEED case, the command inputs consist of the DRIVE_on value and the array size "max" for data collection.

#### GET_MAX_SPEED_DATA
This case transmits the collected data to my computer via Bluetooth. While designing the GET_MAX_SPEEED_DATA case, I decided that the relevant information that I needed to send to Jupyter Notebook consisted of: time, distance (mm), and PWM value. 

Collecting data over a period of two seconds, I collected the following data:

<img width="248" alt="Profile Picture" src="DT.jpg">

In order to calculate the drag and momentum, I used the following equations below:

<div style="display: flex; gap: 10px;">
  <img width="375" alt="Profile Picture" src="DRAG.jpg">
  <img width="375" alt="Profile Picture" src="MOMENTUM.jpg">
</div>


From my trials using a PWM value of 180, I found that my 90% value for speed is 2.74968 m/s which occurs at approximately 1.237 seconds. Using the aforementioned equations, I got the following values for drag and momentum. 

d = 1 / 2.74968 = 0.36368
k = (-0.36368 * 1.237)/ln(0.1) = 0.19537

### 2. Initialize Kalman Filter (Python)

I found my A and B matricies using the following formula and my calculated values for d (drag) and m (momentum):

[insert A and B matricies here]

These matricies were calculated as seen below:

[insert A here]
[insert B here]

In order to discretize the matrices, I found the sampling time

### 3. Implement and Test Kalman Filter in Jupyter (Python)
### 4. Implement the Kalman Filter on the Robot








