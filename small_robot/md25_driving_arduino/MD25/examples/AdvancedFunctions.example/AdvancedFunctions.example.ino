#include <Wire.h>
#include <Enums.MD25.h>
#include <MD25.h>

/*
 * Demonstrates the use of interrupts.
 * This will pause execution whilst an IR
 * range finder detects something within
 * 10cm, and end the program once 20seconds
 * has passed.
 * 
 * The first interrupt function passed to
 * an Advanced function will only pause
 * execution until as long as it is true.
 * 
 * The second interrupt function will end
 * the function, and will return TRUE, so
 * that you can then use that information
 * as required in your program.
 */

using namespace RD02;  // everything is kept in here to prevent possible clashes
                // if you don't understand namespaces, just put this in then forget about it <3

MD25 md25;  // make and empty MD25 object

long timer;  // to record the start time of the program.
int IR_frontPin = A0;
int IR_rightPin = A1;

void setup()
{
  md25 = MD25(102 * PI, 278/2);  // define the MD25 object
                // first parameter is the wheel circumference used (wheel diameter * PI)
                // second parameter is the robot's turn radius (half the distance between each wheel)
  timer = millis();
}

void loop ()
{
  delay(2000);  // 2 second delay for you to position or whatever
  if(md25.AdvancedForward(1000, checkFront, checkTime)) goto timeUp;  // move forwards 1m, checking there's nothing in front of us we may crash into
                                                                       // if the time limit has been exceeded, this will take us to the program end
  for(int i = 0; i < 50; i++)  // delays must be done like this if you don't want to exceed a time limit.
  {
    if (checkTime()) goto timeUp;
    delay (10);
  }

  if(md25.AdvancedArc(1000, 180, [] () -> bool {return checkFront || checkRight;}, checkTime)) goto timeUp;  // 1m radius arc, 180 degrees
                                                                                                            // check there's nothing in front of us OR to our right
                                                                                                            // if the time is up, stop and go to the end.
  timeUp:
  while(true);  // ends the program
}


//INTERRUPT FUNCTIONS//
bool checkFront()
{
  if (getDistance(IR_frontPin) < 100) return true;
  return false;
}
bool checkRight()
{
  if (getDistance(IR_rightPin) < 100) return true;
  return false;
}
bool checkTime()
{
  if (millis() - timer > 10000) return true;
  return false;
}

//UTILITY FUNCTION//
float getDistance(int sensorPin)  // does magic to get an IR range finder distance
{
  double distance = 123430.95 * pow(analogRead(sensorPin), -1.15);

  if (Serial) Serial.println("IR Sensor distance: " + String(distance));
  return distance;
}

  
