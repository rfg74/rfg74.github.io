# Lab 9

*Note: I had issues with my Artemis crashing and was unable to get my orientation control in time to collect ToF sensor data in the lab room. To demonstrate my functioning mapping code, I used my bedroom instead.* 

### Orientation Control

To accomplish the mapping, I used orientation control. Initially, it took a lot of debugging and tweaking to refine my PID control. After many, many hours I found that the best values for my system was [insert PID values here]. Once fully tuned, I was able to get my robot to work in 24 degree increments, taking 3 ToF sensor readings per increment. In order to run this process, I added the following case command to my code:

*INSERT CODE SNIPPET HERE*

This command controls the car so it turns to each angel using my PID controller until the car is within a "close enough" range which I have labeled as 4 degrees. Once stopped, the car takes 3 ToF sensors from the front sensor. This yields me 60 data points per spot. 

*INSERT VIDEO OF CODE WORKING*

### Readings Analysis

By taking measurements across four points in the room, I avereged the 4 collected data points and took that as the main reading. I then adapted the recorded angles. 

I took this data and plotted it to a showto the readings of P diemnstis while easoIa onnjuring hol
\*INSERT CHARTS HRE*

%%% TRANSFORMATION MATHH{


