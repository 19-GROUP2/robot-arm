#include <VarSpeedServo.h>

class CustomServo : public VarSpeedServo
{
private:
    /* data */
    int period = 100;
    int speed = 20;
    unsigned long lastMoveMillis = 0;
    int currPos = 90;

public:
    CustomServo();
    ~CustomServo();
    void init(int pin, int _period = 30, int _speed = 30, int _pos = 90);
    bool move(int pos);
};

CustomServo::CustomServo()
{
}

CustomServo::~CustomServo()
{
}

void CustomServo::init(int pin, int _period, int _speed, int _pos)
{
    period = _period;
    speed = _speed;
    currPos = _pos;
    VarSpeedServo::attach(pin);
}

bool CustomServo::move(int pos)
{
    unsigned long currentMoveMillis = millis();
    if ((currentMoveMillis - lastMoveMillis) < period)
        return pos != currPos;

    lastMoveMillis = currentMoveMillis;

    if (currPos > pos)
    {
        currPos--;
        VarSpeedServo::write(currPos, speed, false);
        return true;
    }
    else if (currPos < pos)
    {
        currPos++;
        VarSpeedServo::write(currPos, speed, false);
        return true;
    }
    else
    {
        return false;
    }
}
