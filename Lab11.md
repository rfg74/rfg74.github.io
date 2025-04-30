# Lab 11

## Lab Tasks

### Localization in Simulation

I ran the Bayes Filter simulation provided to us in notebook *lab11_sim.ipynb* which resulted in the final plot shown below:

<img width="560" alt="Profile Picture" src="FINAL_PLOT.jpg">

The red line represents the odometry, the green is the ground truth, and the blue line represents the belief. 

### Arduino Code

In order to perform localization, I adapted my code from Lab 9 in in order to rotate my robot 360 degrees and collect distance measurements in a a predefined interval. I adapted my code to best fit the purposes of this lab. For this, I defined 2 cases "MAP_ORIENTATION" and "SEND_MAP_ORIENTATION_DATA". The first case initiates/terminates orientation control while the second sends the collected data. All additional code was integrated into "void loop()" 

*Note: Apologies for the bluriness of these pictures, I can't figure out why they look like that*

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

#### Data Collection

*insert video of car spinning and collecting data*

### Implementing PERFORM_OBSERVATION_LOOP

This function is designed to collect the 18 data points documented during the MAP_ORIENTATION case's 360 degree rotation and store the data points in the arrays *sensor_ranges* and *sensor_bearings*. These arrays are then returned and used during the update step. I utilized the asyncio function such that I could run the while loop continuously until all the data points were sent over. The for loop then attaches the data points to their corresponding array as well as converts the mm values into meters. 

<img width="560" alt="Profile Picture" src="PERFORM_OBSERVATION_LOOP.jpg">

I put my command to begin the rotation outside of the defintion of the perform_observation_loop function. In doing so, I made it easier to begin/terminate the rotation. The command I wrote to run the MAP_ORIENTATION case can be seen below:

<img width="560" alt="Profile Picture" src="CASE.jpg">

### Running Robot Localization (Run One)

Below you will find the results of my real robot localization across the four predetermined positions (-3, -2), (0, 3), (5, -3), and (5, 3). At each point, I began my robots angular position at 0 (facing the right). After many trials and tribulations, I got two good runs for each of the desired points (omitting the origin). On the plot, the green represents the ground truth (i.e. the actual location of the robot) and the blue represents the belief of the robot's position. I collected data with my ToF sensor located at the front of the car, turning my robot in a counter clockwise direction. 

#### Top Right (5, 3)

<div style="display: flex; gap: 10px;">
  <img width="560" alt="Profile Picture" src="TR_1.jpg">
  <img width="560" alt="Profile Picture" src="TR_1_PLOT.jpg">
</div>

#### Bottom Right (5, -3)

<div style="display: flex; gap: 10px;">
  <img width="560" alt="Profile Picture" src="BR_1.jpg">
  <img width="560" alt="Profile Picture" src="BR_1_PLOT.jpg">
</div>

#### Top Middle (0, 3)

<div style="display: flex; gap: 10px;">
  <img width="560" alt="Profile Picture" src="TM_1.jpg">
  <img width="560" alt="Profile Picture" src="TM_1_PLOT.jpg">
</div>

#### Bottom Left (-3, -2)

