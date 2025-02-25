# Lab 3:

## Prelab

From the data sheet, the Time-of-Flight sensor's default I2C address is 0x52. Since both sensors have the same default address, I plan to use the XSHUT pin to change the address of one of the sensors to allow for simultaneous use. By soldering a wire to from one sensor's XSHUT pin to the Artmis board's A3, I can shut down the second ToF sensor while the first sensor is turned on, having one keep the deafult address and allowing the ther to be changed. I believe that this is a more compatible solution as opposed to the alternative since it minimizes wiring. 

I intend to place the two ToF sensors at the front of the robot in the left and right corners. This diagonal configuration will help to enhance object detection from the sides as well as provide a wider field-of-view than a tradtional heads-on placement. There is however potential for objects that are head on. 

I connected the Artemis Nano board to the QWIK connector with one of the smaller two connectors. I then used the second smaller connector to connect the IMU to the QWIK connector. I then used the two longer connectors and soldered them directly to the ToF sensors. Using the longer connectors allows for more flexibility in where I'm able to place the sensors. 

<img width="500" alt="Profile Picture" src="IMG_1738.jpeg">
