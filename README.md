# 🔐 Arduino Security System
An Arduino-based smart security system with RFID access control, flame and sound sensors, buzzer alert, and LCD feedback.

## 📽️ Demo Video
> 🔊 **Note:** Turn on your volume when watching the demo!

[![Watch the demo]](https://github.com/user-attachments/assets/b62b862b-97e5-4f9a-bec9-88ddfdaf8b49)

## 📐 Schematics 
![Image](https://github.com/user-attachments/assets/84570d14-7174-42bf-91db-476079b6e82d)


## 🔧 Pre-requisites – Components Used

- Arduino UNO  
- RFID-RC522 Module  
- 16x2 LCD Display (with potentiometer)  
- Flame sensor  
- Sound sensor  
- Active buzzer  
- Red and green LEDs  
- Jumper wires  
- Breadboard  
- 5V USB or external power supply  

## ⚙️ Setup and Build Plan

- Connect the LCD to digital pins 7-2
- Connect RFID module via digital pins 13-9  
- Use A0 and A3 for the flame and sound sensors  
- A4: buzzer, A5: red LED, D8: green LED
- Use a potentiometer to calibrate LCD's contrast  
- Calibrate sensor thresholds in code as needed  
- Upload the sketch via Arduino IDE and monitor via Serial  

## 🚨 Behavior

- System starts in UNLOCKED state  
- Scanning the correct tag toggles LOCKED/UNLOCKED, scanning an unauthorized tag will not lock the system  
- Flame or sound detection in LOCKED mode triggers persistent alarm  
- Flame detection in UNLOCKED mode only activates buzzer temporarily 

## ✅ Result

- Full interaction and real-time responses between components  
- Clear state transitions and intuitive user feedback  
- All logic and thresholds handled within a structured Arduino sketch

## 📁 Files

- [`security_system.ino`](code/security_system.ino): Arduino source code
