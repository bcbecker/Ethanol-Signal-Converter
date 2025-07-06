# Ethanol-Signal-Converter
Arduino Nano-based program to convert a frequency output to 0-5v.

Ethanol fueling has become a popular alternatve to traditional gasoline for its renewability and affordability. Due to its higher octane level and cooler running conditions, it has also found it's place in high performance engines. Ethanol allows engines (especially those with forced induction) to run more ignition timing and higher boost pressure, without experienceing detonation.

This program is designed to be used sepcifically for Subaru 32bit ECUs using the TGV Left/Right input. However, the program will function the same on any input/ECU that uses 0-5v sensor ins. The schematic can be found here (link).

Using a GM ethanol sensor, which reports ethanol content from 0% (50Hz) to 100% (150Hz), the Arduino Nano uses pulse width modulation to translate the signal from the GM sensor into a voltage to be read/understood by the ECU. The signal is analyzed by the ECU, where the tables are adjusted using COBB's Flex Fuel software. Since this is designed to feed directly to the ECU, there is no need for an LCD display. The data is viewable via COBB Accessport, or any other OBDII-based real-time logger.


*POTENTIAL ADDITIONS:*
While it is possible to utilize an LCD to display the ethanol content, the beauty in the current form is that it is lightweight, and integrated directly into the OEM ECU. An LCD could, however, become useful in displaying fuel temperature data (which could be done with no additional hardware) or simply monitoring ethanol content without a direct link to the ECU. Fuel temperature data may be added later in the form of a second .ino file, E85_ECU_V2.

**NOTE:** Designed specifically for the ATmega328P Nano and is not compatible with the newer Nano ESP32.
