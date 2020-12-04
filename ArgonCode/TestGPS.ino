#include <TinyGPS.h>

TinyGPS gps;
char szInfo[64];
char c;

void setup(){
    Serial1.begin(9600);
    Particle.variable("GPS", szInfo);
}

void loop(){
    bool isValidGPS = false;
    
    for (unsigned long start = millis(); millis() - start < 1000;){
        while (Serial1.available()){
            c = Serial1.read();
            
            if (gps.encode(c)) {
                Particle.publish("Blah", "Blah");
                isValidGPS = true;
            }
        }
    }

    if (isValidGPS) {
        float lat, lon;
        unsigned long age;
    
        gps.f_get_position(&lat, &lon, &age);
        
        sprintf(szInfo, "%.6f,%.6f", (lat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : lat), (lon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : lon));
        Particle.publish("gpsloc", szInfo);
    }
    else{
        sprintf(szInfo, "0.0,0.0");
    }
    
    Particle.publish("gpsloc", szInfo);
    Serial.println(szInfo);
}
