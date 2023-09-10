# Ardiuno-Hot-Lap-Timer-for-Hotwheels-Tracks

Ever wanted to find out how fast different diecasts stack up to each other in time attacks?

This hot lap timer uses an ultrasonic sensor to read a passing car and record its laptimes. The LCD display display the current laptime and the fastest lap. The optional python script creates a .txt file that records every laptime as well as the average laptime.
How to use:
1. Set up the Ardiuno according to the schematics below
2. Find the best place for your ultrasonic sensor and run the code

Inlcuding the optional python code:
The python script uses the serial monitor to communicate with the ardiuno. Close the serial monitor in your Ardiuno IDE before running the python script

3. Make sure that python is reading the correct COM port on line 3 of the program - you may need to change it
4. Run the python code. Give the .txt file a name (don't re-use names of already existing files. An example of what this file will look like is included)

 
__________

PARTS LIST:

Ardiuno Uno,
LCD 16x2,
Potentiometer,
220 Ohm Resistor,
Ultrasonic Sensor,
Pushbutton

![circuit](https://github.com/noah-carmichael/Ardiuno-Hot-Lap-Timer-for-Hotwheels-Tracks/assets/126828296/70d02e8b-dea0-4559-9c50-8c410bf25d83)


Video of it in use:

[![Link to youtube video](https://img.youtube.com/vi/FzqWekWGaRg/0.jpg)](https://www.youtube.com/watch?v=FzqWekWGaRg)
