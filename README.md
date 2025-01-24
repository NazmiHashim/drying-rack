# Smart IoT Drying Rack System

The **Smart IoT Drying Rack System** is an automated solution designed to protect clothes from unpredictable weather while promoting energy efficiency and sustainability. The system integrates IoT technology with real-time weather monitoring to retract or extend the drying rack and activate a fan based on rain detection, temperature, and humidity levels. The system is aligned with **UN SDG 9 (Industry, Innovation, and Infrastructure)** and **SDG 11 (Sustainable Cities and Communities)**.

---

## Team Members
1. Mior Muhammad Irfan Bin Mior Latfee (158450)
2. Mohamad Nazmi Bin Hashim (158616)

---

## Technology Deployed
- **Microcontroller**: ESP32  
- **Sensors**:
  - DHT11 (Temperature and Humidity Sensor)  
  - Rain Sensor  
- **Actuator**: Micro Servo Motor, Fan (via Relay Module)  
- **Cloud Platform**: V-One IoT Platform  
- **Programming Languages**: C++ (Arduino)  
- **Communication Protocol**: MQTT (for cloud integration)  

---

## Description
The Smart IoT Drying Rack System works as follows:
1. **System Activation**:  
   - The system is activated by pressing a button, causing the drying rack to extend into an open area for drying.
2. **Rain Detection**:  
   - When rain is detected by the rain sensor, the drying rack automatically retracts into a roofed area to protect clothes.
   - The fan is activated to assist in drying clothes under the roofed area.
3. **Email Notifications**:  
   - Users receive real-time email alerts when rain is detected.
4. **System Deactivation**:  
   - Upon pressing the stop button, the drying rack retracts, and the fan is turned off if activated.

**Key Features**:
- Real-time monitoring and control via the V-One IoT Cloud Platform.
- Historical data visualization, including temperature, humidity, and rain trends.
- Fully automated operation with minimal user intervention.

---

## Demo
Watch the **live demonstration** of the system:  
[Smart IoT Drying Rack System Demo](https://youtu.be/ftpnjOpC7YU<YouTube_Video_ID>)  

