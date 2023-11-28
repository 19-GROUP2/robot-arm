#include <VarSpeedServo.h>

String inputString = "";    // a String to hold incoming data
bool inputComplete = false; // whether the string is complete

const int SERVO_SPEED_MAX = 50; // maximum speed of the servos
const int SERVO_COUNT = 4; // maximum number of angles to load
const int servoPins[SERVO_COUNT] = {9, 10, 11, 12}; // pins for the servos
VarSpeedServo servos[SERVO_COUNT]; // array of VarSpeedServo objects
int angles[SERVO_COUNT];   // array to store the loaded angles
char sepChar = ' ';        // character used to separate the angles



void setup()
{ 
  Serial.begin(9600);  
  inputString.reserve(200);
  for (int i = 0; i < SERVO_COUNT; i++)
  {
    servos[i].attach(servoPins[i]);
    servos[i].write(0, SERVO_SPEED_MAX, false);
  }
}

void loop()
{
  if (inputComplete)
  {
    int startIndex = 0;
    int endIndex = inputString.indexOf(sepChar);
    for (int i = 0; i < SERVO_COUNT; i++)
    {
      if (endIndex == -1)
      {
        angles[i] = inputString.substring(startIndex).toInt();
        break;
      }
      else
      {
        angles[i] = inputString.substring(startIndex, endIndex).toInt();
        startIndex = endIndex + 1;
        endIndex = inputString.indexOf(sepChar, startIndex);
      }
    }

    for (int i = 0; i < SERVO_COUNT; i++)
    {
      Serial.print("angle");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(angles[i]);
      servos[i].write(angles[i], SERVO_SPEED_MAX, false);
    }

    inputString = "";
    inputComplete = false;
  }
}

void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    if (inChar != '\n')
      inputString += inChar;
    else
      inputComplete = true;
  }
}

/*
12 258 69 1

*/