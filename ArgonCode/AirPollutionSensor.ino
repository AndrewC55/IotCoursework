// This #include statement was automatically added by the Particle IDE.
#include <TinyGPS.h>
#define analogMQ7 A1 // Signal 
#define ledPin D7 // Device internal LED      
TinyGPS gps; // declaring gps variable
int MQ7sensorValue = 0; // value read from the sensor
char gpsLocation[64]; // value for displaying GPS location
char serialRead; // global variable that will be used to 

void setup() {
    Serial1.begin(9600); // start serial in order to read via GPS
    
    Particle.variable("GPS", gpsLocation); // decalre gps variable
    Particle.variable("MQ-7", MQ7sensorValue); // decalre gas variable
    
    pinMode(analogMQ7, INPUT); // declare the pinMode for the MQ-7 sensor
}

void getGpsData() { // function for collecting data from the gps sensor
    bool gpsReturnsData = false; // bool value which will be used to determine if the gps is working correclty
    
    for (unsigned long start = millis(); millis() - start < 1000;) { // for loop which 
        while (Serial1.available()) { // while loop which will run as long as gps module is available
            serialRead = Serial1.read();  // read from the gps module
            
            if (gps.encode(serialRead)) { // if read is successful
                gpsReturnsData = true; // change boolean value to true
            }
        }
    }

    if (gpsReturnsData) { // if bool = true meaning gps module is available and reading data
        float lat, lon; // two floats for measuring latitdue and longitude of gps location 
        unsigned long age; // timescale

        gps.f_get_position(&lat, &lon, &age); // function included in the TinyGPS library for getting the gps location
        
        sprintf(gpsLocation, "%.6f,%.6f", (lat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : lat), (lon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : lon)); // sprintf gpsLocation into a readable format
    } else {
        sprintf(gpsLocation, "0.0,0.0"); // bool = false meaning and gps module was unavaliable AND/OR not reading data
    }
}

void loop() { // this function will loop as long as the Argon had a power source
    analogWrite(analogMQ7, HIGH); // turn the heater fully on HIGH = 255
    
    delay(60000); // heat for 1 min exactly
    
    analogWrite(analogMQ7, 71.4); // now reducing the heating power: turn the heater to approx 1,4V (255x1400/5000)
    
    delay(90000); // heat for a further 90 seconds
    
    getGpsData(); // call getGpsData function to assign the gpsLocation variable with a value
    Particle.publish("GPS location", gpsLocation, PRIVATE); // publish the gps location
    
    analogWrite(analogMQ7, HIGH); // we need to read the sensor at 5V as the MQ-7 is a 5V device
    
    delay(50); // allowing time for the analof to properly read
    
    MQ7sensorValue = analogRead(analogMQ7); // read the MQ-7 sensor data through the INPUT, in this case the input is the Argon A1 pin
    Particle.publish("C02 measurements", String(MQ7sensorValue), PRIVATE);
}
