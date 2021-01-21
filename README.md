# Ethanol-Signal-Converter
Arduino Nano-based program to convert a frequency output to 0-5v.

Ethanol fueling has become a popular alternatve to traditional gasoline for its renewability and affordability. Due to its higher octane level and cooler running conditions, it has also found it's place in high performance engines. Ethanol allows engines (especially those with forced induction) to run more ignition timing and higher boost pressure, without experienceing detonation.

This program is designed to be used sepcifically for Subaru 32bit ECUs using the TGV Left/Right input. However, the program will function the same on any input/ECU that uses 0-5v sensor ins. The schematic can be found here (link).

Using a GM ethanol sensor, which reports ethanol content from 0% (50Hz) to 100% (150Hz), the Arduino Nano uses pulse width modulation to translate the signal from the GM sensor into a voltage to be read/understood by the ECU. The signal is analyzed and ECU tables adjusted using COBB's Flex Fuel software.


FUTURE ADDITIONS:
Fuel Temperature (no additional hardware)
LCD Display capability
