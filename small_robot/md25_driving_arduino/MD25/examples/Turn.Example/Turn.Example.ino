#include <Wire.h>
#include <Enums.MD25.h>
#include <MD25.h>

/*
 * This demonstrates how to turn your MD25 robot.
 * 
 * Once you've used the Forward example to calibrate
 * your WheelCircumference value, use this to tweak
 * your TurnRadius value so that your robot's movement
 * is as accurate as possible. Increase TurnCircumference
 * for undershoot and vice versa for overshoot.
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
  md25.Turn (1080);  // do 3 complete revolutions (1080 degrees) on the spot.

  while (true);  // stops the loop
}