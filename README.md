# AviationSoftwareProject
 Part of ROS

# Introduction
We loaded the most accessible features in RC into the Android App, making it easy for anyone to use RC of drones that are difficult to operate. *Intuition* has been implemented so that users can operate drones without being distracted by operating the app. Using *PX4* and *ROS* open source, I collected sensing information of the drone, controlled the position of the drone. 
*ROS* is an open source for Robotics, and *PX4* is an open source for *Autopilot*, serving as *Sensing, Controlling, and Communication*.
With the experience of using an open source, I will be able to quickly adapt and apply to various system development. And also, through this project, I had an understanding of various systems ranging from S/W to H/W.

# Objectives
* *ROS* : Implementing offboard code for functions such as east-west, north-south movement, up-and-down movement, rotation, etc.
*  *Android* : Implementation of *ROS* Node-*Android* Communication, GUI Design, Parameter Restriction.
*  *H/W* : Drone assembly and management, *QGC* calibration, *QGC* to offboard node connection

# Hardware

<img src="https://user-images.githubusercontent.com/26537107/125022640-1cfed880-e0b8-11eb-8ead-4d02e4058864.jpg" width="300" higeht="200">

* Completed Assembly of Drone

<img src="https://user-images.githubusercontent.com/26537107/125022646-1e300580-e0b8-11eb-9062-723c28440a4f.jpg" width="400" higeht="200">

* [QGC Calibration] Radio & RC mapping

<img src="https://user-images.githubusercontent.com/26537107/125022652-21c38c80-e0b8-11eb-8e94-00c6239816bb.jpg" width="400" higeht="200">

* [QGC Calibration] Sensor Calibration
  * Calibrate of *Compass, Gyroscope, Accelerometer, level horizonal sensor* of drone.

# Software

(1) Interworking between *H/W* and *S/W*

<img src="https://user-images.githubusercontent.com/26537107/125024039-ff7f3e00-e0ba-11eb-9f03-b270a1b62834.jpg" width="400" higeht="200">


* Among the several sensors connected to *Pixhawk*, the flight status was wirelessly transmitted through *Telemetry* used to communicate with surrounding devices, between the drone and the *QGC* system.
* Data was sent to *ROS* via *TCP socket* communication between *Android* and *ROS* nodes.
* Within *ROS*, the position of the drone was controlled by transferring data in a *Pubscribe/Subscribe* manner between two nodes.

(2) GUI Design

<img src="https://user-images.githubusercontent.com/26537107/125024044-01e19800-e0bb-11eb-84c7-645a158f067a.jpg" width="150" higeht="350">

<img src="https://user-images.githubusercontent.com/26537107/125024050-03ab5b80-e0bb-11eb-8247-8d2110659f3e.jpg" width="150" higeht="350">

(3) Gazebo Simulation

<img src="https://user-images.githubusercontent.com/26537107/125024056-05751f00-e0bb-11eb-9c58-e4c30c4ee63f.png" width="400" higeht="200">

* The safety of operation is important because drones are equipped with various sensors and the influence of the external environment is significant. So, we did simulation in a virtual *Gazebo* environment before the flight.

# Test Flight

<img src="https://user-images.githubusercontent.com/26537107/125024060-073ee280-e0bb-11eb-935d-a326a99607e2.jpg" width="200" higeht="200">

<img src="https://user-images.githubusercontent.com/26537107/125024069-0d34c380-e0bb-11eb-9b38-d5afa35877a0.jpg" width="200" higeht="200">


# Achievment
* Receive input of the **distance** and **direction**.

* Maintain **hovering status** if no other application input is available.

* Press the **Back button** to return to the previous position.

* Press the **Home button** to return to the initial start position.

* Select the **clock and counter-clockwise direction** and **rotate** 90 degrees relative to the z-axis.

* Press the **Up-Down button** to control the height.

* Display the **current position** in the application.
   * Highlight position numbers changed for 0.3 seconds each time the distance changes

* For safety, restrict the parameter from moving more than a certain distance.

* Event does not go over when double clicks are made in less than 3 seconds.
   * Due to the slow response rate of the drone, multiple clicks of the button in a row can cause the drone to move rapidly.

* By sending a toast message from the application, all **errors and exceptions are visible** to the user 

