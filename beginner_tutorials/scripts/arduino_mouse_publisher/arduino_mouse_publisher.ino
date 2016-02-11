/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>

#include <ps2.h>
#define WProgram.h Arduino.h

const int dataPin = 5;
const int clockPin = 6;

const int mouseRangeX = 3779; // the maximum range of x/y values
const int mouseRangeY = 1889;

char x;
char y;
byte status;

int xPosition = 0; // values incremented and decremented when mouse moves
int yPosition = 0;
int xBrightness = 128; // values increased and decreased based on mouse position
int yBrightness = 128;

const byte REQUEST_DATA = 0xeb; // command to get data from the mouse

PS2 mouse(clockPin, dataPin);

ros::NodeHandle  nh;

String readMouse(){
    // get a reading from the mouse
  mouse.write(REQUEST_DATA); // ask the mouse for data
  mouse.read(); // ignore ack
  status = mouse.read(); // read the mouse buttons
  if (status & 1) // this bit is set if the left mouse btn pressed
    xPosition = 0; // center the mouse x position
  if (status & 2) // this bit is set if the right mouse btn pressed
    yPosition = 0; // center the mouse y position
  x = mouse.read();
  y = mouse.read();
  if ( x != 0 || y != 0)
  {
    // here if there is mouse movement
    xPosition = xPosition + x; // accumulate the position
    xPosition = constrain(xPosition, -mouseRangeX, mouseRangeX);
    yPosition = constrain(yPosition + y, -mouseRangeY, mouseRangeY);
    
    xBrightness = map(xPosition, -mouseRangeX, mouseRangeX, 0, 255);
    yBrightness = map(yPosition, -mouseRangeY, mouseRangeY, 0, 255);
    
    String output = "X: " + String(xPosition) + " Y: " + String(yPosition);
    
//    Serial.println("X: " + String(xPosition) + " Y: " + String(yPosition));

//    char output[13] = 
    
    return output;
    
  }
  return "";
}

char mouseBegin()
{
  // reset and initialize the mouse
  //198 | Chapter 6: Getting Input from Sensors
  mouse.write(0xff); // reset
  delayMicroseconds(100);
  mouse.read(); // ack byte
  mouse.read(); // blank
  mouse.read(); // blank
  mouse.write(0xf0); // remote mode
  mouse.read(); // ack
  delayMicroseconds(100);
}

void messageCb( const std_msgs::Empty& toggle_msg){
  digitalWrite(13, HIGH-digitalRead(13));   // blink the led
}

ros::Subscriber<std_msgs::Empty> sub("toggle_led", messageCb );

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

char hello[13] = "hello world!";

char out[15];

void setup()
{
  mouseBegin();
  pinMode(13, OUTPUT);
  nh.initNode();
  nh.advertise(chatter);
  nh.subscribe(sub);
}

void loop()
{
  readMouse().toCharArray(out, 15);
  
  str_msg.data = out;
  chatter.publish( &str_msg );
  nh.spinOnce();
  delay(500);
}
