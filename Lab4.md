# Lab 4

## Prelab

Below is a wiring diagram for how I intend to connect the dual motor drivers to my Artemis board. I connected the two VIN pins togethers and then connected them to the battery. I did the same process for the two GND pins. I bridged together the BOUT1 and AOUT1 pins which I then connected to the DC motor. I did the same process for the BOUT2 and AOUT2 pins. This was done for both of the dual motor drivers. For my dual motor driver which was also connected to ground with the Artemis board, I bridged together BIN1 and AIN1  and connected that to pin 11 on the Artermis. The same was done for BIN2 and AIN2 which was connected to pin 12. On the other dual motor driver, I bridged BIN1 and AIN1 and connected them to pin to pin 6. BIN2 and AIN2 were also bridged together and connected to pin 7. 

<img width="500" alt="Profile Picture" src="IMG_1234.jpg">

The Artemis board and the motors/motor drivers are being powered by seperate batteries such that the Artemis board is powered by a 650 mAh battery and the motors are powered by an 850 mAh battery. This was done intentionally. Having these components powered by different batteries allows them to function independently as well as avoids electrical noise and voltage drops caused by current fluctuation in the motors. Furthermore, the motors require a higher current battery as opposed to the Artemis board which is why they are being powered by the 850mAh battery. 
