# 🚀 Smart Arduino-Based Radar System with Real-Time Target Tracking

A low-cost intelligent radar platform capable of real-time object detection, target tracking, and graphical visualization using Arduino Mega 2560, ultrasonic sensors, servo motors, and a Processing-based radar interface.

---

## 📖 Project Overview

Traditional radar systems provide excellent performance but require expensive hardware and complex signal-processing units. This project demonstrates how a smart radar-like platform can be implemented using affordable embedded-system components while maintaining reliable detection, tracking, and visualization capabilities.

The system continuously scans the environment, detects nearby objects, tracks targets using a stepper motor, and displays real-time data through an interactive radar-style graphical user interface.

---

## ✨ Key Features

* 🎯 Real-Time Object Detection
* 📡 Multi-Sensor Environmental Scanning
* 🔄 Servo-Based Radar Sweep Motion
* 🎯 Automatic Target Tracking
* 📊 Real-Time Radar GUI Visualization
* ⚡ Non-Blocking Embedded Architecture
* 🔧 Digital Signal Filtering
* 🔔 Proximity Alert System
* 💰 Low-Cost Hardware Design

---

## 🏗 System Architecture

```text
Ultrasonic Sensors
        │
        ▼
Arduino Mega 2560
        │
 ┌──────┼──────┐
 ▼             ▼
Servo Motors   Stepper Motor
(Scanning)     (Tracking)
        │
        ▼
Serial Communication
        │
        ▼
Processing Radar GUI
```

---

## 🔩 Hardware Components

| Component                 | Quantity |
| ------------------------- | -------- |
| Arduino Mega 2560         | 1        |
| HC-SR04 Ultrasonic Sensor | 4        |
| Servo Motor               | 4        |
| 28BYJ-48 Stepper Motor    | 1        |
| ULN2003 Driver Module     | 1        |
| Buzzer                    | 1        |
| 12V Battery               | 1        |
| Breadboards & Wiring      | Multiple |

---

## 💻 Software Stack

* Arduino IDE (C/C++)
* Processing IDE
* Serial Communication
* Embedded Real-Time Programming
* Digital Filtering Algorithms

---

## ⚙️ Working Principle

1. Sensors scan the environment continuously.
2. Distance measurements are filtered to reduce noise.
3. Arduino processes incoming sensor data.
4. Targets are detected within predefined thresholds.
5. The system calculates target direction.
6. The stepper motor aligns toward the detected object.
7. Data is transmitted to Processing.
8. The GUI visualizes targets in real time.

---

## 📈 Project Achievements

✅ Multi-target detection

✅ Real-time tracking

✅ Stable signal processing

✅ Interactive graphical visualization

✅ Low-cost implementation

✅ Successful graduation project validation

---

## 🎓 Academic Information

**Graduation Project**

Department of Systems and Control Engineering

College of Electronics Engineering

Ninevah University

Iraq — 2026

---

## 👨‍💻 Project Team

### Students

* Mohamad Al-Sadr Raad Omran
* Mustafa Abdulwahid Shaaban
* Mohamed Subhi Saleh

### Supervisor

**Dr. Thakwan Akram Jawad**

---

## 📬 Contact

### Mohamad Raad Omran

Systems and Control Engineer

📧 Email: [hello@mohmad.net](mailto:hello@mohmad.net)

🔗 LinkedIn: https://www.linkedin.com/in/eng-mohamad

💻 GitHub: https://github.com/M-0o

---

## 📄 License

This project is provided for educational, academic, and research purposes.

---

⭐ If you found this project useful, consider giving it a star.
