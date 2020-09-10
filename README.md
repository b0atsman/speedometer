# speedometer

### Description

Simple project for IPC demo.

Speed generator app uses time based periodic function (sum of two sines) to generate abstract speed value, then passes it to data_bus library via API.

Speed displays receives speed value from data_bus library via API, and displays it in console.

Library uses shared memory mechanism for data exchange & mutex synchronization.

User must have permissions to be able to create /tmp/speed_data file (created automatically upon start of any application).

### Requirements

Ubuntu Linux (tested on v.18.04)

CMake (tested on v.3.10.2) 

g++ (tested on v.7.5)

### Build procedure

*git clone https://github.com/b0atsman/speedometer.git*

*cd speedometer*

*mkdir build*

*cd build*

*cmake ../*

*make*

### Usage

In the first terminal window run command:

*./speed_generator*

In the second terminal window run command (multiple clients allowed):

*./speed_display*
  
Generated speed data will be logged in speed_generator console, received speed data will be displayed in speed_display console.
  
