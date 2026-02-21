#ifndef GAME2D_HPP
#define GAME2D_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>
#include "MyVector.hpp"
#include "Shader.hpp"

/**26.02. 21 add */
class Game2D{
    private:
        int width, height; 
        std::string title; 
        std::vector<float> color; 
        GLFWwindow* window=nullptr; 
        Shader* shader=nullptr; //쉐이더   설정. 
        unsigned int VBO, VAO; 

    public:
        Game2D()=default; 
        Game2D(const std::string& title, const int& width, const int& height, const std::vector<float> color ); 
        ~Game2D(); 

        Game2D& init(); 
        Game2D& init(const std::string& title, const int& width, const int& height, std::vector<float> vec); 
        Game2D& run(); 
        void reportError(const std::string& functionName, const std::string& message); 

        virtual void Update(); 
        virtual void Render(); 
};  

#endif