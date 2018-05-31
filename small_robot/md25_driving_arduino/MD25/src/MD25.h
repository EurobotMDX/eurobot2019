/*
    *	MD25 I2C Library
    *	
    *	For use with an MD25 motor controller
    *	connected via I2C
    *	
    *	Written by Dief Bell, 2016
    *	diefenbaker.bell@gmail.com
    *
    *  For the full MD25 technical specifications, goto:
    *  http://www.robot-electronics.co.uk/htm/md25tech.htm
*/

#ifndef MD25_H
	#define MD25_H
	
	#include <Arduino.h>  // required for any Arduino features (e.g. I2C) to work.
	#include <Wire.h>  // wire library is needed by the functions in this library, but must also be included in the main code.
	#include "Enums.MD25.h"  // various enumerations for the library. Kept in a separate files so that they can be passed to functions (due to a bug with the compiler and the IDE).
	
	//PREPROCESSOR DEFINITIONS//
	#define CMD					(byte) 0x10		// byte for sending commands to the MD25, such as reset.
	#define RST                 0x20			// byte to reset the encoders.
	#define VOLTREAD            0x0A			// byte to read the voltage of the battery powering the MD25.
	#define ACCELERATION		0xD				// byte to set the MD25 acceleration (defaults to 5/10);
	#define MODE_SELECTOR		0xF				// byte to set the control mode.
    
    namespace RD02
    {
        class MD25
        {
            public:
            ////CONSTRUCTORS////{
                MD25 ();  // useful for if you only need to check battery voltage, or if the MD25 is initialised in Setup() because some code must be run first.
                MD25 (int s_wheelCircumference, int s_turnRadius);
                MD25 (int s_wheelCircumference, int s_turnRadius, Debug::Level s_debugLevel);
                
                MD25 (int s_wheelCircumference, int s_turnRadius, int s_defaultSpeed);
                MD25 (int s_wheelCircumference, int s_turnRadius, int s_defaultSpeed, Debug::Level s_debugLevel);
                
                MD25 (int s_wheelCircumference, int s_turnRadius, int s_defaultSpeed, Motor::Swap s_MotorSwap);
                MD25 (int s_wheelCircumference, int s_turnRadius, int s_defaultSpeed, Motor::Swap s_MotorSwap, Debug::Level s_debugLevel);
                
                MD25 (int s_wheelCircumference, int s_turnRadius, int s_defaultSpeed, Motor::Swap s_MotorSwap, int s_acceleration);
                MD25 (int s_wheelCircumference, int s_turnRadius, int s_defaultSpeed, Motor::Swap s_MotorSwap, int s_acceleration, Debug::Level s_debugLevel);	
                
                MD25 (int s_wheelCircumference, int s_turnRadius, int s_defaultSpeed, Motor::Swap s_MotorSwap, int a_acceleration, I2C::Address s_address);
                MD25 (int s_wheelCircumference, int s_turnRadius, int s_defaultSpeed, Motor::Swap s_MotorSwap, int s_acceleration, I2C::Address s_address, Debug::Level s_debugLevel);
            //}
            
            ////PUBLIC ACCESSORS//{
                byte GetAddress ();
                void SetAddress (I2C::Address n_address);
                
                int GetAcceleration ();
                void SetAcceleration (int n_acceleration);
                
                int GetWheelCircumference ();
                void SetWheelCircumference (int n_wheelCircumference);
                
                int GetTurnRadius ();
                void SetTurnRadius (int n_turnRadius);
                
                int GetDefaultSpeed ();
                void SetDefaultSpeed (int n_defaultSpeed);
                
                Debug::Level GetDebugLevel ();
                void setDebugLevel (Debug::Level n_debugLevel);
                
                Motor::Swap GetMotorSwap ();
                void setMotorSwap (Motor::Swap n_motorSwap);
            //}
            
            ////PRIMARY FUNCTIONS//{
                void Forward (int distance);
                void Forward (int distance, float (*falloff)(float f));
                void Forward (int distance, int speed);
                void Forward (int distance, int speed, float (*falloff)(float f));
                
                bool AdvancedForward (int distance, bool (*interruptCondition)(), bool (*exitCondition)());
                bool AdvancedForward (int distance, bool (*interruptCondition)(), bool (*exitCondition)(), float (*falloff)(float f));
                bool AdvancedForward (int distance, bool (*interruptCondition)(), bool (*exitCondition)(), int speed);
                bool AdvancedForward (int distance, bool (*interruptCondition)(), bool (*exitCondition)(), int speed, float (*falloff)(float f));
                
                void Turn (int angle);
                void Turn (int angle, float (*falloff)(float f));
                void Turn (int angle, int speed);
                void Turn (int angle, int speed, float (*falloff)(float f));
                
                bool AdvancedTurn (int angle, bool (*interruptCondition)(), bool (*exitCondition)());
                bool AdvancedTurn (int angle, bool (*interruptCondition)(), bool (*exitCondition)(), float (*falloff)(float f));	
                bool AdvancedTurn (int angle, bool (*interruptCondition)(), bool (*exitCondition)(), int speed);
                bool AdvancedTurn (int angle, bool (*interruptCondition)(), bool (*exitCondition)(), int speed, float (*falloff)(float f));
                
                void Arc (int arcRadius, int angle);
                void Arc (int arcRadius, int angle, float (*falloff)(float f));
                void Arc (int arcRadius, int angle, int speed);
                void Arc (int arcRadius, int angle, int speed, float (*falloff)(float f));
                
                bool AdvancedArc (int arcRadius, int angle, bool (*interruptCondition)(), bool (*exitCondition)());
                bool AdvancedArc (int arcRadius, int angle, bool (*interruptCondition)(), bool (*exitCondition)(), float (*falloff)(float f));
                bool AdvancedArc (int arcRadius, int angle, bool (*interruptCondition)(), bool (*exitCondition)(), int speed);
                bool AdvancedArc (int arcRadius, int angle, bool (*interruptCondition)(), bool (*exitCondition)(), int speed, float (*falloff)(float f));
                
                float GetBatteryVoltage();  // returns the voltage of the MD25's power supply (e.g. a 12v battery).
            //}
            
            private:
            ////COMMON CONSTRUCTOR////{
                void init (int s_wheelCircumference, int s_turnRadius, Debug::Level s_debugLevel, int s_defaultSpeed, Motor::Swap s_MotorSwap, int a_acceleration, I2C::Address s_address);
            //}   
            
            ////PRIVATE VARIABLES//{
                I2C::Address address = I2C::_DEFAULT_ADDRESS;  // the MD25's address (as there are multiple addresses for an MD25).
                int acceleration = 5;  // acceleration of the motors.
                
                int wheelCircumference = 0;  // circumference of the wheels used e.g. distance travelled per motor rotation.
                int turnRadius = 0;  // half of the distance between wheels. Used for calculating arcs and turns.
                
                int defaultSpeed = 20;  // the speed at which the MD25 will move if no other speed is specified.
                
                Motor::Swap motorSwap = Motor::_UNSWAPPED;  // if the motors move the robot backwards due to them being connected in reverse, this corrects that.
                
                Debug::Level debugLevel = Debug::_MEDIUM;  // how much information should be sent to the Serial monitor for debugging purposes.
            //}
            
            ////CONSTANTS//{
                static const int SPEEDOFFSET;  // just adds 128 to all speeds, so that positive numbers move the motors forwards, and negative backwards.
                static const double DEG2RAD;  // converts degree values to radians for when working with arcs.
            //}
            
            ////UTILITY FUNCTIONS//{
                void setMotorSpeed (Motor::Side motor, int speed);  // sets the speed of the specified motor to the given speed.
                void stopMotors ();  // stops both of the motors. More efficient than just specifying zero speed.
                
                int averageDistance ();  // gets the average distance travelled by both wheels since the encoders were last reset
                int getDistance (Encoder::Side encoder);  // gets the distance travelled by the specified wheel (in mm).
                long getEncoder (Encoder::Side encoder);  // returns the number of degrees rotated by the specified wheel since the encoder was last reset.
                void resetEncoders ();  // resets both encoders.
                
                int setAcceleration ();  // synchronizes the current acceleration to the MD25.
                int setAddress (I2C::Address n_address);  // sets a new address for the connected MD25
            //}
        };
        
        ////falloff FUNCTION LIBRARY////{
        struct falloffFunctions {
            static float Constant (float f);  // keeps the motors at the same speed for the duration of the manoeuvre.
            static float Linear_10percent (float f);  // linearly reduces the speed to 10% over the duration of the manoeuvre.
            static float Final10percent_linear_10percent (float f);  // keeps the motor speeds constant for the first 90% of the manoeuvre, after which it is reduced to 10% linearly over the remaining 10%.
        };
        //}
    }
#endif