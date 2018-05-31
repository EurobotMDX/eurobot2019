#include <Wire.h>
#include <Enums.MD25.h>
#include <MD25.h>

/*
 * Demonstrates how to perform an arc.
 * 
 * Note that generally there is little
 * use for an arc as the Turn() function
 * is more reliable. It just looks pretty.
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
  md25.Arc (1000, 90);  // move in an arc with radius 1000mm (1m) an angle of 90 degrees

  while (true);  // stops the loop
}
