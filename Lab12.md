# Lab 12

## Lab Tasks

I figured the best way to approach this lab was to start with the simplest solution possible, and then increase system complexity until achieving a satisfactory solution. 

### Trial 1: Open Loop

For my first attempt, I decided to run this using an open loop system. The primary goal here was to find out which points would require localization, and which wouldn't. My initial plan was to implement a code which would have my robot follow the outlined path and yse measurements from the front ToF sensor to know when to stop itself. While figuring out the necessary distances, I decided to pivot from the most direct path. During the beginning, instead of turning at 45 and 60 degree angles to follow the optimal path, I opted to instead use a trajectory in which only 90 degree angles were necessary for every rotation to keep things simple. Additionally, this made it more reliable as I no longer needed to worry about large, innacuarate measurements from the ToF sensor. 
