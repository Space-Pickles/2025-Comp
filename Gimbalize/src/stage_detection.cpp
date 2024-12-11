#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP085.h>
#include <stage_detection.h>

// Define thresholds
launch_accel_threshold = 75; // m/s^2
coast_accel_threshold = 40; // m/s^2
apogee_altitude_threshold = 240.792; // m


// Initialize sensors
Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;

void initializeSensors() {
  Wire.begin();

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  if (!bmp.begin()) {
    Serial.println("Failed to find BMP180 sensor");
    while (1) {
      delay(10);
    }
  }
  Serial.println("BMP085 Found!");
}

// Function to read sensor data
void read_sensor_data(float &accel_x, float &accel_y, float &accel_z, float &altitude) {
    // Read acceleration data from MPU6050
    mpu.getAcceleration(&accel_x, &accel_y, &accel_z);

    // Read altitude data from BMP085
    const altitude = bmp.readAltitude();
}

// Main loop
void detectFlightPhase(float accel_x, float accel_y, float accel_z, float altitude, float current_pressure, float last_pressure, float current_phase){

    read_sensor_data(accel_x, accel_y, accel_z, altitude);
    current_pressure = 1013;
    last_pressure = 1013;
    current_phase = "ground";


    
    // Detect flight phases based on sensor data and thresholds

    switch(true){
        case (current_phase == "ground" && accel_z >= launch_accel_threshold):
            Serial.println("Launch detected!");
            FlightPhase current_stage = LAUNCH;
            current_phase = "powered ascent"
       case (current_phase == "powered_acsent" && accel_z > coast_accel_threshold && current_pressure-last_pressure < 1 ):
            Serial.println("Coasting Phase detected!");
            FlightPhase current_stage = POWERED_ASCENT;
            current_phase = "coasting";
       case (current_phase == "coasting" && abs(altitude - apogee_altitude_threshold)<10 && accel_z < 5):
            Serial.println("Apogee detected!");
            FlightPhase current_stage = COASTING;
            current_phase = "apogee";
       case (current_phase == "apogee" && 0 > accel_z ):
            Serial.println("Descent detected!");
            current_phase = "descent";
            FlightPhase current_stage = DESCENT;
       case (current_phase == "descent" && abs(0 - accel_z) < 2) : // <2 is accounting for hallucination data
            Serial.println("Landing detected!");
            current_phase = "landing";
            FlightPhase current_stage = LANDING;
       default:
            Serial.println("PROBLEM DETECTED!");
        }
    

    lastPressure = currentPressure;
    currentPressure = bmp.readPressure()/100

    // Delay for sensor readings
    delay(100); 
}
