#include "Game2D.hpp"
/** 26.02.21 add */

Game2D::Game2D(const std::string &_title, const int &_width, const int &_height, const std::vector<float> _color) 
: title(_title), width(_width), height(_height), color(_color)
{
    
}

Game2D::~Game2D()
{
}

Game2D &Game2D::init()
{
    // TODO: insert return statement here
}

Game2D &Game2D::init(const std::string &title, const int &width, const int &height, std::vector<float> vec)
{
    std::cout<<" in Game2D init " <<std::endl; 

    if(!window){
        reportError(__FUNCTION__, "Aleady window initial"); 
    }

    if(!glfwInit()){
        reportError(__FUNCTION__, "glfwinit Failed"); 
        // glfwTerminate(); 
    }

    //키보드 이벤트 핸들러 초기화. here 

    glfwMakeContextCurrent(window); 

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        reportError(__FUNCTION__ , "GLAD initial failed"); 
        // std::cout<<"GLAD initial failed " << std::endl; 
        // return -1; 
    }
    int displayWidth, displayHeight; 
    glfwGetFramebufferSize(window, &displayWidth, &displayHeight); 
    glViewport(0, 0, displayWidth, displayHeight); 
    

}

Game2D &Game2D::run()
{
    if(window == nullptr){
        init(title, width, height, color); 
    }
    // TODO: insert return statement here
}

void Game2D::reportError(const std::string &functionName, const std::string &message)
{
    std::cout<<"ERROR " << functionName << " " << message; 
    glfwTerminate(); 
    exit(-1); 

}
