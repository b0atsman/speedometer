# speedometer

## Description

Sample project for IPC demo.

Speed generator app used time based periodic function (sum of two sines) to generate abstract speed value, then passes it to data_bus library via API.

Speed displays receives speed value from data_bus library via API, and displays it in console.

User must have permissions to be able to create /tmp/speed_data file (created automatically upon start of any application).

## Build procedure:

*git clone https://github.com/b0atsman/speedometer.git*

*cd speedometer*

*mkdir build*

*cd build*

*cmake ../*

*make*

## Usage:

Inside first terminal window run command:

*./speed_generator*

Inside second terminal window run command (multiple displays allowed):

*./speed_display*
  
Generated speed data will be displayed in speed_generator console, received speed data will be displayed in speed_display console.
  
