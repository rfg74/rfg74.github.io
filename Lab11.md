# Lab 11

## Lab Tasks

### Localization in Simulation

I ran the Bayes Filter simulation provided to us in notebook *lab11_sim.ipynb* which resulted in the final plot shown below:

<img width="560" alt="Profile Picture" src="FINAL_PLOT.jpg">

The red line represents the odometry, the green is the ground truth, and the blue line represents the belief. 

### Arduino Code

In order to perform localization, I adapted my code from Lab 9 in in order to rotate my robot 360 degrees and collect distance measurements in a a predefined interval. I adapted my code to best fit the purposes of this lab. For this, I defined 2 cases "MAP_ORIENTATION" and "SEND_MAP_ORIENTATION_DATA". The first case initiates/terminates orientation control while the second sends the collected data. All additional code was integrated into "void loop()" 

#### MAP_ORIENTATION

I revised my codes from Lab 9 & 11 such that my robot would turn in 20 degree increments. The code I used can be found below:

<img width="560" alt="Profile Picture" src="MAP_ORIENTATION.jpg">

#### SEND_MAP_ORIENTATION_DATA

I also revised my code for sending data such that it only collects the angle, time, and distance data at each angle increment. The code I used can be found below:

<img width="560" alt="Profile Picture" src="SEND_MAP_ORIENTATION_DATA.jpg">

#### VOID LOOP()

In my main loop I used a flag variable to turn on the orientation and begin data collection to store distance data and yaw values within their respective arrays. The code I used can be found below:

<div style="display: flex; gap: 10px;">
  <img width="560" alt="Profile Picture" src="LOOP_1.jpg">
  <img width="560" alt="Profile Picture" src="LOOP_2.jpg">
</div>

### Implementing PERFORM_OBSERVATION_LOOP

### Running Robot Localization


