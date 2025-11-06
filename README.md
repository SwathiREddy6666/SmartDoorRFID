# SmartDoorRFID
# 🔐 Configurable Password Security of Digital Door System Using Advanced Controllers

### 🎯 Overview
This project implements a **dual-authentication digital door locking system** using **Arduino UNO**, **RFID**, and **Keypad** modules.  
It provides a **secure, modular, and affordable** solution for access control in homes, offices, and institutions.

The system performs two-step authentication:
1. **RFID card verification**
2. **Keypad password validation**

Only when both credentials match is access granted; otherwise, the system alerts the user via an LCD display and buzzer.

---

## ⚙️ Features
- Dual authentication: RFID + Password verification  
- Real-time LCD display and buzzer alerts  
- Servo motor–based door actuation  
- Automatic reset after each attempt  
- Lockout after multiple failed attempts  
- Modular Arduino code (easy to upgrade for IoT / Wi-Fi / GSM features)  
- Compact, low-cost, and energy-efficient design  

---

## 🧠 System Architecture
The Arduino UNO acts as the central controller, interfacing with:
- **RFID Reader (MFRC522)** for card scanning  
- **4×4 Keypad** for password input  
- **Servo Motor** for locking/unlocking  
- **LCD Display (I2C)** for feedback  
- **Buzzer / LED** for alerts  


