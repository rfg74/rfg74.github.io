#include "BLECStringCharacteristic.h"
#include "EString.h"
#include "RobotCommand.h"
#include <ArduinoBLE.h>

//////////// ToF ////////////
#include <Wire.h>
#include "SparkFun_VL53L1X.h"
#define XSHUT2 3 // Blue

SFEVL53L1X sensor1; 
SFEVL53L1X sensor2(Wire, XSHUT2);
//////////// ToF ////////////

//////////// MOTORS ////////////
#define PIN1 0 // Left forward
#define PIN2 1 // Left backward

#define PIN3 7 // Right forward
#define PIN4 6 // Right backward

int lower = 45;
int upper = 255;

float base_speed = 50; // Base Motor Speed
float cal = 1.2; // Calibration Factor
//////////// MOTORS ////////////

//////////// IMU ////////////
#include "ICM_20948.h" 
#define SERIAL_PORT Serial

#define SPI_PORT SPI // Your desired SPI port.
#define CS_PIN 2     // Which pin you connect CS to. 

#define WIRE_PORT Wire // Your desired Wire port.      
#define AD0_VAL 1

#ifdef USE_SPI
ICM_20948_SPI myICM; // If using SPI create an ICM_20948_SPI object
#else
ICM_20948_I2C myICM; // Otherwise create an ICM_20948_I2C object
#endif
//////////// IMU ////////////

//////////// PID CONTROL ////////////
int PID_on = 0;
int max_size = 1000;
float stopDistance = 304.8; // one foot in mm
//////////// PID CONTROL ////////////

//////////// VARIABLES & ARRAYS ////////////
float time_PID[1000];
float distance_array[1000];
float ToF_distance[1000];
float motor_speedArray[1000];
float motorInput_Forward[1000];
float motorInput_Backward[1000];
float error_PID[1000];
float P[1000];
float I[1000];
float D[1000];
float PID_array[1000];
float PID = 0;
float time_1 = 0;
float time_2 = 0;
float dist_1 = 0;
float dist_2 = 0;
float K_p = 0;
float K_i = 0;
float K_d = 0;
float dt = 0;
int a = 0;
float prevTime = 0;
float prevError = 0;
float d_error = 0;
float dist = 0.0;
//////////// VARIABLES & ARRAYS ////////////

//////////// BLE UUIDs ////////////
#define BLE_UUID_TEST_SERVICE "e81d1925-217b-46e1-8718-ba10e53245d7"

#define BLE_UUID_RX_STRING "9750f60b-9c9c-4158-b620-02ec9521cd99"

#define BLE_UUID_TX_FLOAT "27616294-3063-4ecc-b60b-3470ddef2938"
#define BLE_UUID_TX_STRING "f235a225-6735-4d73-94cb-ee5dfce9ba83"
//////////// BLE UUIDs ////////////

//////////// Global Variables ////////////
BLEService testService(BLE_UUID_TEST_SERVICE);

BLECStringCharacteristic rx_characteristic_string(BLE_UUID_RX_STRING, BLEWrite, MAX_MSG_SIZE);

BLEFloatCharacteristic tx_characteristic_float(BLE_UUID_TX_FLOAT, BLERead | BLENotify);
BLECStringCharacteristic tx_characteristic_string(BLE_UUID_TX_STRING, BLERead | BLENotify, MAX_MSG_SIZE);

// RX
RobotCommand robot_cmd(":|");

// TX
EString tx_estring_value;
float tx_float_value = 0.0;

long interval = 500;
static long previousMillis = 0;
unsigned long currentMillis = 0;
//////////// Global Variables ////////////

enum CommandTypes
{
    PING,
    SEND_TWO_INTS,
    SEND_THREE_FLOATS,
    ECHO,
    DANCE,
    SET_VEL,
    GET_TIME_MILLIS,
    TIME_LOOP,
    TOF_DATA,
    START_PID,
    STOP_PID,
    SEND_PID_DATA,
    ORIENTATION,
    SEND_ORIENTATION_DATA,
};

void
handle_command()
{   
    // Set the command string from the characteristic value
    robot_cmd.set_cmd_string(rx_characteristic_string.value(),
                             rx_characteristic_string.valueLength());

    bool success;
    int cmd_type = -1;

    // Get robot command type (an integer)
    /* NOTE: THIS SHOULD ALWAYS BE CALLED BEFORE get_next_value()
     * since it uses strtok internally (refer RobotCommand.h and 
     * https://www.cplusplus.com/reference/cstring/strtok/)
     */
    success = robot_cmd.get_command_type(cmd_type);

    // Check if the last tokenization was successful and return if failed
    if (!success) {
        return;
    }

    // Handle the command type accordingly
    switch (cmd_type) {
        /*
         * Write "PONG" on the GATT characteristic BLE_UUID_TX_STRING
         */
        case PING:
            tx_estring_value.clear();
            tx_estring_value.append("PONG");
            tx_characteristic_string.writeValue(tx_estring_value.c_str());

            Serial.print("Sent back: ");
            Serial.println(tx_estring_value.c_str());

            break;
        /*
         * Extract two integers from the command string
         */
        case SEND_TWO_INTS:
            int int_a, int_b;

            // Extract the next value from the command string as an integer
            success = robot_cmd.get_next_value(int_a);
            if (!success)
                return;

            // Extract the next value from the command string as an integer
            success = robot_cmd.get_next_value(int_b);
            if (!success)
                return;

            Serial.print("Two Integers: ");
            Serial.print(int_a);
            Serial.print(", ");
            Serial.println(int_b);
            
            break;
        /*
         * Extract three floats from the command string
         */
        case SEND_THREE_FLOATS:
            float float_a, float_b, float_c;

            success = robot_cmd.get_next_value(float_a);
            if (!success)
              return;

            success = robot_cmd.get_next_value(float_b);
            if (!success)
              return;

            success = robot_cmd.get_next_value(float_c);
            if (!success)
              return;

            Serial.print("Three Floats: ");
            Serial.print(float_a);
            Serial.print(", ");
            Serial.print(float_b);
            Serial.print(", and ");
            Serial.print(float_c);
            break;
        /*
         * Add a prefix and postfix to the string value extracted from the command string
         */
        case ECHO:

            char char_arr[MAX_MSG_SIZE];

            // Extract the next value from the command string as a character array
            success = robot_cmd.get_next_value(char_arr);
            if (!success)
                return;

            tx_estring_value.clear();
            tx_estring_value.append("Robot says -> ");
            tx_estring_value.append(char_arr);
            tx_estring_value.append(" :)");
            tx_characteristic_string.writeValue(tx_estring_value.c_str());

            break;
        /*
         * DANCE
         */
        case DANCE:
            Serial.println("Look Ma, I'm Dancin'!");

            break;
        
        /*
         * SET_VEL
         */
        case SET_VEL:

            break;

        case GET_TIME_MILLIS:
          tx_estring_value.clear();
          tx_estring_value.append("T: ");
          tx_estring_value.append((float) millis());
          tx_characteristic_string.writeValue(tx_estring_value.c_str());

          Serial.print("Time was sent");

          break;

        case TIME_LOOP:
          
            break;

        case TOF_DATA:
          /*{
            digitalWrite(3, LOW); // turn ToF sensor 2 off   

            float dist_array1[1000];
            float dist_array2[1000];
            float t_tof[1000];

            Sensor2.setI2CAddress(0x30);

            if (Sensor2.begin() != 0)
            {
              Serial.println("sensor two flopped");
              while(1)
              ;
            }
            Serial.println("sensor two works now at 0x30");
            digitalWrite(3, HIGH); // turn ToF sensor 2 back on

            if (Sensor1.begin() != 0)
            {
              Serial.println("sensor one flopped");
              while(1)
              ;
            }
            Serial.println("both sensors work");

            double t_start = millis();

            for (int i = 0; i < 1000; i++) 
            {
              Sensor1.startRanging();
              Sensor2.startRanging(); 

              while (!Sensor1.checkForDataReady() && !Sensor2.checkForDataReady())  
              {

              dist_array1[i] = Sensor1.getDistance(); //Get the result of the measurement from the sensor (mm)
              dist_array2[i] = Sensor2.getDistance(); 

              Sensor1.clearInterrupt();
              Sensor2.clearInterrupt(); 
              Sensor1.stopRanging();
              Sensor2.stopRanging(); 

              double t_stop = millis(); 

              t_tof[i] = t_stop - t_start;
              }
            }
            //sending data
            for (int i = 0; i < 1000; i++) 
            {
              tx_estring_value.clear();
              tx_estring_value.append(t_tof[i]);
              tx_estring_value.append(",");
              tx_estring_value.append(dist_array1[i]);
              tx_estring_value.append(",");
              tx_estring_value.append(dist_array2[i]);
              tx_characteristic_string.writeValue(tx_estring_value.c_str());
            }*/
            break;
               
        case START_PID:
         {
          float target_dist, Kp, Ki, Kd;
          success = robot_cmd.get_next_value(target_dist);
            if (!success)
              return;
            float stop_dist = target_dist;
          success = robot_cmd.get_next_value(Kp);
            if (!success)
              return;
            K_p = Kp;
          success = robot_cmd.get_next_value(Ki);
            if (!success)
              return;
            K_i = Ki;
          success = robot_cmd.get_next_value(Kd);
            if (!success)
              return;
            K_d = Kd;

          PID_on = 1;

          Serial.print("PID_on Value:");
          Serial.print(PID_on);
          Serial.print(" Stop Distance: ");
          Serial.print(stop_dist);
          Serial.print(" Kp: ");
          Serial.print(K_p);
          Serial.print(" Ki: ");
          Serial.print(K_i);
          Serial.print(" Kd: ");
          Serial.println(K_d);
         }
          break;
         

        case STOP_PID:
        {
          PID_on = 0;
          stopMotors();
          
        }
          break;

        case SEND_PID_DATA:
        {
          for(int i = 0; i < 1000; i++) {
            tx_estring_value.clear();
            tx_estring_value.append(time_PID[i]);
            tx_estring_value.append(",");
            tx_estring_value.append(distance_array[i]);
            tx_estring_value.append(",");
            tx_estring_value.append(error_PID[i]);
            tx_estring_value.append(",");
            tx_estring_value.append(P[i]);
            tx_estring_value.append(",");
            tx_estring_value.append(I[i]);
            tx_estring_value.append(",");
            tx_estring_value.append(D[i]);
            tx_estring_value.append(",");
            tx_estring_value.append(PID_array[i]);
            tx_estring_value.append(",");
            tx_estring_value.append(motorInput_Forward[i]);
            tx_estring_value.append(",");
            tx_estring_value.append(motorInput_Backward[i]);
            tx_characteristic_string.writeValue(tx_estring_value.c_str());
          }
        }
          break;

          case ORIENTATION:
          {
            success = robot_cmd.get_next_value(ORIENTATION_on)
              if (!success)
                return; 
            success = robot_cmd.get_next_value(angle);
              if (!success)
                return;
            success = robot_cmd.get_next_value(Kp);
              if (!success)
                return;
            success = robot_cmd.get_next_value(Ki);
              if (!success)
                return;
            success = robot_cmd.get_next_value(Kd);
              if (!success)
                return;
            if (ORIENTATION_on == 1) {
              yaw[0] = 0;
              i = 1;
              end_time = millis();
              prev_time = millis();
            }
            if(ORIENTATION_on == 0) {
              Serial.print("Robot Powered Off");
            }
            Serial.print("ORIENTATION_on: ");
            Serial.print(ORIENTATION_on);
            Serial.print(" Angle: ");
            Serial.print(angle, 3);
            Serial.print(" Kp: ");
            Serial.print(Kp);
            Serial.print(" Ki: ");
            Serial.print(Ki);
            Serial.print(" Kd: ");
            Serial.print(Kd);
          }
          break;

        case SEND_ORIENTATION_DATA:
        {
          for (int i = 0; i < 1000; i++) {
            tx_estring_value.clear();
            tx_estring_value.append(time_PID_O[i]);
            tx_estring_value.append(";");
            tx_estring_value.append(yaw[i]);
            tx_estring_value.append(";");
            tx_estring_value.append(error_O[i]);
            tx_estring_value.append(";");
            tx_estring_value.append(P_O[i]);
            tx_estring_value.append(";");
            tx_estring_value.append(I_O[i]);
            tx_estring_value.append(";");
            tx_estring_value.append(D_O[i]);
            tx_estring_value.append(";");
            tx_estring_value.append(motorInput_Left[i]);
            tx_estring_value.append(";");
            tx_estring_value.append(motorInput_Right[i]);
            tx_estring_value.append(";");
          }
        }

        default:
            Serial.print("Invalid Command Type: ");
            Serial.println(cmd_type);
            break;
        
    }
}

void setup()
{
    digitalWrite(XSHUT1, LOW);
    Wire.begin();
    Serial.begin(115200);

    sensor1.setI2CAddress(0x30);
    if (sensor1.begin() != 0) {
      Serial.println("Sensor 1 failed to begin. Freezing...");
      while (1);
    }
    digitalWrite(XSHUT1, HIGH);

    if (sensor2.begin() != 0) {
      Serial.println("Sensor 2 failed to begin. Freezing...");
      while (1);
    }
    Serial.println("Both sensors online!");
  
    // Motor Setup
    pinMode(PIN1, OUTPUT);
    pinMode(PIN2, OUTPUT);
    pinMode(PIN3, OUTPUT);
    pinMode(PIN4, OUTPUT);

    BLE.begin();

    // Set advertised local name and service
    BLE.setDeviceName("Artemis BLE");
    BLE.setLocalName("Artemis BLE");
    BLE.setAdvertisedService(testService);

    // Add BLE characteristics
    testService.addCharacteristic(tx_characteristic_float);
    testService.addCharacteristic(tx_characteristic_string);
    testService.addCharacteristic(rx_characteristic_string);

    // Add BLE service
    BLE.addService(testService);

    // Initial values for characteristics
    // Set initial values to prevent errors when reading for the first time on central devices
    tx_characteristic_float.writeValue(0.0);

    /*
     * An example using the EString
     */
    // Clear the contents of the EString before using it
    tx_estring_value.clear();

    // Append the string literal "[->"
    tx_estring_value.append("[->");

    // Append the float value
    tx_estring_value.append(9.0);

    // Append the string literal "<-]"
    tx_estring_value.append("<-]");

    // Write the value to the characteristic
    tx_characteristic_string.writeValue(tx_estring_value.c_str());

    // Output MAC Address
    Serial.print("Advertising BLE with MAC: ");
    Serial.println(BLE.address());

    BLE.advertise();

    // Motor Setup
  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);
  pinMode(PIN3, OUTPUT);
  pinMode(PIN4, OUTPUT);

}

void
write_data()
{
    currentMillis = millis();
    if (currentMillis - previousMillis > interval) {

        tx_float_value = tx_float_value + 0.5;
        tx_characteristic_float.writeValue(tx_float_value);

        if (tx_float_value > 100000) {
            tx_float_value = 0;
            
        }

        previousMillis = currentMillis;
    }
}

void
read_data()
{
    // Query if the characteristic value has been written by another BLE device
    if (rx_characteristic_string.written()) {
        handle_command();
    }
}

// Function to Drive Forward w/o PID Control
void driveForward_noPID(float forward_speed) {
  analogWrite(PIN1, forward_speed);
  analogWrite(PIN2, 0);
  analogWrite(PIN3, forward_speed*cal);
  analogWrite(PIN4, 0);
  }

// Function to Drive Backward w/o PID Control
void driveBackward_noPID(float back_speed) {
  analogWrite(PIN1, 0);
  analogWrite(PIN2, back_speed);
  analogWrite(PIN3, 0);
  analogWrite(PIN4, back_speed*cal);
  }

// Function to Drive Forward w/ PID Control
void driveForward_PID(float PID, int a) {

  if (PID >= lower && PID <= upper) { // if PID is within bounds
    analogWrite(PIN1, PID);
    analogWrite(PIN2, 0);
    analogWrite(PIN3, PID*cal);
    analogWrite(PIN4, 0);
    motorInput_Forward[a] = PID; // collects PID data
  }
  else if(PID >= 0.1 && PID < lower) { // if PID is less than 45
    analogWrite(PIN1, lower);
    analogWrite(PIN2, 0);
    analogWrite(PIN3, lower*cal);
    analogWrite(PIN4, 0);
    motorInput_Forward[a] = lower;
  }
  else if(PID > upper) { // if PID is larger than 255
    analogWrite(PIN1, upper);
    analogWrite(PIN2, 0);
    analogWrite(PIN3, upper*cal);
    analogWrite(PIN4, 0);
    motorInput_Forward[a] = upper;
  }
  else {
    analogWrite(PIN1, 0);
    analogWrite(PIN2, 0);
    analogWrite(PIN3, 0);
    analogWrite(PIN4, 0);
    motorInput_Forward[a] = 0;
  }
}

// Function to Drive Backward w/ PID Control
void driveBackward_PID(float PID, int a) {
  if (abs(PID) >= lower && abs(PID) <= upper) {
    analogWrite(PIN1, 0);
    analogWrite(PIN2, PID);
    analogWrite(PIN3, 0);
    analogWrite(PIN4, PID);
    motorInput_Backward[a] = abs(PID);
  }
  else if(abs(PID) >= 0.1 && abs(PID) < lower) {
    analogWrite(PIN1, 0);
    analogWrite(PIN2, lower);
    analogWrite(PIN3, 0);
    analogWrite(PIN4, lower*cal);
    motorInput_Backward[a] = lower;
  }
  else if (abs(PID) > upper) {
    analogWrite(PIN1, 0);
    analogWrite(PIN2, upper);
    analogWrite(PIN3, 0);
    analogWrite(PIN4, upper);
    motorInput_Backward[a] = upper;
  }
  else {
    analogWrite(PIN1, 0);
    analogWrite(PIN2, 0);
    analogWrite(PIN3, 0);
    analogWrite(PIN4, 0);
    motorInput_Backward[a] = 0;
  }
}

// Function to Stop Car
void stopMotors() {
  analogWrite(PIN1, 0);
  analogWrite(PIN2, 0);
  analogWrite(PIN3, 0);
  analogWrite(PIN4, 0);
  }

void loop()
{
    // Listen for connections
    BLEDevice central = BLE.central();

    // If a central is connected to the peripheral
    if (central) {
        Serial.print("Connected to: ");
        Serial.println(central.address());

        // While central is connected
        while (central.connected()) {
            // Send data
            write_data();

            sensor1.startRanging();
            delay(1);
            // Turns Robot on with PID Control
            if (PID_on == 1) {
              time_PID[a] = millis();

              if (sensor1.checkForDataReady()) {
                delay(1);

                dist = sensor1.getDistance(); // Collect ToF Distance Data
                //Serial.println(dist);
                distance_array[a] = dist; 
                //Serial.print("Array: ");
                //Serial.println(distance_array[a]);
                time_2 = time_1;
                time_1 = (float)millis();
                dist_2 = dist_1;
                dist_1 = dist;
              }
              else if(!sensor1.checkForDataReady()) {
                distance_array[a] = dist_1 + ((float)millis() - time_1)*((dist_2 - dist_1)/(time_2 - time_1));   
              }
              ToF_distance[a] = dist_1;

              //////////// PID CONTROL ////////////
              dt = millis() - prevTime;
              error_PID[a] = dist - stopDistance;
              Serial.println(error_PID[a]);

              d_error = (error_PID[a] - prevError) /dt;
              PID = K_p * error_PID[a] + K_d*d_error;
              P[a] = K_p * error_PID[a];
              D[a] = K_d * d_error;
              PID_array[a] = PID;
              prevError = error_PID[a];
              prevTime = millis();
              //////////// PID CONTROL ////////////

              // Stop Robot at stop distance
              if(error_PID[a] >= -5 && error_PID[a] <=5) {
                analogWrite(PIN1, 0);
                analogWrite(PIN2, 0);
                analogWrite(PIN3, 0);
                analogWrite(PIN4, 0);
                Serial.println("Finished");
              }
              else if (PID > 0) {
                driveForward_PID(PID, a);
              }

              else if (PID < 0) {
                driveBackward_PID(PID, a);
              }

              a++;

              // Stop robot if data size exceeds array size
              if (a > 1000) {
                analogWrite(PIN1, 0);
                analogWrite(PIN2, 0);
                analogWrite(PIN3, 0);
                analogWrite(PIN4, 0);
                Serial.println("Max Array Size Reached");
              }
            }

            // Turn robot off
            if(PID_on == 0) {
              analogWrite(PIN1, 0);
              analogWrite(PIN2, 0);
              analogWrite(PIN3, 0);
              analogWrite(PIN4, 0);
              }

            // Read data
            read_data();
          }

          // Stop robot if disconnected
          analogWrite(PIN1, 0);
          analogWrite(PIN2, 0);
          analogWrite(PIN3, 0);
          analogWrite(PIN4, 0);
          Serial.println("Robot Disconnected");
  }
}
