#include "TankGame.hpp"

static constexpr float vertices[]={
    -0.5f, -0.5f, 0.0f, 
    0.5f, -0.5f, 0.0f,  
    0.0f, 0.5f, 0.0f  
}; 

TankGame::TankGame(){ }
TankGame::TankGame(const std::string& title, const int& width, const int& height , std::vector<float> color)
: Game2D(title, width, height, color) {
    SPDLOG_INFO("TankGame init"); 
    // tp::Vector3 myVec(1.0f, 1.0f, 1.0f); //vector3 class 테스트용. 
    // tp::Vector3 myVec2(2.0f, 2.09f, 2.0f); 
    // myVec += myVec2; 
   
} 

TankGame::~TankGame(){
        glDeleteVertexArrays(1, &VAO); 
        glDeleteBuffers(1, &VBO); 

}

void TankGame::OnInit()
{
    glGenVertexArrays(1, &VAO); //향후 -->tank로 
    glGenBuffers(1, &VBO); //향후 -->tank로 

    //vao bind. 지금부터 VBO정보가 VAO 에 기록된다. 
    glBindVertexArray(VAO); 
    
    //VBO bind 및 데이터 복사. 
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,  GL_STATIC_DRAW); 

    //정점 속성 설정( 0번속성, 3개 요소x,y,z ,float 타입, 간격, 시작점 )
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), (void*)0); //이 싯점이후 VBO가 VAO에 저장된다. 
    glEnableVertexAttribArray(0); 

}

//실제로 그림은 이곳에서 그려짐. 
void TankGame::OnRender()
{ 
    //Template Method Pattern 
    // if(basicShader){
        // basicShader->use(); //쉐이더 사용
    // }
    //쉐이더 변경.-refactoring  
    m_programPtr->Use(); 
    glBindVertexArray(VBO);  // 그릴 데이터 바인딩. 
    glDrawArrays(GL_TRIANGLES, 0, 3); //그리기 실행. 

    glBindVertexArray(0); // 4. 해제 -- 엔진 스타일로 
    // Game2D::Render(); 

}
