# IEEEZSB Fire Fighting Robot


Features :-


1- Fire Detection & Extinguishing: Detects fire using flame sensors and activates a firefighting mechanism (can be controlled manually by the mobile application or extinguishes fire automatically at autonomous mode ).
2- Line Following Mode: Follows a predefined path using a 5-channel IR sensor.

3- Manual Control Mode: The user can control the robot manually via a mobile app (made by MIT app inventor).

4- Mode Switching: The mobile application allows switching between different operation modes dynamically.

5- Bluetooth Communication: Wireless control using a Bluetooth module (HC-05).  
 


Components Used :-


1- Microcontroller: Arduino Uno R3.

2- Sensors: 5-channel IR Sensors (for line following), Flame Sensor (for fire detection).

3- Actuators: DC motors with motor driver (L298N), Servo Motor (for extinguishing mechanism).

4- Communication: Bluetooth Module (HC-05).

5- Power Supply: Rechargeable Li-ion batteries.

6- Extinguishing Mechanism: Water pump (3V - 6V).

7- Display: LCD 2*16 with I2C module (displays the current mode and fire location at autonomous fire fighting mode), Battery level indicator (to display the battery level).



Mobile Application :-


The mobile app is developed using MIT App Inventor and provides the following functionalities:

1- Manual joystick control for movement.

2- Buttons to switch between Firefighting Mode, Line Following Mode, and Manual Mode.

3- Bluetooth connectivity to send commands to the robot.


