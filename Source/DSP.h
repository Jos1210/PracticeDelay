//DSP LIBRARY

#pragma once
#include <cmath>
#include <numbers>

inline void panningEqualPower(float panning, float &left, float &right){    //Inline permite definir la función
                                                                            //Dentro del header
    float piDividedFour = (float)std::numbers::pi * 0.25f;
    float x =  piDividedFour* (panning + 1.0f);
    left = std::cos(x);
    right = std::sin(x);
}

