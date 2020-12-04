#define analogMQ135 A1      // Signal 
#define ledPin D7         // Device internal LED      
int MQ135sensorValue = 0;   // value read from the sensor

void setup() {
    Serial.begin(9600);
    Particle.publish("MQ-135", "Gas Sensor started");

    pinMode(analogMQ135, INPUT);
    pinMode(ledPin, OUTPUT);
}

void loop() { 
    // turn the heater fully on
    analogWrite(analogMQ135, HIGH); // HIGH = 255
    // heat for 1 min
    delay(60000);
    // now reducing the heating power: turn the heater to approx 1,4V
    analogWrite(analogMQ135, 71.4); // 255x1400/5000
    // heat for 90 sec
    delay(90000);
    
    // CO2 via MQ135: we need to read the sensor at 5V, but must not let it heat up. So hurry!
    analogWrite(analogMQ135, HIGH); 
    delay(50); // Getting an analog read apparently takes 100uSec
    MQ135sensorValue = analogRead(analogMQ135);
    
    Particle.publish("MQ-135", "PPM:");
    Particle.publish("MQ-135", String(MQ135sensorValue), PRIVATE);
}
