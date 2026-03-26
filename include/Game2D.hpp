#ifndef GAME2D_HPP
#define GAME2D_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "MyVector.hpp"
#include "kshader.h"
#include "kprogram.h"
// #include "Shader.hpp"


/**26.02. 21 add */
class Game2D{
    private:
        int width, height; 
        std::string title; 
        std::vector<float> color; 
    protected:
        GLFWwindow* window=nullptr; 
        // Shader* basicShader=nullptr; //쉐이더 설정. 
        uint32_t m_shader{0}; 
        uint32_t m_program{0}; 
        std::unique_ptr<KProgram> m_programPtr=nullptr; 
        std::shared_ptr<KShader>  m_vertexShaderPtr=nullptr; 
        std::shared_ptr<KShader>  m_fragmentShaderPtr=nullptr; 

        virtual void OnInit(); //Template method pattern 구현.  
        virtual void OnRender(); //Template method pattern 구현.        

    public:
        Game2D()=default; 
        Game2D(const std::string& title, const int& width, const int& height, const std::vector<float> color ); 
        virtual ~Game2D(); //virtual 로 선언되지 않으면 상속되는 하위 destructor만 호출 되어서 위험. 

        Game2D& init(); 
        Game2D& init(const std::string& title, const int& width, const int& height, std::vector<float> vec); 
        Game2D& run(); 
        void reportError(const std::string& functionName, const std::string& message); 

        // Shader* getShader(){return basicShader; }
        // std::unique_ptr<KShader> getShader() { return m_shaderPtr; }
};  

#endif