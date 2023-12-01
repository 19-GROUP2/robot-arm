#include <ServoEasing.hpp>

String inputString = "";    // a String to hold incoming data
bool inputComplete = false; // whether the string is complete

const int SERVO_SPEED_MAX = 10; // maximum speed of the servos
const int SERVO_COUNT = 5;
const int JOINT_COUNT = SERVO_COUNT - 1;
const int GRIPPER_INDEX = SERVO_COUNT - 1;           // maximum number of angles to load
const int servoPins[SERVO_COUNT] = {3, 5, 6, 9, 11}; // pins for the servos
ServoEasing servos[SERVO_COUNT];                   // array of VarSpeedServo objects
int angles[SERVO_COUNT];                             // array to store the loaded angles
char sepChar = ' ';                                  // character used to separate the angles

void setup()
{
  Serial.begin(9600);
  inputString.reserve(200);
  for (int i = 0; i < SERVO_COUNT; i++)
  {
    servos[i].attach(servoPins[i]);
    servos[i].easeTo(90, SERVO_SPEED_MAX);
  }
  delay(2000);
  Serial.println("Ready");
}

void loop()
{
  if (inputComplete)
  {    
    updateServos();
    inputString = "";
    inputComplete = false;
  }
}

void updateServos()
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

  for (int i = 0; i < JOINT_COUNT; i++)
  {
    Serial.print(angles[i]);
    Serial.print(' ');
    servos[i].easeTo(angles[i], SERVO_SPEED_MAX);
  } 

  bool wait = true;
  int time = 0;
  while (wait && time <5000)
  {
//    break;
    wait = false;
    for (int i = 0; i < JOINT_COUNT; i++)
    {
      if (servos[i].read() != angles[i])
        wait = true;
    }
    delay(50);
    time += 50;
  }

  servos[GRIPPER_INDEX].easeTo(angles[GRIPPER_INDEX], SERVO_SPEED_MAX);
  delay(1000);

  Serial.println();
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
