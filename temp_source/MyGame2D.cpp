// #include <iostream>
#include "temp_include/MyGame2D.hpp"
// #include "MyUtils.hpp"
#include "temp_include/Render.hpp"
/**
 * 원소스 Game2D.hpp cpp파일 직접 구현
 * 2025 12.29~ 12.31 * and 2026. 1.1 
 * 
 */

MyGame2D::MyGame2D() : title("test window"), width(800), height(640) {} 

//1.2 수정 glfwWindow 등 초기화 리스트에 추가. init 명시적 호출로 
MyGame2D::MyGame2D(const std::string title,  const int& width, const int& height, const MyRGB color) 
    : title(title), width(width), height(height), color(color),  glfwWindow(nullptr) { 
}

MyGame2D::~MyGame2D(){  
    glfwDestroyWindow(glfwWindow); 
}

//1.4 테스트 용 귀찮아서. 
MyGame2D& MyGame2D::init() {
    title="test"; 
    width=800; 
    height=640; 
    color=MyRGB(255, 0,0); 
    glfwWindow=nullptr; 

    return *this; 
 } 

//1,1
MyGame2D& MyGame2D::init(const std::string& title, const int& width, const int& height, const MyRGB& bgColor){
    std::cout << "in MyGame2D init()"<< std::endl; 
    //만약 nullptr이 아니면 어디선가에서 생성
    if(glfwWindow != nullptr){
        // std::cout << "SKIP SECOND INIT" << std::endl; 
        reportError(__FUNCTION__, " Aleady window initialed"); 
        return *this; 
    }

    //멤버변수의 지정. 1.2 
    this->title=title; 
    this->width=width; 
    this->height=height; 
    this->bgColor=bgColor; //MyVector 에서 대입 연산자 const 수정 
    

    //glfw 라이브러리 초기화 
    if(!glfwInit()){ 
        reportError(__FUNCTION__, " glfw Init failed "); 
    }; 

    //원도우 생성. 
    glfwWindow =glfwCreateWindow(width, height, title.c_str(), NULL, NULL); 
    if(!glfwWindow){
        reportError(__FUNCTION__, " glfw Window initial Failed"); 
        glfwTerminate(); 
    }
    //키보드 이벤트 핸들러 초기화. 
    inputManager =InputManager(glfwWindow); 
    inputManager.SetupInputBindings(); 

    glfwMakeContextCurrent(glfwWindow); 

    //1.5 추가 Vsync로 update간격 마추기 --화면 주사율에 따라 일정하게 
    // glfwSwapInterval(1); // vsync on 시간변화량 함수 추가 

    glewExperimental =true; 

    if(glewInit() !=GLEW_OK){
        reportError(__FUNCTION__, "glew libray Initial Failed "); 
        glfwDestroyWindow(glfwWindow); 
        glfwTerminate(); 
    }

    int displayWidth, displayHeight; 
    glfwGetFramebufferSize(glfwWindow, &displayWidth, &displayHeight); 
    glViewport(0,0, displayWidth, displayHeight); 
    glMatrixMode(GL_PROJECTION); //사용못함. 
    glLoadIdentity(); //사용못함. 

    float aspect_ratio= (float) displayWidth/(float)displayHeight; 
    glOrtho(-aspect_ratio, aspect_ratio, -1.0, 1.0, -1.0, 1.0); //사용못함. 

    glMatrixMode(GL_MODELVIEW); //1.2 추가 //사용못함. 
    glLoadIdentity(); //사용못함. 

    std::cout << "display width  "<< displayWidth << " display height " << displayHeight << std::endl;  

    return *this; 
}

/*1.1 작성. 
1.5 수정 glfwMakeContextCurrent,  glMatrixMode,  glPushMatrix추가 
init에서 했더라도 다시 pre draw개념으로 그 이후엔 post draw로 해 주어야. 
*/
void MyGame2D::run(){
    if(glfwWindow == nullptr) {
        init(title, width, height, color); 
    }
    
    // float dt=0.0f; 
    //1.6 add 시간 변화량 변수 초기화 
    //--> while루프가 너무 빨리 돌아 밀리초를 회전수에 적용하기 어렵다. 초단위의 double 값. 
    double lastTime =glfwGetTime(); 
    
    while(!glfwWindowShouldClose(glfwWindow)){
        double currentTime=glfwGetTime(); //1.6 add 
        float dt =static_cast<float>(currentTime- lastTime); //1.6 add 
        lastTime=currentTime; //1.6 add

        inputManager.Update(); //02.08

        //glfwPollEvents() 가 없으면 키 입력이 업데이트가 되지 않음. 
        // if(isKeyPressed(GLFW_KEY_ESCAPE)){
        //     std::cout << "main loop escapeed " <<std::endl; 
        //     break; 
        // }
        
        // timer.start(); //1.6 add 프레임 시작시 타이머 시작. --> 제대로 동작하지 않음. 주석처리. 
        

        glfwMakeContextCurrent(glfwWindow); //1.5 add
        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //1,1 추가 배경화면 정리 
        // glClearColor(1,1,1,1); //1.5 바탕화면을 화이트로 위쪽에서 
        glClearColor(bgColor.rgb[0],bgColor.rgb[1], bgColor.rgb[2],  1.0f); //1,1 추가 배경화면 정리 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //1.5
        // glClear(GL_COLOR_BUFFER_BIT); //1.1 추가 
        glMatrixMode(GL_MODELVIEW); //1.5

        glPushMatrix(); //1.5

        glColor3fv(color.rgb); //1.2 옮겨서 해봄. 컬러값 바뀌지 않음. 
        // drawMyGrid(MyRGB(255, 0, 0), 0.5f); 
        PrimitiveRenderer::drawMyGrid(MyRGB(128, 128, 128), 0.5f); //0.5 -> 0.4 수치변경. --> 다시변경. 디버깅 
        // 버추얼 함수로 구현 측에서 오버라이딩. 사용자 드로우 코드. 
        //1.6 dt라는 시간 변화량을 전달 하는 구조로 변경. 
        update(dt);

        glPopMatrix(); //1.5

        glfwSwapBuffers(glfwWindow); //1.1추가 
        glfwPollEvents(); //1.1 이것이 없어서 화면닫기가 않되었다고. isKeyPressed에서 키값 없데이트 않됨. 

        //1.6 루프끝에서 경과된 시간을 계산. 
        // dt=static_cast<float> (timer.stopAndGetElapsedMilli()); 

        //spf제한 로직이 필요하다면 여기에서 컨드롤 한다. 
        /*  
        if(dt < spf){

        } */
    }
    glfwTerminate(); 
}

 void MyGame2D::run(MyRGB _color){
    color=_color; 
    run(); 
 } 

bool MyGame2D:: isKeyPressed(const int& key){
    if(key_status.count(key) <=0) key_status [key]= false; 

    if(glfwGetKey(glfwWindow, key) == GLFW_PRESS ){
        key_status[key]=true; 
    }else{
        key_status[key]=false; 
    }

    return key_status[key]; 
} 
//1.5 add 키 입력 한번만 되도록 키를 막 눌렀을때만 true --> 작동을 잘 모르겠음. .. 
bool MyGame2D::isKeyJustPressed(const int& key){
    if(key_prev_status.count(key) <= 0) {
        key_prev_status[key]=false; 
    }
    //현재 상태 확인. 
    bool current_state=(glfwGetKey(glfwWindow, key)==GLFW_PRESS); 

    //이전에 안 눌렸고, 지금만 눌린 경우 true 
    bool just_pressed =current_state && !key_prev_status[key]; 

    //현재 상태를 이전 상태로 저장. 
    key_prev_status[key]=current_state; 

    return just_pressed; 
} 
//1.5 add --> 키값 안넘어 --> 오류수정. 
bool MyGame2D::isKeyPressedAndReleased(const int& key){
    if(key_status.count(key) <= 0) key_status[key] =false; 

    if(glfwGetKey(glfwWindow, key) == GLFW_RELEASE){
        if(key_status[key] ==true) {
            key_status[key] =false; 
            return true; 
        }else {
            key_status[key] =false; 
            return false; 
        }
    } else {
        key_status[key] = true; 
        return false; 
    }
}

//에러 처리 로직. 
void MyGame2D::reportError(const std::string& functionName, const std::string& message){
    std::cout <<"ERROR" <<functionName << " " << message << std::endl; 
    glfwTerminate(); 
    exit(1); 

} 
//1.2, 1.4 수정  
const MyRGB&  MyGame2D:: getColor() const{
    return color; 
} 
//1.2 1.4 수정. 
void MyGame2D::setColor(const MyRGB& newColor ){
    color=newColor; 
}
//1.5 추가 
float MyGame2D::getTimeStamp(){
    return spf; 
}

// 1.5 add 시간 설정
void MyGame2D::setTimeStamp(const float newSpf){
    spf=1.0f/newSpf; 
}

// 2.09 
InputManager* MyGame2D::getInputManager()
{
    return &inputManager; 
}

