#include <Wire.h>
#include <Enums.MD25.h>
#include <MD25.h>

/*
 * Demonstrates use of the GetBatteryVoltage function.
 * 
 * This can be used just to check your battery's charge
 * (given in volts), or is a useful tool for verifying
 * the I2C connection between Arduino and MD25.
 */

using namespace RD02;  // everything is kept in here to prevent possible clashes
                // if you don't understand namespaces, just put this in then forget about it <3
                
MD25 md25;  // make and empty MD25 object

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);  // open the Serial monitor to debug the voltage to the Serial monitor
  md25 = MD25();  // since we're just reading the battery voltage, we don't need to specify wheel sizes etc.
}

void loop()
{
  // put your main code here, to run repeatedly:
  md25.GetBatteryVoltage ();  // read the battery voltage (automatically prints to the Serial monitor if a Serial connection has begun
  delay (500);  // pause for half a second
}
