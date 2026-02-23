#pragma once 
#include <string>
#include <map>
#include <vector>
#include <GLFW/glfw3.h>

/* 26. 01.09 새로 추가해서 이전 키 입력 대체 하기. */
enum class InputType{
    KeyPressed, 
    KeyReleased, 
    KeyHeld
}; 

struct InputBinding{
    int key;
    InputType type;  
};


class InputManager{
private:
    //action 이름과 키 바인딩. 
    std::map<std::string, std::vector<InputBinding>> actionBindings; 

    //현재와 이전의 키 프레임 상태. 
    std::map<int, bool> currentKeyStatus; 
    std::map<int, bool> previousKeyStatus; 

    GLFWwindow* window; 

public: 
    InputManager()=default; 
    InputManager(GLFWwindow* window); 
    ~InputManager(); 
    void SetupInputBindings(); //키 바인딩. 
    void BindAction(const std::string& actionName, int key, InputType type= InputType::KeyHeld ); 
    void Update(); 
    bool isActionActive(const std::string& actionName); 
    float GetAxisValue(const std::string& axisName); 
}; 