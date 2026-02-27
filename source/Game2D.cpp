#include "Game2D.hpp"


Game2D::Game2D(const std::string &_title, const int &_width, const int &_height, const std::vector<float> _color)
: title(_title), width(_width), height(_height), color(_color)
{
    
}
/** 모든 리소스 해제, 버텍스 어래이, 버택스 버퍼, 원도우등...  */
Game2D::~Game2D()
{
    if(window){
        if(basicShader) delete basicShader; 
        glfwDestroyWindow(window); 
    }
    glfwTerminate(); 
    
}

Game2D &Game2D::init()
{
    return init(this->title, this->width, this->height, this->color); 
}

Game2D &Game2D::init(const std::string &title, const int &width, const int &height, std::vector<float> vec)
{
    // std::cout<<" in Game2D init " <<std::endl; 
    SPDLOG_INFO("Game2D init"); 
    
    if(window != nullptr ){
        reportError(__FUNCTION__, "Aleady window initialized"); 
        return *this; 
    }

    if(!glfwInit()){
        reportError(__FUNCTION__, "glfwinit Failed"); 
        // glfwTerminate(); 
        exit(-1); 
    }
    
    // OpenGL 버전 설정 (Core Profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window =glfwCreateWindow(width, height, title.c_str(), NULL, NULL); 
    if(!window){
        reportError(__FUNCTION__, "Window Create Failed "); 
        glfwTerminate(); 
        exit(-1); 
        
    }
    //키보드 이벤트 핸들러 초기화. here 
    
    glfwMakeContextCurrent(window); 
    
    //프로세스 어드레스를 얻어 오는 함수. openGL 함수를 로딩함. 
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        reportError(__FUNCTION__ , "GLAD initial failed"); 
    }
    
    auto glVersion = glGetString(GL_VERSION); 
    SPDLOG_INFO("glVersion: {}", reinterpret_cast<const char*>(glVersion)); 
    
    int displayWidth, displayHeight; 
    glfwGetFramebufferSize(window, &displayWidth, &displayHeight); 
    
    //그림을 그릴 화면의 위치 및 크기설정,  init에서 한번 하는 것으로 
    glViewport(0, 0, displayWidth, displayHeight); 

    OnInit(); //하위 클래스 openGL init 호출 하는 구조. 
    
    //shaders경로에 있는 버택스와 프래그먼트 정보. 
    basicShader = new Shader("shaders/vertex_shader.vs", "shaders/fragment_shader.fs"); 
    SPDLOG_INFO( "Shader create :{}", (basicShader ? "OK " :"FAIL")); 

    // VAO, VBO 관련 openGL함수들 이동 --> tankGame으로 
    
    return *this; 
    
}

Game2D &Game2D::run()
{
    if(!window){
        reportError(__FUNCTION__, "Window is null. Call init() first.");
        return *this; 
    }
    SPDLOG_INFO(" Game 2d run"); 
    
    while(!glfwWindowShouldClose(window)){

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //지울컬러를 세팅. 
        glClear(GL_COLOR_BUFFER_BIT);// 컬러버퍼를 지움. 
        
        //상속받을 클래스의 업데이트 로직. 여기에 
        OnRender(); // 하위 클래스의 상속받을 클래스에서 구현부 호출. 

        glfwSwapBuffers(window); 
        glfwPollEvents(); 
    }
    return *this; //없어서 미정의 동작 했다고. 
}

void Game2D::reportError(const std::string &functionName, const std::string &message)
{
    std::cout<<"ERROR " << functionName << " " << message; 
    // glfwTerminate(); 
    // exit(-1); 
    
}

//26. 02, 27 template method pattern 
void Game2D::OnInit()
{ 
    //virtual function -- 하위 클래스 메서드 호출. 
}
//template method pattern 
void Game2D::OnRender()
{
    //virtual function -- 하위 클래스 메서드 호출. 
}
