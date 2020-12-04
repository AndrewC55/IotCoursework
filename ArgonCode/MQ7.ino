#define analogMQ7 A1      // Signal 
#define ledPin D7         // Device internal LED      
int MQ7sensorValue = 0;   // value read from the sensor

void setup() {
    Serial.begin(9600);
    Particle.publish("MQ-7", "Gas Sensor started");

    pinMode(analogMQ7, INPUT);
    pinMode(ledPin, OUTPUT);
}

void loop() { 
    // turn the heater fully on
    analogWrite(analogMQ7, HIGH); // HIGH = 255
    // heat for 1 min
    delay(60000);
    // now reducing the heating power: turn the heater to approx 1,4V
    analogWrite(analogMQ7, 71.4); // 255x1400/5000
    // heat for 90 sec
    delay(90000);
    
    // CO2 via MQ7: we need to read the sensor at 5V, but must not let it heat up. So hurry!
    analogWrite(analogMQ7, HIGH); 
    delay(50); // Getting an analog read apparently takes 100uSec
    MQ7sensorValue = analogRead(analogMQ7);
    
    Particle.publish("MQ-7", "PPM:");
    Particle.publish("MQ-7", String(MQ7sensorValue), PRIVATE);
  
    if (MQ7sensorValue <= 200) {
        Particle.publish("MQ-7", "Air-Quality: CO perfect");
    } else if ((MQ7sensorValue > 200) || (MQ7sensorValue <= 400)) {
        Particle.publish("MQ-7", "Air-Quality: CO normal");
    } else if ((MQ7sensorValue > 600) || (MQ7sensorValue <= 1000)) {
        Particle.publish("MQ-7", "Air-Quality: CO high");
    } else if (MQ7sensorValue > 1000) {
        digitalWrite(ledPin, HIGH); // optical information in case of emergency
        Particle.publish("MQ-7", "Air-Quality: CO very high");
        delay(3000);
        digitalWrite(ledPin, LOW);
    } else {
        Particle.publish("MQ-7", "cant read any value - check the sensor!");
    }
}
