# 🤖 6-DOF Robotic Arm – Web-Controlled Embedded System

![WhatsApp Image 2026-02-22 at 10 19 58 PM](https://github.com/user-attachments/assets/89a29c38-588b-4931-83fe-f27e53e6d851)

---

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

## Simple HTML page to controll
<img width="385" height="443" alt="image" src="https://github.com/user-attachments/assets/2c202010-cd68-431b-bf41-393632eb2007" />

---

## Demonstration

https://github.com/user-attachments/assets/3ca6224a-dc82-4bec-910f-a80644d93c94

---

## Future Improvements
- Inverse kinematics integration
- WebSocket-based smoother control
- Computer vision integration
