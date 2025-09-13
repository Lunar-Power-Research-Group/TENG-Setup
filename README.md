# TENG Test Setup

## Overview
This setup is used to measure the AC voltage generated from continuously separating and joining the two triboelectric layers. This is done by placing a layer on the top and base plate and using the rack & pinion mechanism + a MG996R servo to move the plates into each other.

## How to use the setup
### Materials:
* 9 plates ([click here](https://cad.onshape.com/documents/5c0fde93ce1233b219407b02/w/5a7b02e2281981782c322d61/e/4ee829c3a89cbf12159e9ca7?renderMode=0&uiState=68c5b3f746557c9b987ad30f) for models):
  * Base
  * Servo holder
  * Rack
  * Pinion
  * Rack support
  * 2 platform plates
  * Platform supports
* MG996R Servo
* Digilent Analog Discovery 2
* Arduino UNO
* Arduino Multi-functional Shield
* **TODO (needs more info)**:
  * Wires/USB cables
  * Jumper wires
  * Layers and tape

## Assembly
Assembly information here!

## Usage
### Modifying parameters
On the segment display, you will see a letter and a number. The letter corresponds to which setting you are modifying. "L" represents the height (in millimeters) at which the top layer will reach; "T" represents the amount of time (in seconds) it takes for the system to separate and make contact again. The number corresponds to the value of the setting.

On the bottom of the shield, there are 3 buttons. The left two buttons handles changing the values and the right-most button switches which setting is being modified.

### Running the system
To start the system, hold the right-most button for 1 second and release. You are able to change the settings while the system is in motion. To stop, hold the same button for 1 second and release. This will put the system back into contact state.

### Logging data

