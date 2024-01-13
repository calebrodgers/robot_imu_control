#include <Arduino_LSM9DS1.h> // Available from Library Manager
#include <MadgwickAHRS.h> // ZIP Included, Source: https://www.arduinolibraries.info/libraries/madgwick, Credit: Sebastian Madgwick. An efficient orientation filter for inertial and inertial/magnetic sensor arrays. April 30, 2010. http://www.x-io.co.uk/open-source-imu-and-ahrs-algorithms/
#include <Math.h>

float pitchFilteredOld;
float rollFilteredOld;
Madgwick filter;
const float sensorRate = 104.00;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  if(!IMU.begin())  {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  filter.begin(sensorRate);
  Serial.println("Setup complete!");
}  
void loop() {
  float xAcc, yAcc, zAcc;
  float xGyro, yGyro, zGyro;
  float roll, pitch, heading;
  if(IMU.accelerationAvailable() && IMU.gyroscopeAvailable()){
    IMU.readAcceleration(xAcc, yAcc, zAcc);
    IMU.readGyroscope(xGyro, yGyro, zGyro); 
    filter.updateIMU(xGyro, yGyro, zGyro, xAcc, yAcc, zAcc);
    pitch = -filter.getPitch();
    roll = filter.getRoll();
    float pitchFiltered = 0.1 * pitch + 0.9 * pitchFilteredOld; // low pass filter
    float rollFiltered = 0.1 * roll + 0.9 * rollFilteredOld; // low pass filter
    
    Serial.print(pitchFiltered);
    Serial.print(",");
    Serial.print(rollFiltered);
    Serial.println();

    pitchFilteredOld = pitchFiltered;
    rollFilteredOld = rollFiltered;
  }
}