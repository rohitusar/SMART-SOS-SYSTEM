#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#define GSM_TX_PIN 2
#define GSM_RX_PIN 3
#define DHT_PIN 4
#define FLAME_SENSOR_PIN 5
#define BUZZER_PIN 6
#define MQ5_SENSOR_PIN A0
SoftwareSerial gsmSerial(GSM_TX_PIN, GSM_RX_PIN);
DHT dht(DHT_PIN, DHT22);

void setup()
 {
  gsmSerial.begin(9600);
  Serial.begin(9600);
  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  dht.begin();
}
void loop()
 {
  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  // Read flame sensor
  int flameValue = digitalRead(FLAME_SENSOR_PIN);

  // Read MQ5 sensor
  int mq5Value = analogRead(MQ5_SENSOR_PIN);

  // Check flame sensor
  if (flameValue == HIGH) 
  {
    digitalWrite(BUZZER_PIN, HIGH);
    sendSMS("Fire detected!");
  }
   else
    {
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Check MQ5 sensor for gas detection
  if (mq5Value > 100)
   {
    sendSMS("Gas leak detected!");
  }

  // Send temperature and humidity readings via SMS
  if (mq5Value != NAN && flameValue != NAN) 
  {
    String message = "Gas sensor: " + String(mq5Value) + ", Flame Sensor: " + String(flameValue);
    sendSMS(message);
    Serial.println(message);
  }

  delay(100); // Delay for 10 seconds before reading sensors again
}

void sendSMS(String message)
 {
  gsmSerial.println("AT+CMGF=1"); // Set SMS mode to text
  delay(100);
  gsmSerial.println("AT+CMGS=\"+91000*****\""); // Replace with your destination number
  delay(100);
  gsmSerial.println(message);
  delay(100);
  gsmSerial.write(0x1A);
  delay(1000); // Wait for SMS to be sent
}