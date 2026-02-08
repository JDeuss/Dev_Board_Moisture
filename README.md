# Dev_Board_Moisture_Sensor

This board is designed to measure soil moisture and transmit the data to a Raspberry Pi via MQTT

![Schematic](docs/Sensor%20Revision%202.jpg)

## Usage

After assembling the board and connecting or soldering the LiPo cell, the board must first be powered via a USB‑C cable.
Before uploading the official firmware, the sensor must be calibrated.
To do this, upload the calibration code. The ESP can be programmed through the USB programmer adapter using the Arduino IDE.
After flashing the calibration sketch, place the sensor once in dry soil and once in moist soil or water.
Note the values displayed during both measurements and adjust the main firmware accordingly.

Before uploading the main firmware, make sure to update the Wi‑Fi credentials and the MQTT broker settings in the code.


