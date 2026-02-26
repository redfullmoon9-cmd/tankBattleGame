#include "TankGame.hpp"


TankGame::TankGame(){
    SPDLOG_INFO("TankGame init"); 
    tp::Vector3 myVec(1.0f, 1.0f, 1.0f); 
    tp::Vector3 myVec2(2.0f, 2.09f, 2.0f); 
    
}
TankGame::TankGame(const std::string& title, const int& width, const int& height , std::vector<float> color)
: Game2D(title, width, height, color) {

} 
TankGame::~TankGame(){

}

void TankGame::Update()
{
}

void TankGame::Render()
{
    // if(basicShader){
    //     basicShader->use(); 
    //     glBindVertexArray(VBO); 
    //     glDrawArrays(GL_TRIANGLES, 0, 3); 
    // }
    // Game2D::Render(); 
}
