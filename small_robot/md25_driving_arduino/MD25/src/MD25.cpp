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

#include <Arduino.h>
#include <Wire.h>

#include "MD25.h"
#include "Enums.MD25.h"  // various enumerations for the library. Kept in a separate files so that they can be passed to functions (due to a bug with the compiler and the IDE).

namespace RD02
{
    
    //CONSTANT DEFINITIONS//{
        const int MD25::SPEEDOFFSET = 128;
        const double MD25::DEG2RAD = PI / 180.0;
    //}
    
    //CONSTRUCTORS//{
        MD25::MD25 ()
        {
            /*
                The default constructor for MD25 objects does
                nothing but initialize the object, and is only
                used for debugging purposes, such as reading
                the battery voltage to the Serial monitor.
            */
            if (Serial && debugLevel >= Debug::_LOW)
            {
                Serial.println("An abstract MD25 object has been created.");
                Serial.println("------------------------------------------------------------");
            }
            Wire.begin ();  // starts the Wire connection for I2C communications.
        }
        MD25::MD25 (int s_wheelCircumference, int s_turnRadius)
        {
            /*
                This is the most basic setting for a functioning MD25 system.
                The wheelCircumference value is used to calculate the number
                of revolutions the motor must undergo to move a set distance,
                and the turnRadius is used to calculate the distance each wheel 
                must travel when turning / arcing.
            */
            init (s_wheelCircumference, s_turnRadius, Debug::_LOW, defaultSpeed, Motor::_UNSWAPPED, 5, I2C::_DEFAULT_ADDRESS);
        }
        MD25::MD25 (int s_wheelCircumference, int s_turnRadius, Debug::Level s_debugLevel)
        {
            /*
                As previous, but also allows the user to set the amount of information
                they want sending to the serial monitor i.e. the "debug level".
            */
            init (s_wheelCircumference, s_turnRadius, s_debugLevel, defaultSpeed, Motor::_UNSWAPPED, 5, I2C::_DEFAULT_ADDRESS);
        }
        
        MD25::MD25 (int s_wheelCircumference, int s_turnRadius, int s_defaultSpeed)
        {
            /*
                Allows the user to specify a default speed for movement.
            */
            init (s_wheelCircumference, s_turnRadius, Debug::_LOW, s_defaultSpeed, Motor::_UNSWAPPED, 5, I2C::_DEFAULT_ADDRESS);
        }
        MD25::MD25 (int s_wheelCircumference, int s_turnRadius, int s_defaultSpeed, Debug::Level s_debugLevel)
        {
            /*
                As above, but with debugLevels.
            */
            init (s_wheelCircumference, s_turnRadius, s_debugLevel, s_defaultSpeed, Motor::_UNSWAPPED, 5, I2C::_DEFAULT_ADDRESS);
        }
        
        MD25::MD25 (int s_wheelCircumference, int s_turnRadius, int s_defaultSpeed, Motor::Swap s_motorSwap)
        {
            /*
                If the motors are connected to the MD25 such that the
                Forward () function moves the robot backwards instead
                of forward then setting swapMotors to SWAPPED fixes this
                without needing to alter the hardware/electronics.
            */
            init (s_wheelCircumference, s_turnRadius, Debug::_LOW, s_defaultSpeed, s_motorSwap, 5, I2C::_DEFAULT_ADDRESS);
        }
        MD25::MD25 (int s_wheelCircumference, int s_turnRadius, int s_defaultSpeed, Motor::Swap s_motorSwap, Debug::Level s_debugLevel)
        {
            /*
                As above, but with debugLevels.
            */
            init (s_wheelCircumference, s_turnRadius, s_debugLevel, s_defaultSpeed, s_motorSwap, 5, I2C::_DEFAULT_ADDRESS);
        }
        
        MD25::MD25 (int s_wheelCircumference, int s_turnRadius, int s_defaultSpeed, Motor::Swap s_motorSwap, int s_acceleration)
        {
            /*
                For the setting of custom accelerations,
                between the values of 1 and 10.
            */
            init (s_wheelCircumference, s_turnRadius, Debug::_LOW, s_defaultSpeed, s_motorSwap, s_acceleration, I2C::_DEFAULT_ADDRESS);
        }
        MD25::MD25 (int s_wheelCircumference, int s_turnRadius, int s_defaultSpeed, Motor::Swap s_MotorSwap, int s_acceleration, Debug::Level s_debugLevel)
        {
            /*
                As above, but with debugLevels.
            */
            init (s_wheelCircumference, s_turnRadius, s_debugLevel, s_defaultSpeed, s_MotorSwap, s_acceleration, I2C::_DEFAULT_ADDRESS);
            
        }
        
        MD25::MD25 (int s_wheelCircumference, int s_turnRadius, int s_defaultSpeed, Motor::Swap s_MotorSwap, int s_acceleration, I2C::Address s_address)
        {
            /*
                Allows the setting of a custom address for the MD25.
            */
            init (s_wheelCircumference, s_turnRadius, Debug::_LOW, s_defaultSpeed, s_MotorSwap, s_acceleration, s_address);
            
        }
        MD25::MD25 (int s_wheelCircumference, int s_turnRadius, int s_defaultSpeed, Motor::Swap s_MotorSwap, int s_acceleration, I2C::Address s_address, Debug::Level s_debugLevel)
        {
            /*
                As above, but with debugLevels.
            */
            init (s_wheelCircumference, s_turnRadius, s_debugLevel, s_defaultSpeed, s_MotorSwap, s_acceleration, s_address);
        }
    //}
    
    //PUBLIC ACCESSORS//{
        byte MD25::GetAddress ()  // returns the current address of the MD25.
        {
            return address;
        }
        void MD25::SetAddress (I2C::Address n_address)  // sets the address of the MD25 to any of the possible addresses.
        {
            setAddress (n_address);  // calls the setAddress subroutine
        }
		
        int MD25::GetAcceleration ()  // returns the current acceleration of the MD25.
        {
            return acceleration;
        }
        void MD25::SetAcceleration (int n_acceleration)  // sets the acceleration of the MD25.
        {
            acceleration = constrain(n_acceleration, 1, 10);  // ensures the given acceleration is between 1 and 10. See documentation (linked above) for details.
            setAcceleration ();  // calls the setAcceleration subroutine.
        }
		
        int MD25::GetWheelCircumference ()  // returns the current wheelCircumference of the wheels controlled by the MD25.
        {
            return wheelCircumference;
        }
        void MD25::SetWheelCircumference (int n_wheelCircumference)  // sets the wheelCircumference value used in calculations. Probably not needed, but who knows what use someone may find for it?
        {
            wheelCircumference = n_wheelCircumference;
        }
		
        int MD25::GetTurnRadius ()  // returns the current turn radius that the robot makes.
        {
            return turnRadius;
        }
        void MD25::SetTurnRadius (int n_turnRadius)  // sets the turn radius of the robot for use in calculations (as above). Is equal to half the robot's wheel spacing.
        {
            turnRadius = n_turnRadius;
        }
		
        int MD25::GetDefaultSpeed ()  // returns the default speed of the robot.
        {
            return defaultSpeed;
        }
        void MD25::SetDefaultSpeed (int n_defaultSpeed)  // sets the default speed of the robot.
        {
            defaultSpeed = n_defaultSpeed;
        }
		
        Debug::Level MD25::GetDebugLevel ()  // returns the current debug level.
        {
            return debugLevel;
        }
        void MD25::setDebugLevel (Debug::Level n_debugLevel)  // sets the debug level for the MD25 object.
        {
            debugLevel = n_debugLevel;
        }
        
        Motor::Swap MD25::GetMotorSwap ()  // gets whether the motors are "swapped".
        {
            return motorSwap;
        }
        void MD25::setMotorSwap (Motor::Swap n_motorSwap)  // sets the motor swap.
        {
            motorSwap = n_motorSwap;
        }
    //}
    
    //COMMON CONSTRUCTOR//{
        void MD25::init (int s_wheelCircumference, int s_turnRadius, Debug::Level s_debugLevel, int s_defaultSpeed, Motor::Swap s_MotorSwap, int a_acceleration, I2C::Address s_address)
        {
             Wire.begin ();  // starts the Wire connection for I2C communications.
        
            wheelCircumference = s_wheelCircumference;
            turnRadius = s_turnRadius;
            defaultSpeed = s_defaultSpeed;
            motorSwap = s_MotorSwap;
            
            int errCodeAddr = setAddress (s_address);  // sets the MD25 address being used.
            
            // Sets the MD25's communication mode (i.e. what instruction each byte is associated with).
            Wire.beginTransmission (address);
            Wire.write (MODE_SELECTOR);
            Wire.write (0);  // sets the MD25's mode to zero (different modes affect what writing to each register does).
            int errCodeMode = Wire.endTransmission (); // <<<Add Debug Level>>
            
            acceleration = constrain (acceleration, 1, 10);
            int errCodeAcc = setAcceleration ();  // <<Add Debug Level>>
            
            debugLevel = s_debugLevel;
            
            if (Serial && debugLevel >= Debug::_LOW)
            {
                Serial.print ("A functional MD25 object has been created");
                if (debugLevel >= Debug::_MEDIUM)
                {
                        Serial.println (" with parameters:");
                        Serial.println ("\tWheel Circumference\t->\t" + String(wheelCircumference) + " mm.");
                        Serial.println ("\tTurn Radius\t\t->\t" + String(turnRadius) + " mm.");
                        Serial.println ("\tDefault Speed\t\t->\t" + String(abs(defaultSpeed)) + ".");
                        Serial.println ("\tMotor Swap status\t->\t" + String(motorSwap) + ".");
                        
                        Serial.print ("\tMD25 Address\t\t->\t" + String(address));
                        if (debugLevel == Debug::_HIGH) Serial.println ("; Error Code: " + String(errCodeAddr) + ".");
                        else Serial.println (".");
                        
                        Serial.print ("\tCommunication Mode\t->\tO");
                        if (debugLevel == Debug::_HIGH) Serial.println ("; Error Code: " + String(errCodeMode) + ".");
                        else Serial.println (".");
                        
                        Serial.print ("\tAcceleration\t\t->\t" + String(acceleration));
                        if (debugLevel == Debug::_HIGH) Serial.println ("; Error Code: " + String(errCodeAcc) + ".");
                        else Serial.println (".");
                        
                        Serial.println ("\tDebug Level\t\t->\t" + String(debugLevel) + ".");
                        
                        Serial.println ("------------------------------------------------------------");
                } else Serial.println (".");
            }
        }
    //}
    
    //PRIMARY FUNCTIONS//{
        //FORWARD//{
            void MD25::Forward (int distance)
            {
                AdvancedForward (distance, [] () -> bool {return false;}, [] () -> bool {return false;}, defaultSpeed, falloffFunctions::Constant);
            }
            void MD25::Forward (int distance, float (*falloff)(float f))
            {   
                AdvancedForward (distance, [] () -> bool {return false;}, [] () -> bool {return false;}, defaultSpeed, falloff);
            }
            void MD25::Forward (int distance, int speed)
            {
                AdvancedForward (distance, [] () -> bool {return false;}, [] () -> bool {return false;}, speed, falloffFunctions::Constant);
            }
            void MD25::Forward (int distance, int speed, float (*falloff)(float f))
            {
                AdvancedForward (distance, [] () -> bool {return false;}, [] () -> bool {return false;}, speed, falloff);
            }
            
            bool MD25::AdvancedForward (int distance, bool (*interruptCondition)(), bool (*exitCondition)())
            {
                return AdvancedForward (distance, interruptCondition, exitCondition, defaultSpeed, falloffFunctions::Constant);
            }
            bool MD25::AdvancedForward (int distance, bool (*interruptCondition)(), bool (*exitCondition)(), float (*falloff)(float f))
            {
                return AdvancedForward (distance, interruptCondition, exitCondition, defaultSpeed, falloff);
            }
            bool MD25::AdvancedForward (int distance, bool (*interruptCondition)(), bool (*exitCondition)(), int speed)
            {
                return AdvancedForward (distance, interruptCondition, exitCondition, speed, falloffFunctions::Constant);        
            }
            bool MD25::AdvancedForward (int distance, bool (*interruptCondition)(), bool (*exitCondition)(), int speed, float (*falloff)(float f))
            {
                /*Docstring
                    params:
                        - int distance: the distance in mm that the robot will move forwards.
                        - bool* interruptCondition: a bool function that will temporarily pause
                          execution of the function if true.
                        - bool* exitCondition: a bool function that will exit from the function
                          with a return value of true if true.
                        - int speed: the speed at which the robot will move forwards.
                        - float* falloff: a function that accepts a float (0 <= f <= 1) as a fraction of the function to completion.
                          The return value will then scale the speed accordingly.
                    returns:
                        - (bool) true: exitCondition returns true at any point in the function.
                        - (bool) false: exitCondition never returns false.
                    desc:
                        Moves the robot forward the defined distance at
                          the defined speed. The speed is scaled
                          over the length of the function by the return of <falloff()>.
                    
                        If "interruptCondition" returns true, then the
                          robot will pause movement until it returns false.
                    
                        If "exitCondition" returns true then the robot
                          will stop moving, and the function will exit with
                          a return value of "true". Returns "false" if the
                          exit condition is not reached.
                    techDesc:
                        Uses a loop to continuously set the speeds of the
                          left and right motors to the same speed, defined
                          by the "speed" argument and the "motorSwap"
                          member variable. It is then multiplied by the
                          falloff function, which accepts a float 0->1, the ratio
                          of the travelled distance over the required distance, and
                          returns a float 0->1 that the speed is scaled by. This
                          helps to reduce overshoot if a "good" function
                          is designed and implemented.
                    
                        It is worth noting that the speeds are continuously set
                          because the md25 will stop the motors after no signal
                          is sent for two seconds as a safety feature.
                    
                        The loop is terminated once the average distance
                          traversed by each wheel is greater than or equal to
                          the desire distance, and is calculated by<averageDistance()>.
                    
                        Within the loop, <exitCondition()> is checked, and the function
                          exits with a return value of "true" (to notify the caller) if
                          <exitCondition()> returns true.
                    
                        If <interruptCondition()> returns "true" then the motors are
                          stopped until it returns "false". In addition, <exitCondition()>
                          is still checked during the pause.
                */
                
                if (Serial && debugLevel >= Debug::_LOW) Serial.println ("\n-------------------- MD25.Forward(" + String(distance) + ", " + String(speed) + ") --------------------");
                
                if (speed <= 0) return false;  // if for some reason the speed is less than zero, just exit the function.
                resetEncoders ();  // reset the motor encoders for tracking the distance travelled.
                
                int dir = 1;
                if (distance < 0)
                {
                    dir = -1;
                    distance *= -1;
                }
                
                int dist = averageDistance ();
                while (dist <= distance)  // loops until the required distance has been reached.
                {
                    if (exitCondition ())  // checks whether the "exitCondition" has been met.
                    {
                        stopMotors ();  // stops the motors.
                        return true;  // returns "true" so the caller knows the "exitCondition" was met.
                    }
                    if (interruptCondition())  // checks whether the "interruptCondition" has been met.
                    {
                        stopMotors ();  // stops the motors.
                        if (exitCondition()) return true;  // checks whether the "exitCondition" has been met and returns "true" to the caller if it has.
                    }
                    else
                    {
                        float falloffMultiplier = falloff(dist/float(distance));
                        if (Serial && debugLevel >= Debug::_MEDIUM && falloffMultiplier < 1) Serial.println("\tFalloff\t\t->\t" + String(falloffMultiplier * 100) + "%.");
                
                        setMotorSpeed (Motor::_LEFT, speed * dir * falloffMultiplier);  // sets the speed and movementDirection of the left-hand motor.
                        setMotorSpeed (Motor::_RIGHT, speed * dir * falloffMultiplier);  // sets the speed and direction of the right-hand motor.
                    }
                    
                    dist = averageDistance ();
                }
                
                stopMotors ();  // stops the motors.
				
				if (Serial && debugLevel >= Debug::_MEDIUM) Serial.println("\tOvershoot\t->\t" + String(averageDistance() - float(distance)) + " mm.");
				
                return false;
            }
        //}
        
        //TURN//{
            void MD25::Turn (int angle)
            {
                AdvancedTurn (angle, [] () -> bool {return false;}, [] () -> bool {return false;}, defaultSpeed, falloffFunctions::Constant);
            }
            void MD25::Turn (int angle, float (*falloff)(float f))
            {
                AdvancedTurn (angle, [] () -> bool {return false;}, [] () -> bool {return false;}, defaultSpeed, falloff);            
            }
            void MD25::Turn (int angle, int speed)
            {
                AdvancedTurn (angle, [] () -> bool {return false;}, [] () -> bool {return false;}, speed, falloffFunctions::Constant);
            }
            void MD25::Turn (int angle, int speed, float (*falloff)(float f))
            {
                AdvancedTurn (angle, [] () -> bool {return false;}, [] () -> bool {return false;}, speed, falloff);
            }
            
            bool MD25::AdvancedTurn (int angle, bool (*interruptCondition)(), bool (*exitCondition)())
            {
                AdvancedTurn (angle, interruptCondition, exitCondition, defaultSpeed, falloffFunctions::Constant);
            }
            bool MD25::AdvancedTurn (int angle, bool (*interruptCondition)(), bool (*exitCondition)(), float (*falloff)(float f))
            {
                AdvancedTurn (angle, interruptCondition, exitCondition, defaultSpeed, falloff);
            }
            bool MD25::AdvancedTurn (int angle, bool (*interruptCondition)(), bool (*exitCondition)(), int speed)
            {
                AdvancedTurn (angle, interruptCondition, exitCondition, speed, falloffFunctions::Constant);
            }
            bool MD25::AdvancedTurn (int angle, bool (*interruptCondition)(), bool (*exitCondition)(), int speed, float (*falloff)(float f))
            {
                /*Docstring
                    params:
                        - int angle: the distance in mm that the robot will move forwards.
                        - bool* interruptCondition: a bool function that will temporarily pause
                          execution of the function if true.
                        - bool* exitCondition: a bool function that will exit from the function
                          with a return value of true if true.
                        - bool doCorrection: whether or not to do a slower correction turn after it first executed the manoeuvre.
                        - int speed: the speed at which the robot will move forwards.
                        - float* falloff: a function that accepts a float (0 <= f <= 1) as a fraction of the function to completion.
                          The return value will then scale the speed accordingly.
                    returns:
                    - (bool) true: exitCondition returns true at any point in the function.
                    - (bool) false: exitCondition never returns false.
                    desc:
                        Turns the robot by defined distance at
                          the defined speed. The speed is scaled
                          over the length of the function by the
                          return of <falloff()>.
                    
                        If "interruptCondition" returns true, then the
                          robot will pause movement until it returns false.
                    
                        If "exitCondition" returns true then the robot
                          will stop moving, and the function will exit with
                          a return value of "true". Returns "false" if the
                          exit condition is not reached.
                    
                        If doCorrection is true then upon completing the
                          initial turn, the MD25 will reread the motor encoders
                          and make a small correction turn as 1/10th its original speed.
                    techDesc:
                        Uses a loop to continuously set the speeds of the
                          left and right motors to the same speed but in opposite directions,
                          defined by the "speed" argument and the "motorSwap"
                          member variable.
                          
                        The speed is then multiplied by the
                          falloff function, which accepts a float 0->1, the ratio
                          of the travelled distance over the required distance, and
                          returns a float 0->1 that the speed is scaled by. This
                          helps to reduce overshoot if a "good" function
                          is designed and implemented.
                    
                        It is worth noting that the speeds are continuously set
                          because the md25 will stop the motors after no signal
                          is sent for two seconds as a safety feature.
                    
                        The loop is terminated once the average distance
                          traversed by each wheel is greater than or equal to
                          the length of the arc that each wheel must trace, given by
                          the turn angle and "turnRadius" member variable,
                          and is calculated by <averageDistance()>.
                    
                        Within the loop, <exitCondition()> is checked, and the function
                          exits with a return value of "true" (to notify the caller) if
                          <exitCondition()> returns true.
                    
                        If <interruptCondition()> returns "true" then the motors are
                          stopped until it returns "false". In addition, <exitCondition()>
                          is still checked during the pause.
                */
                
                if (Serial && debugLevel >= Debug::_LOW) Serial.println ("\n-------------------- MD25.Turn(" + String(angle) + ", " + String(speed) + ") --------------------");
                
                if (angle == 0) return false;  // if there's no turn angle then just quit out.
                if (speed <= 0) return false;  // if for some reason the speed is less than zero, exits the function.
                resetEncoders ();
                
                float turnCircumference = abs(angle * DEG2RAD * turnRadius);  // calculates the circumference of the arc each wheel must traverse.
                
                int leftSpeed = speed * (angle / abs(angle));  // sets the speed of the motor, and changes the direction based on "angle".
                int rightSpeed = -1 * speed * (angle / abs(angle));  // note how for the <Turn()> function, a negative speed won't change the direction.
                
                int dist = averageDistance ();
                while (dist < turnCircumference)  //repeats this code until the arc circumference for the turn has been travelled by the motors.
                {
                    if (exitCondition ())
                    {
                        stopMotors ();
                        return true;
                    }
                    
                    if (interruptCondition ())
                    {
                        stopMotors ();
                        if (exitCondition ()) return true;
                    }
                    else
                    {
                        float falloffMultiplier = falloff(dist/turnCircumference);
                        if (Serial && debugLevel >= Debug::_MEDIUM && falloffMultiplier < 1) Serial.println("\tFalloff\t\t->\t" + String(falloffMultiplier * 100) + "%.");
                        
                        setMotorSpeed (Motor::_LEFT, leftSpeed * falloffMultiplier);  // sets the speed and direction of the left-hand motor.
                        setMotorSpeed (Motor::_RIGHT, rightSpeed * falloffMultiplier);  // sets the speed and direction of the right-hand motor.
                    }
                    
                    dist = averageDistance ();
                }
                
                stopMotors ();  // stops both of the motors. 
				
				if (Serial && debugLevel >= Debug::_MEDIUM) Serial.println("\tOvershoot\t->\t" + String(averageDistance() - turnCircumference) + " mm.");
				
                return false;
            }
        //}
        
        //Arc//{
            void MD25::Arc (int arcRadius, int angle)
            {
                AdvancedArc (arcRadius, angle, [] () -> bool {return false;}, [] () -> bool {return false;}, defaultSpeed, falloffFunctions::Constant);
            }
            void MD25::Arc (int arcRadius, int angle, float (*falloff)(float f))
            {
                AdvancedArc (arcRadius, angle, [] () -> bool {return false;}, [] () -> bool {return false;}, defaultSpeed, falloff);
            }
            void MD25::Arc (int arcRadius, int angle, int speed)
            {
                AdvancedArc (arcRadius, angle, [] () -> bool {return false;}, [] () -> bool {return false;}, speed, falloffFunctions::Constant);
            }
            void MD25::Arc (int arcRadius, int angle, int speed, float (*falloff)(float f))
            {
                AdvancedArc (arcRadius, angle, [] () -> bool {return false;}, [] () -> bool {return false;}, speed, falloff);
            }
            
            bool MD25::AdvancedArc (int arcRadius, int angle, bool (*interruptCondition)(), bool (*exitCondition)())
            {
                return AdvancedArc (arcRadius, angle, interruptCondition, exitCondition, defaultSpeed, falloffFunctions::Constant);
            }
            bool MD25::AdvancedArc (int arcRadius, int angle, bool (*interruptCondition)(), bool (*exitCondition)(), float (*falloff)(float f))
            {
                return AdvancedArc (arcRadius, angle, interruptCondition, exitCondition, defaultSpeed, falloff);
            }
            bool MD25::AdvancedArc (int arcRadius, int angle, bool (*interruptCondition)(), bool (*exitCondition)(), int speed)
            {
                return AdvancedArc (arcRadius, angle, interruptCondition, exitCondition, speed, falloffFunctions::Constant);
            }
            bool MD25::AdvancedArc (int arcRadius, int angle, bool (*interruptCondition)(), bool (*exitCondition)(), int speed, float (*falloff)(float f))
            {
                /*Docstring
                    params:
                        - int arcRadius: the radius of the arc the MD25 robot is to traverse.
                        - int angle: the angle of the arc. The robot will go round in circles for values about 360! :D
                        - bool* interruptCondition: a bool function that will temporarily pause
                          execution of the function if true.
                        - bool* exitCondition: a bool function that will exit from the function
                          with a return value of true if true.
                        - bool doCorrection: whether or not to do a slower correction turn after it first executed the manoeuvre.
                        - int speed: the speed at which the robot will move forwards.
                        - float* falloff: a function that accepts a float (0 <= f <= 1) as a fraction of the function to completion.
                          The return value will then scale the speed accordingly.
                    returns:
                        - (bool) true: exitCondition returns true at any point in the function.
                        - (bool) false: exitCondition never returns false.
                    desc:
                        Moves the robot forwards of backwards in an arc
                          defined by "arcRadius" and "angle" at the given speed.
                          The speed is scaled over the length of the function by the return of "falloff".
                    
                        If "interruptCondition" returns true, then the
                          robot will pause movement until it returns false.
                    
                        If "exitCondition" returns true then the robot
                          will stop moving, and the function will exit with
                          a return value of "true". Returns "false" if the
                          exit condition is not reached.
                    techDesc:
                        Uses a loop to continuously set the speeds of the
                          left and right motors, where the outter wheel of the arc
                          will be faster than the given speed, and the inner wheel
                          slower (this is of course constrained to not exceed 128
                          for either wheel).
                    
                        The speed is then multiplied by the
                          falloff function, which accepts a float 0->1, the ratio
                          of the travelled distance over the required distance, and
                          returns a float 0->1 that the speed is scaled by. This
                          helps to reduce overshoot if a "good" function
                          is designed and implemented.
                    
                        It is worth noting that the speeds are continuously set
                          because the md25 will stop the motors after no signal
                          is sent for two seconds as a safety feature.
                    
                        The loop is terminated once the average distance
                          traversed by each wheel is greater than or equal to
                          the length of the arc that each wheel must trace, given by
                          the arcRadius and the angle to traverse (e.g. centreline distance
                          between the two wheels), and is calculated by <averageDistance()>.
                    
                        Within the loop, <exitCondition()> is checked, and the function
                          exits with a return value of "true" (to notify the caller) if
                          <exitCondition()> returns true.
                    
                        If <interruptCondition()> returns "true" then the motors are
                          stopped until it returns "false". In addition, <exitCondition()>
                          is still checked during the pause.
                */
                
                if (Serial && debugLevel >= Debug::_LOW) Serial.println ("\n-------------------- MD25.Arc(" + String(arcRadius) + ", " + String(angle) + ", " + String(speed) + ") --------------------");
                
                if (angle == 0) return false;  // if there's no turn angle then just quit out.
                if (arcRadius <= 0) return false;  // if for some reason the speed or radius is less than zero, exits the function.
                resetEncoders ();  // resets the motor encoders.
                
                int leftSpeed = speed * (1 + ((float)angle / abs(angle) * (turnRadius / (float)arcRadius)));  // calculates the speed and direction of the left-hand motor.
                int rightSpeed = speed * (1 - ((float)angle / abs(angle)* (turnRadius / (float)arcRadius)));  // note how again the speed does not affect direction, but movementDirection does.
                
                if (leftSpeed > 128)  // checks that the speed of the left wheel isn't too high.
                {
                    rightSpeed *= (128 / leftSpeed);  // decrease the rightSpeed by the same factor we're decreasing the leftSpeed by.
                    leftSpeed = 128;  // essentially multiplying by the same value as above.
                }
                else if (leftSpeed < -128)
                {
                    rightSpeed *= (-128 / leftSpeed);  // this code assumes that if one motor is at a negative speed then the other is too, or that if it's positive the other is also.
                    leftSpeed = -128;
                }
                if (rightSpeed > 128)  // checks that the speed of the left wheel isn't too high.
                {
                    leftSpeed *= (128 / rightSpeed);
                    rightSpeed = 128;
                }
                else if (rightSpeed < -128)
                {
                    leftSpeed *= (-128 / rightSpeed);
                    rightSpeed = -128;
                }
                
                float arcCircumference = abs(angle * DEG2RAD * arcRadius);  // calculates the circumference of the arc that must be traversed.
                if (Serial && debugLevel >= Debug::_MEDIUM) Serial.println ("\tArc Length\t->\t" + String(arcCircumference) + " mm.");
                
                int dist = averageDistance ();
                while (dist < arcCircumference)  //repeats this code until the arc circumference for the turn has been travelled by the motors.
                {
                    if (exitCondition ())
                    {
                        stopMotors ();
                        return true;
                    }
                    
                    if (interruptCondition ())
                    {
                        stopMotors ();
                        if (exitCondition ()) return true;
                    }
                    else
                    {
                        float falloffMultiplier = falloff(dist/arcCircumference);
                        if (Serial && debugLevel >= Debug::_MEDIUM && falloffMultiplier < 1) Serial.println("\tFalloff\t\t->\t" + String(falloffMultiplier * 100) + "%.");
                        
                        setMotorSpeed (Motor::_LEFT, leftSpeed * falloffMultiplier);  // sets the speed and direction of the left-hand motor.
                        setMotorSpeed (Motor::_RIGHT, rightSpeed * falloffMultiplier);  // sets the speed and direction of the right-hand motor.
                    }
                    
                    dist = averageDistance ();
                }
                
                stopMotors ();  // stops both of the motors.
				
				if (Serial && debugLevel >= Debug::_MEDIUM) Serial.println("\tOvershoot\t->\t" + String(averageDistance() - arcCircumference) + " mm.");
				
                return false;
            }
        //}
        
        float MD25::GetBatteryVoltage()
        {
            Wire.beginTransmission(address);  // begins a transmission to the MD25.
            Wire.write(VOLTREAD);  // sends the byte to request the battery voltage from the MD25.
            int errCode = Wire.endTransmission();  // ends the transmission to the MD25.
            
            Wire.requestFrom(address, 1);  // requests a single byte from the MD25.
            while(Wire.available() < 1);  // waits until the one requested byte is in the Arduino's buffer.
            int batteryVolts = Wire.read();  // reads the byte from the I2C buffer.
            
            if (Serial && debugLevel >= Debug::_LOW)  // checks whether there is an established Serial connection.
            {
                Serial.print ("Battery voltage = ");  // prints some text to the Serial monitor.
                Serial.print (batteryVolts / 10, DEC);  // prints the whole part of the voltage.
                Serial.print (".");  // prints a decimal point
                Serial.println (batteryVolts % 10, DEC);  // prints the fraction part of the voltage.
            }
			
			return batteryVolts / 10.0;  //returns the battery voltage.
        }
    //}
    
    //UTILITY FUNCTIONS//{
        void MD25::setMotorSpeed (Motor::Side motor, int speed)  // sets the speed of the specified motor to the given speed.
        {
            speed = constrain(speed + SPEEDOFFSET, 0, 255);  // adds the SPEEDOFFSET to speed so it is suitable for the MD25, and then constrains it to the correct range.
            Wire.beginTransmission (address);
            Wire.write(motor);
            Wire.write(speed * motorSwap);
            int errCode = Wire.endTransmission();
        }
        void MD25::stopMotors ()  // stops both of the motors. More efficient than just specifying zero speed.
        {
            setMotorSpeed (Motor::_LEFT, 0);  // sets the speed of the left-hand motor to zero.
            setMotorSpeed (Motor::_RIGHT, 0);  // sets the speed of the right-hand motor to zero.
            delay (300);
        }
        
        int MD25::averageDistance ()  // gets the average distance travelled by both wheels since the encoders were last reset
        {
            int distL = getDistance (Encoder::_LEFT);  // gets the distance travelled by the left-hand motor.
            int distR = getDistance (Encoder::_RIGHT);  // gets the distance travelled by the right-hand motor.
            int avgDist = (distL + distR) / 2.0;  // the mean of the two distances.
            
            return avgDist;
        }
        int MD25::getDistance (Encoder::Side encoder)  // gets the distance travelled by the specified wheel (in mm).
        {
            long encoderDist = abs(getEncoder (encoder));
            int dist = encoderDist * wheelCircumference / 360;
            
            return dist;
        }
        long MD25::getEncoder (Encoder::Side encoder)  // returns the number of degrees rotated by the specified wheel since the encoder was last reset.
        {
            Wire.beginTransmission(address);  // begins an I2C transmission to the MD25.
            Wire.write(encoder);
            int errCode = Wire.endTransmission();  // ends the transmission to the MD25.
            
            Wire.requestFrom(address, 4);  // request 4 bytes from MD25.
            while(Wire.available() < 4);  // waits until the four bytes are in the Arduino's I2C buffer.
            
            long angleTurned = Wire.read();  // read the first byte into the variable andleTurned.
            angleTurned <<= 8;
            angleTurned += Wire.read();  // read the second.
            angleTurned <<= 8;
            angleTurned += Wire.read();  // read the third byte.
            angleTurned <<= 8;
            angleTurned  += Wire.read();  // read the fourth byte.
            
            return angleTurned;  // returns the absolute number of degrees rotated by the specified motor.
        }
        void MD25::resetEncoders ()  // resets both encoders.
        {
            Wire.beginTransmission (address);  // begins an I2C transmission to the MD25.
            Wire.write(CMD);  // tells the MD25 we're sending it a command value.
            Wire.write(RST);  // write the value 0x20 to the MD25 to reset encoders.
            int errCode = Wire.endTransmission();
        }
        
        int MD25::setAcceleration ()  // sends the current acceleration to the MD25.
        {	
            Wire.beginTransmission(address);
            Wire.write(ACCELERATION);
            Wire.write(acceleration);
            int errCode = Wire.endTransmission();
            
            return errCode;
        }
        int MD25::setAddress (I2C::Address n_address)  // sets a new address for the connected MD25
        {
            Wire.beginTransmission(address);
            Wire.write(0xA0);
            Wire.write(0xAA);
            Wire.write(0xA5);
            Wire.write(n_address);
            int errCode = Wire.endTransmission();
            
            address = n_address;
            
            return errCode;
        }
    //}
    
    //falloff FUNCTION LIBRARY//{
        float falloffFunctions::Constant (float f)
        {
            return 1;
        }
        float falloffFunctions::Linear_10percent (float f)
        {
            float v = 1.0 - (0.9 * f);
            return v;
        }
        float falloffFunctions::Final10percent_linear_10percent (float f)
        {
            if (f < 0.9) return 1;
            
            float v = 1.0 - (9 * (f - 0.9));
            return v;
        }
    //}
}