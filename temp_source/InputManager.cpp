#include "InputManager.hpp"

InputManager::InputManager(GLFWwindow* win) :window(win){ }
InputManager::~InputManager() { }

void InputManager::BindAction(const std::string& actionName, int key, InputType type){
    actionBindings[actionName].push_back({key, type}); 
}

void InputManager::Update(){
    previousKeyStatus = currentKeyStatus; 
}

// 액션이 활성화되었는지 확인
bool InputManager::isActionActive(const std::string& actionName)
{
    if(actionBindings.find(actionName) == actionBindings.end()) return false;

    for(const auto& binding : actionBindings[actionName]){
        bool isPressed = glfwGetKey(window, binding.key) == GLFW_PRESS; 
        bool wasPressed = previousKeyStatus[binding.key]; 

        currentKeyStatus[binding.key] =isPressed; 
        switch (binding.type){
        case InputType::KeyHeld:
            if(isPressed) return true; 
            break;
        case InputType::KeyPressed:
            if(isPressed && !wasPressed) return true; 
            break; 
        case InputType::KeyReleased:
            if(!isPressed && wasPressed) return true; 
            break; 
        }
    }
    return false; 
}

float InputManager::GetAxisValue(const std::string &axisName)
{   
    float value =0.0f; 
    if(isActionActive(axisName+"_Positive" )) value += 1.0f; 
    if(isActionActive(axisName+"_Negative")) value -= 1.0f; 
    return value;
}

//GLFW의 키 값을 바인딩 한다. 
void InputManager::SetupInputBindings() {
    BindAction("MoveHorizontal_Positive", GLFW_KEY_D); 
    BindAction("MoveHorizontal_Positive", GLFW_KEY_RIGHT); 
    BindAction("MoveHorizontal_Negative", GLFW_KEY_A);
    BindAction("MoveHorizontal_Negative", GLFW_KEY_LEFT);
    
    BindAction("MoveVertical_Negative", GLFW_KEY_W); 
    BindAction("MoveVertical_Negative", GLFW_KEY_UP); 
    BindAction("MoveVertical_Positive", GLFW_KEY_S); 
    BindAction("MoveVertical_Positive", GLFW_KEY_DOWN); 

    BindAction("Jump", GLFW_KEY_SPACE, InputType::KeyPressed); 
    BindAction("Attack", GLFW_KEY_J, InputType::KeyPressed); 
    BindAction("Dash", GLFW_KEY_LEFT_SHIFT, InputType::KeyHeld); 
    BindAction("Escape", GLFW_KEY_ESCAPE, InputType::KeyPressed); 

}

