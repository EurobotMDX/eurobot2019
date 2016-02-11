#include <Wire.h>
#include <Enums.MD25.h>
#include <MD25.h>

/*
 * This demonstrates the most basic function: going forwards.
 * 
 * Use this to calibrate the WheelCircumference value by
 * moving the robot forwards over a known distance. If it needs
 * to go further, decrease the first value in the MD25 constructor
 * and vice verse for overshoot.
 */

using namespace RD02;  // everything is kept in here to prevent possible clashes
                // if you don't understand namespaces, just put this in then forget about it <3

MD25 md25;  // make and empty MD25 object

void setup()
{
  md25 = MD25(102 * PI, 278/2);  // define the MD25 object
                // first parameter is the wheel circumference used (wheel diameter * PI)
                // second parameter is the robot's turn radius (half the distance between each wheel)
}

void loop()
{
  delay (2000);  // a delay before it moves, so you've got time to turn it on, then position it
  md25.Forward (1000);  // move forwards 1000mm (1m)

  while (true);  // stops the loop
}