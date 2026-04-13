#include <Wire.h>
#include <Adafruit_INA219.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// INA219
Adafruit_INA219 ina219;

// DS18B20
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Voltage pin
int voltagePin = 34;

// 🔥 Logic variables
int deepDischargeCount = 0;
bool deepDischargeFlag = false;

// 🔥 Overcharge variables
unsigned long overchargeTime = 0;
unsigned long startTime = 0;
bool overchargeActive = false;

// 🔥 Temperature stress variables
unsigned long tempStressTime = 0;
unsigned long tempStartTime = 0;
bool tempStressActive = false;

// 🔥 Battery health variables
float health = 100.0;
float lifeYears = 10.0;
String condition;

void setup() {
  Serial.begin(115200);

  Wire.begin();

  if (!ina219.begin()) {
    Serial.println("INA219 not found");
    while (1);
  }

  sensors.begin();

  Serial.println("System Started...");
}

void loop() {

  // 🔋 Voltage
  int raw = analogRead(voltagePin);
  float v_out = (raw / 4095.0) * 3.3;

  float busVoltage = ina219.getBusVoltage_V();
  float current = ina219.getCurrent_mA();

  float multiplier = 0;
  if (v_out > 0.01) {
    multiplier = busVoltage / v_out;
  }

  float batteryVoltage = v_out * multiplier;

  // 🌡 Temperature
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  // 🔁 Status
  String status;
  if (current > 10) status = "Charging ⚡";
  else if (current < -10) status = "Discharging 🔋";
  else status = "Idle";

  // 🔥 DEEP DISCHARGE LOGIC
  if (batteryVoltage < 3.0 && !deepDischargeFlag) {
    deepDischargeCount++;
    deepDischargeFlag = true;
  }

  if (batteryVoltage > 3.2) {
    deepDischargeFlag = false;
  }

  // 🔥 OVERCHARGE LOGIC
  if (batteryVoltage > 4.1) {
    if (!overchargeActive) {
      startTime = millis();
      overchargeActive = true;
    }
    overchargeTime = (millis() - startTime) / 1000;
  } 
  else {
    overchargeActive = false;
  }

  // 🔥 TEMPERATURE STRESS LOGIC
  if (tempC > 40) {
    if (!tempStressActive) {
      tempStartTime = millis();
      tempStressActive = true;
    }
    tempStressTime = (millis() - tempStartTime) / 1000;
  } 
  else {
    tempStressActive = false;
  }

  // 🔥 BATTERY HEALTH CALCULATION
  health = 100;

  // penalties
  health -= deepDischargeCount * 1.5;
  health -= overchargeTime * 0.05;
  health -= tempStressTime * 0.05;

  if (health < 0) health = 0;

  // life prediction
  lifeYears = (health / 100.0) * 10;

  // condition
  if (health > 80) condition = "Good";
  else if (health > 50) condition = "Moderate";
  else condition = "Poor";

  // 📊 OUTPUT
  Serial.println("===== BATTERY SYSTEM =====");

  Serial.print("Voltage: ");
  Serial.print(batteryVoltage);
  Serial.println(" V");

  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" mA");

  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" °C");

  Serial.print("Status: ");
  Serial.println(status);

  Serial.println("---- USAGE STATS ----");

  Serial.print("Deep Discharge Count: ");
  Serial.println(deepDischargeCount);

  Serial.print("Overcharge Time: ");
  Serial.print(overchargeTime);
  Serial.println(" sec");

  Serial.print("Temp Stress Time: ");
  Serial.print(tempStressTime);
  Serial.println(" sec");

  Serial.println("---- BATTERY HEALTH ----");

  Serial.print("Health: ");
  Serial.print(health);
  Serial.println(" %");

  Serial.print("Estimated Life: ");
  Serial.print(lifeYears);
  Serial.println(" years");

  Serial.print("Condition: ");
  Serial.println(condition);

  Serial.println("==========================\n");

  delay(2000);
}