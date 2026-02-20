#include "Obstacle.hpp"
#include "Render.hpp"
// #include <GL/glew.h>
// #include <GLFW/glfw.h>

/**
 * 11,18 tankExampleTest 에서 별도 파일로 분리  
 * 
 */

//1.18 렌더 위임구조로 변경. 
// 작은 블록으로 큰 블록 2 개를 같은 방식으로 그림.   
void ObstacleRenderComponent::render(std::vector<ObstacleBlock>& obstacleBlocks){
     /* 현재 작동되는 코드 
        for(const auto& block : obstacleBlocks ){
        // glTranslatef(block.pos.x, block.pos.y, 0.0f); 
            PrimitiveRenderer::drawBoxFilled(block.getCurrentColor(), block.pos, block.width, block.height); 
        } */

       // 변경코드 - gltranslate로 각각의 위치 값 남긴다. -- 
       for(const auto& block : obstacleBlocks ){
        glPushMatrix(); 
        {
            glTranslatef(block.pos.x, block.pos.y, 0.0f); 
            // 
            PrimitiveRenderer::drawBoxFilled(block.getCurrentColor(), block.width, block.height); 
    
        }
        glPopMatrix(); 
    }

}    
//1.18 큰 장애물 2개 렌더링 렌더. 위임구조로 변경.  
void ObstacleRenderComponent::render(ObsApperance& obstacle){
    glPushMatrix(); 
    {
        glTranslatef(obstacle.pos.x, obstacle.pos.y, obstacle.pos.z);
        PrimitiveRenderer::drawBoxFilled(obstacle.color, obstacle.size);  
    }
    glPopMatrix(); 

} 



// 1.18 크기와 컬러 기본값 가지고 있도록. 
// 작은 크기로 변경. -- 각각의 크기와 색갈은 구조체 내애서 가지고 있음. 
// 두개의 구조물을 배치 할 경우 위치 설정은. 
Obstacle:: Obstacle(){
    //myVec startPos, int row, int col, float padding
   initObstaclesBlocks(myVec(0.0f, 0.0f, 0.0f), row, col, 0.01f); 
}

// 작은 블록 단위로 장애물 만듬.  
// 파라미터 : 시작 위치 , 로우 , 컬럼값 
Obstacle::Obstacle( myVec vec){
    initObstaclesBlocks(vec, row, col, 0.01f); 

} 

//장애물이 큰 것 2개 있을 경우의 생성자. - TankGame 생성자에서 호출. 
Obstacle:: Obstacle(MyRGB color,  myVec vec){
    obs.color=color; 
    obs.pos=vec;  // 장애물 위치 
    obs.size=myVec(0.2f, 0.3f, 0.0f); //크기  
}

//11.19 tankExampleTest에서 호출. 그 싯점에서 파리미터 넘기기 어려움. 
// 기본 호출로 변경. 세부사항은 Obstacle자체로 
void  Obstacle::draw(){
    // std::cout << " obstacle draw " << std::endl; 
     obstacleRender.render(obstacleBlocks); 
}

//1.18 이동.  쟝애물 블록으로 변경. 
void  Obstacle::draw(std::vector<ObstacleBlock>& obstacleBlocks){
     obstacleRender.render(obstacleBlocks); 
}

//1.18 큰 박스 2개 그리는 것 --> 사용하지 않음.--> 변경.  
void Obstacle::draw(ObsApperance& obs){
    // std::cout<< "in draw ObsApperance"<< std::endl; 
    obstacleRender.render(obs); 
} 

// 1.12 장애물 충돌검사. --> 제대로 동작함. 장애물이 두개 인데... ? 어떻게 두개다 검사?--루프 두번 돈다.  
//target --> tank 중심좌표, targetW H 탱크의 크기 
 bool Obstacle::isColliding(const myVec& targetPos, float targetW, float targetH ) {
    // std::cout << "is coll getObsApperance()->pos.x =" << getObsApperance()->pos.x << " , targetPos.x="<< targetPos.x << std::endl;  

    // std::cout << "Obstacle pos: (" << obs.pos.x << ", " << obs.pos.y << "), " << "size: (" << obs.size.x << ", " << obs.size.y << ")" << std::endl;
    // std::cout << "Target pos: (" << targetPos.x << ", " << targetPos.y << "), "<< "size: (" << targetW << ", " << targetH << ")" << std::endl;

    //1.13 이부분 로직 분석해야. --> 분석.. collision.txt에 분석 내용. 
    float obsLeft = obs.pos.x - obs.size.x / 2.0f;
    float obsRight = obs.pos.x + obs.size.x / 2.0f;
    float obsBottom = obs.pos.y - obs.size.y / 2.0f;
    float obsTop = obs.pos.y + obs.size.y / 2.0f;
    
    float targetLeft = targetPos.x - targetW / 2.0f;
    float targetRight = targetPos.x + targetW / 2.0f;
    float targetBottom = targetPos.y - targetH / 2.0f;
    float targetTop = targetPos.y + targetH / 2.0f;              
    bool collision = (obsRight > targetLeft && obsLeft < targetRight) && (obsTop > targetBottom && obsBottom < targetTop);
    
    if(collision) {
        std::cout << ">>> COLLISION DETECTED! <<<" << std::endl;
    }
    
    return collision;

    /* return (abs(getObsApperance()->pos.x - targetPos.x) < (getObsApperance()->size.x + targetW)/2.0f) &&  
    (abs(getObsApperance()->pos.y - targetPos.y) < (getObsApperance()->size.y + targetH) /2.0f);  */
 } 

 //1.18 구조체에 작은 block이 있고, 이곳에서 그 블록을 5*10 구조로 담는다. 
 void Obstacle::initObstaclesBlocks(myVec startPos, int row, int col, float padding){
  
    for(int i = 0; i < col; i++){
        for(int j =0; j <row; j++){ //현재 로우 5. 컬럼  10. 
            ObstacleBlock obsBlock; 
            obsBlock.pos= startPos + myVec( j * (obsBlock.width + padding), i *(obsBlock.height +padding), 0.0f); 
            // std::cout<< " block pos x , y : "<< obsBlock.pos.x << " ,  " << obsBlock.pos.y<< std::endl; 
            obstacleBlocks.push_back(obsBlock); 
        }
    }
    //실제 작은 블록 좌표 출력. 
 /*    std::cout << "Obstacle :::  initObstaclesBlocks strartPos --> " <<startPos<< std::endl; 
    for(auto& block: obstacleBlocks){
        std::cout << "block pos : "<< block.pos << std::endl;  
    } */
}; 

std::vector<ObstacleBlock>& Obstacle::getObstacleBlocks(){
    return obstacleBlocks; 
}; 

