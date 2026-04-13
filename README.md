# Adaptive Battery Health Monitoring System

## Overview
This project implements an adaptive battery health monitoring system using ESP32 to measure voltage, current, and temperature, and perform real-time analysis of battery performance.

---

## Components Used
- ESP32
- INA219 (Current Sensor)
- Temperature Sensor
- Voltage Divider Circuit

---

## Features
- Real-time monitoring of battery parameters
- Voltage, current, and temperature sensing
- Serial output for live data visualization
- Basic battery health analysis

---

## Circuit Diagram
![Circuit](images/circuit_diagram.jpeg)

---

## Output
![Output](images/output.png)

---

## System Architecture
The system reads:
- Voltage using a voltage divider
- Current using INA219 sensor
- Temperature using a sensor module

All data is processed by ESP32 and displayed via serial monitor for analysis.

---

## What I Learned
- Embedded C programming
- Sensor interfacing with ESP32
- Real-time data acquisition
- Debugging hardware and software issues

---

## Future Improvements
- Add LCD/OLED display
- Implement battery health prediction algorithm
- Integrate IoT dashboard for remote monitoring
