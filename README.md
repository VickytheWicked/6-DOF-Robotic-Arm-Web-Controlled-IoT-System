# 🤖 6-DOF Robotic Arm – Web-Controlled Embedded System

## Overview
A web-controlled robotic arm built using XIAO ESP32-C3 microcontroller. The device hosts an onboard web server allowing real-time joint control via browser-based sliders.

---

## Tech Stack
- XIAO ESP32-C3
- Embedded C / Arduino Framework
- HTML + JavaScript
- PWM Servo Control

---

## Architecture
Browser → HTTP Request → ESP32 Web Server → PWM Signal → Servo Motors

---

## Features
- Onboard embedded web server
- Real-time joint angle control
- Multi-servo PWM coordination
- Calibrated movement logic

---

## Setup
1. Flash firmware to ESP32
2. Connect device to Wi-Fi
3. Access device IP in browser
4. Control arm using sliders

---

## Future Improvements
- Inverse kinematics integration
- WebSocket-based smoother control
- Computer vision integration
