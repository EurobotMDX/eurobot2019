#ifndef ENUMS_MD25_H
	#define ENUMS_MD25_H
	
	#include <Arduino.h>
    
    namespace RD02
    {
        struct Motor {
            enum Side {  // MD25 register addresses for setting the speeds of each motor.
                _RIGHT = (byte)0x00,  // byte to access the first motor.
                _LEFT = 0x01  // byte to access the second motor.
            };
            enum Swap {  // a direction multiplier in case the wheels turn backwards when trying to move forwards.
                _UNSWAPPED = 1,
                _SWAPPED = -1
            };
        };
        struct Encoder {
            enum Side {  // MD25 register addresses for reading from each encoder.
                _RIGHT = 0x02,  // byte to read from the first encoder.
                _LEFT = 0x06  // byte to read from the second encoder.
            };
        };
        struct I2C {
            enum Address {  /* I2C addresses that the MD25 can use. On start up, the MD25 LED will flash to indicate its assigned address.
            One long flash, followed by short flashes depending on which address it is.
            Currently does not work as the other addresses appear not to work.*/
                _DEFAULT_ADDRESS = 0x58,  // default MD25 address. One long flash, one short flash.
                _ADDRESS_01 = 0xB2,  // One long flash, one short flash.
                _ADDRESS_02 = 0xB4,  // One long flash, two short flashes.
                _ADDRESS_03 = 0xB6,  // etc.
                _ADDRESS_04 = 0xB8,
                _ADDRESS_05 = 0xBA,
                _ADDRESS_06 = 0xBC,
                _ADDRESS_07 = 0xBE
            };
        };
        struct Debug {  // Determines how much information is printed to the Serial monitors as the MD25 runs commands.
            enum Level {
                _NONE = 0,  // No data is printed to the serial monitor.
                _LOW = 1,  // Only prints the function that it is currently executing and its arguments.
                _MEDIUM = 2,  // Also notifies the user of other miscellaneous information during function execution.
                _HIGH = 3  //  Also prints out I2C communications and error codes. Not yet implemented.
            };
        };
    }  
#endif