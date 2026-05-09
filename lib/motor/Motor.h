#pragma once
#include <stdint.h>

class Motor {
public:
    virtual void setSpeed(double rads) = 0;
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual double getCurrentSpeed() = 0;
    virtual double getPosition() = 0;
};
