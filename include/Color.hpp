#ifndef COLOR_HPP
#define COLOR_HPP

#include "MyVector.hpp"

namespace tp{
    class Color : public Vector3<float> {
        private:
            float r, g, b; 

        public:
            Color(); 
            Color(const float& r, const float& g, const float& b):r(r), g(g), b(b){}; 
            ~Color(); 

    }; 

    class RGB{

    }; 
} 

#endif