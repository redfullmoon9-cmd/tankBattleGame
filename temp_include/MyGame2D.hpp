#pragma once 
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include "MyVector.hpp"
#include "InputManager.hpp"


namespace ref{
    // #include "MyTimer.hpp"
    /**
     * 2025.12.29 ~ 12.31
     * and 2026. 1. 1
     * 1.9 그리기 함수 모두 render.hpp 로 이동. 게임객체 클래스 작성할 때 유연하도록. 
     * MyGame2D 클래스는 사용자 입력등을 담당하고 그리기는 component 방식으로 처리 하도록. 
     */
    using myVec= MyVector<float>; 
    
    class MyGame2D{
    private: 
        int width, height;  
        std::string title; 
        GLFWwindow* glfwWindow=nullptr; 
        std::map<int, bool> key_status; 
        std::map<int, bool> key_prev_status; //1. 5 키입력 수정. 
        MyRGB color; 
        float spf =1.0f / 60.0f; //1.4 60초의 1프레임. 
        bool draw_grid =true; //배경에 그리드 그리기 
        
        protected:
        MyRGB bgColor; 
        InputManager inputManager;     
        
    public : 
            MyGame2D();
            // MyGame2D(const std::string& title, const int& width, const int& height);
            MyGame2D(const std::string title, const int& width, const int& height, const MyRGB color);
            ~MyGame2D();
    
            MyGame2D& init(); 
            MyGame2D& init(const std::string& title, const int& width, const int& height, const MyRGB& bgColor); 
            void reportError(const std::string& funtionName, const std::string& message); 
            void run(); 
            void run(MyRGB color); 
            bool isKeyPressed(const int& key); 
            bool isKeyJustPressed(const int& key); //1.5 add 키 입력 한번 만.. 
            bool isKeyPressedAndReleased(const int& key); //1.5 
            // virtual void update(){} 
            virtual void update(const float& dt){}; 
            const MyRGB& getColor() const; //1.4 추가 
            void setColor(const MyRGB& newColor); 
            //spf값 구하기 세팅하기 
            float getTimeStamp(); 
            void setTimeStamp(const float newSpf); 
            InputManager* getInputManager(); 
          
    };  

}


