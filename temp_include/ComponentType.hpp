#pragma once 

#include "MyVector.hpp"

//탱크의 외형 수치. 

struct TankApperance{
    MyRGB bodyColor, turretColor, barrelColor; 
    MyVector<float> bodySize, turretSize, barrelSize; 
}; 