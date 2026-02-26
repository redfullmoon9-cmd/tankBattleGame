#ifndef GAME2D_HPP
#define GAME2D_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>
#include "MyVector.hpp"
#include "Shader.hpp" // Shader 클래스 포함
namespace refcode{

    class Game2D {
    private:
        int width, height;
        std::string title;
        std::vector<float> color;
        GLFWwindow* window = nullptr;
    
        // 현대적 OpenGL을 위한 추가 멤버
        Shader* basicShader = nullptr;
        unsigned int VAO, VBO; 
    
    public:
        Game2D() = default;
        Game2D(const std::string& title, const int& width, const int& height, const std::vector<float> color);
        ~Game2D();
    
        Game2D& init();
        // 매개변수가 있는 init에서 기본 설정을 수행하도록 개선
        Game2D& init(const std::string& title, const int& width, const int& height, std::vector<float> vec);
        Game2D& run();
        
        void reportError(const std::string& functionName, const std::string& message);
        
        // 렌더링을 담당할 가상 함수 (나중에 상속받아 탱크 등을 그릴 때 사용)
        virtual void update() {}
        virtual void render() {}
    };
    
}
#endif