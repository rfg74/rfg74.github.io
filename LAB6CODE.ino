#include "BLECStringCharacteristic.h"
#include "EString.h"
#include "RobotCommand.h"
#include <ArduinoBLE.h>

//////////// MOTORS ////////////
#define PIN1 0 // Left forward
#define PIN2 1 // Left backward

#define PIN3 7 // Right forward
#define PIN4 6 // Right backward

int lower = 120;
int upper = 255;
//////////// MOTORS ////////////

//////////// IMU ////////////
#include "ICM_20948.h" 
#define SERIAL_PORT Serial

#define SPI_PORT SPI // Your desired SPI port.
#define CS_PIN 2     // Which pin you connect CS to. 

#define WIRE_PORT Wire // Your desired Wire port.      
#define AD0_VAL 1

ICM_20948_I2C myICM; // Otherwise create an ICM_20948_I2C object
//////////// IMU ////////////

//////////// PID CONTROL ////////////
int ORIENTATION_on = 0;
int max_size = 1000;
//////////// PID CONTROL ////////////

//////////// VARIABLES & ARRAYS ////////////
float time_PID_O[1000];
float yaw[1000];
float P_O[1000];
float I_O[1000];
float D_O[1000];
float error_PID_O[1000];
float motorInput_Left[1000];
float motorInput_Right[1000];
float setAngle[1000];
float end_time = 0;
int a = 0;
float prev_error = 0;
float prev_time = 0;
float gyrZ = 0;
float dt_gyr = 0;
float angle, Kp, Ki, Kd;
int y = 0;
float PID_O = 0;
float yaw_gyr = 0;
float PID_deltaT = 0;
float target_angle = 0;

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

long interval = 1000;
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
         
            break;
               
        case START_PID:
         
          break;
         

        case STOP_PID:
        
          break;

        case SEND_PID_DATA:
        {
        }
          break;

          case ORIENTATION:
          {
            float angle;
            success = robot_cmd.get_next_value(ORIENTATION_on);
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
              int y = 1;
              end_time = millis();
              prev_time = millis();
              target_angle = angle;
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
              tx_estring_value.append(error_PID_O[i]);
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
              tx_estring_value.append(setAngle[i]);
              tx_characteristic_string.writeValue(tx_estring_value.c_str());
            }  
          }
          break; 

          case MAX_SPEED: 
          {
            int DRIVE_on, max;
            float start_time, dt, distance;
            dt = 0;

            success = robot_cmd.get_next_value(DRIVE_on);
            if (!success)
              return;
            success = robot_cmd.get_next_value(max);
            if (!success)
              return;
            
            start_time = millis();

            if (DRIVE_on == 1) {
              sensor1.startRanging();

              while (dt < maxTime) {
                dt = millis() - start;
                driveForward(255); // change speed to account for calibration

                if (sensor1.checkForDataReady()) {
                  distance = distance1.getDistance();
                  time_PID[PID_index] = (millis() - start)/1000;
                  distance_array[PID_index] = distance;

                  distance1.clearInterrupt();
                  PID_index++;
                }
              }
              stopMotors();
              delay(2000);

              sensor1.stopRanging();
            }
            else {
              stopMotors();
              return;
            }
          }
          break;

          default:
            Serial.print("Invalid Command Type: ");
            Serial.println(cmd_type);
            break;
        
    }
    }

void setup () {

    Serial.begin(115200);

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

     //////////// D M P ////////////
    // Initialize the DMP
    bool success = true;
    success &= (myICM.initializeDMP() == ICM_20948_Stat_Ok);

    // Enable the DMP Game Rotation Vector sensor
    success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_GAME_ROTATION_VECTOR) == ICM_20948_Stat_Ok);

    // Set the DMP output data rate (ODR): value = (DMP running rate / ODR ) - 1
    // E.g. for a 5Hz ODR rate when DMP is running at 55Hz, value = (55/5) - 1 = 10.
    success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Quat6, 4) == ICM_20948_Stat_Ok); // Set to the maximum

    // Enable the FIFO queue
    success &= (myICM.enableFIFO() == ICM_20948_Stat_Ok);

    // Enable the DMP
    success &= (myICM.enableDMP() == ICM_20948_Stat_Ok);

    // Reset DMP
    success &= (myICM.resetDMP() == ICM_20948_Stat_Ok);

    // Reset FIFO
    success &= (myICM.resetFIFO() == ICM_20948_Stat_Ok);

    // Check success
    if (!success) {
        Serial.println("Enabling DMP failed!");
        while (1) {
            // Freeze
        }
    }
    //////////// D M P ////////////

    // Motor Setup
    pinMode(PIN1, OUTPUT); // left forward
    pinMode(PIN2, OUTPUT); // left backward
    pinMode(PIN3, OUTPUT); // right forward
    pinMode(PIN4, OUTPUT); // right backward

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

int PID_Orientation(float Kp, float Ki, float Kd, float angle) {
  double current_time = millis();
  PID_deltaT = current_time - prev_time; 
  prev_time = current_time;

  icm_20948_DMP_data_t data;
  myICM.readDMPdataFromFIFO(&data);

  // Is valid data available?
  if ((myICM.status == ICM_20948_Stat_Ok) || (myICM.status == ICM_20948_Stat_FIFOMoreDataAvail)) {
      // We have asked for GRV data so we should receive Quat6
      if ((data.header & DMP_header_bitmap_Quat6) > 0) {
          double q1 = ((double)data.Quat6.Data.Q1) / 1073741824.0; // Convert to double. Divide by 2^30
          double q2 = ((double)data.Quat6.Data.Q2) / 1073741824.0; // Convert to double. Divide by 2^30
          double q3 = ((double)data.Quat6.Data.Q3) / 1073741824.0; // Convert to double. Divide by 2^30

          double q0 = sqrt(1.0 - ((q1 * q1) + (q2 * q2) + (q3 * q3)));

          double qw = q0; // See issue #145 - thank you @Gord1
          double qx = q2;
          double qy = q1;
          double qz = -q3;

          // yaw calculation
          double t3 = +2.0 * (qw * qz + qx * qy);
          double t4 = +1.0 - 2.0 * (qy * qy + qz * qz);
          double yaw_gyr = atan2(t3, t4) * 180.0 / PI; 
          yaw[y] = yaw_gyr;
      }
  }
  float err = yaw_gyr - target_angle;
  error_PID_O[a] = err;
  float err_d = (err - prev_error) / PID_deltaT;
  float err_i = err_i + (err * PID_deltaT);

  PID_O = (int)(Kp*err + Ki * err_i + Kd *err_d);

  if (PID_O > 0) { // Right Forward
    if (PID_O >= lower && PID_O <= upper) {
      analogWrite(PIN1, 0);
      analogWrite(PIN2, PID_O);
      analogWrite(PIN3, PID_O);
      analogWrite(PIN4, 0);
    }
    else if (PID_O >= 0 && PID_O < lower) {
      analogWrite(PIN1, 0);
      analogWrite(PIN2, lower);
      analogWrite(PIN3, lower);
      analogWrite(PIN4, 0);
    }
    else if (PID_O > upper) {
      analogWrite(PIN1, 0);
      analogWrite(PIN2, 255);
      analogWrite(PIN3, upper);
      analogWrite(PIN4, 0);
    }
  }

  if (PID_O < 0) { 
    if (abs(PID_O) >= lower && abs(PID_O) <= upper) {   
      analogWrite(PIN1, abs(PID_O));
      analogWrite(PIN2, 0);
      analogWrite(PIN3, 0);
      analogWrite(PIN4, abs(PID_O));
    }
    else if (abs(PID_O) >= 0 && abs(PID_O) < lower) {
      analogWrite(PIN1, lower);
      analogWrite(PIN2, 0);
      analogWrite(PIN3, 0);
      analogWrite(PIN4, lower);
    }
    else if (abs(PID_O) > upper) {
      analogWrite(PIN1, upper);
      analogWrite(PIN2, 0);
      analogWrite(PIN3, 0);
      analogWrite(PIN4, upper);
    }
  }
}

// Function to Stop Car
void stopMotors() {
  analogWrite(PIN1, 0);
  analogWrite(PIN2, 0);
  analogWrite(PIN3, 0);
  analogWrite(PIN4, 0);
  }

// Function to Drive Forward
void driveForward(int input) {
  analogWrite(PIN1, input);
  analogWrite(PIN2, 0);
  analogWrite(PIN3, input);
  analogWrite(PIN4, 0);
}

void loop()
{
    // Listen for connections
    BLEDevice central = BLE.central();
    icm_20948_DMP_data_t data;
    myICM.readDMPdataFromFIFO(&data);

    // If a central is connected to the peripheral
    if (central) {
        Serial.print("Connected to: ");
        Serial.println(central.address());

        // While central is connected
        while (central.connected()) {
            // Send data
            write_data();
            // Read data
            read_data();

            // Turns Robot on with PID Control
            if (ORIENTATION_on == 1) {
              PID_Orientation(Kp, Ki, Kd, angle);
              y++;
              a++;
            }

            if (a > 1000) {
              analogWrite(PIN1, 0);
              analogWrite(PIN2, 0);
              analogWrite(PIN3, 0);
              analogWrite(PIN4, 0);
              Serial.println("Max Array Size Reached");
              ORIENTATION_on = 0;
              a = 0;
              y = 0;
              }
            }
              
            // Turn robot off
            if(ORIENTATION_on == 0) {
              analogWrite(PIN1, 0);
              analogWrite(PIN2, 0);
              analogWrite(PIN3, 0);
              analogWrite(PIN4, 0);
              }            
          }

          // Stop robot if disconnected
          analogWrite(PIN1, 0);
          analogWrite(PIN2, 0);
          analogWrite(PIN3, 0);
          analogWrite(PIN4, 0);
          Serial.println("Robot Disconnected");
  }
