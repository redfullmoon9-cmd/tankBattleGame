#include "TankGame.hpp"
#include "MyGame2D.hpp"
#include <algorithm>


//1.13 이동. 장애물 초기화 
TankGame::TankGame() :MyGame2D("test Tank", 1024, 768, MyRGB(255, 255, 255)){
    initBoundaries(); //화면의 전체 경계 
    // initObstracles(); //장애물 큰것 2개 초기화 
    initObstracleBlocks(); //블록으로 이루어진 obstalce 초기화 
} 

// topView 구분 
TankGame::TankGame(std::string view):viewType(view), MyGame2D("test Tank", 1024, 768, MyRGB(255, 255, 255)){ 
    initBoundaries(); //화면의 전체 경계 
    initObstracleBlocks(); //블록으로 이루어진 obstalce 초기화 
    // void* input=getInputManager();  
    tank =Tank(getInputManager()); //2.09 add 
}  

//1.9 add 
//1.25 add 컨트롤 변경. 
//2.09 tank 컨트롤 이동. 
void TankGame ::update(const float& dt){
    myVec tempVec=tank.getCenter(); 

    float rotationSpeed = 100.0f * dt; // 초당 100도 회전
    float moveSpeed = 0.5f * dt;
    float currentRotation = tank.getRotation();
    
    // 회전 (좌/우 키) 부분 변경 및 상하 키 조작 변경. . 1.25 
    if(isKeyPressed(GLFW_KEY_LEFT)){ 
        currentRotation += rotationSpeed;
    } 
    if(isKeyPressed(GLFW_KEY_RIGHT)){ 
        currentRotation -= rotationSpeed;
    }
    tank.setRotation(currentRotation);

    // 이동 (상/하 키) 키조작 변경 1.25 
    if(isKeyPressed(GLFW_KEY_UP)){ 
        float angleRad = PrimitiveRenderer::getMyRadian(currentRotation);
        tempVec.x += cos(angleRad) * moveSpeed;
        tempVec.y += sin(angleRad) * moveSpeed;
    }
    if(isKeyPressed(GLFW_KEY_DOWN)){ 
        float angleRad = PrimitiveRenderer::getMyRadian(currentRotation);
        tempVec.x -= cos(angleRad) * moveSpeed;
        tempVec.y -= sin(angleRad) * moveSpeed;
    }


    if(isKeyPressedAndReleased(GLFW_KEY_SPACE)){
        auto newBullet=std::make_unique<Bullet>(); 

        myVec barrelTip =tank.getBarrelTipPositionTopView();//임시로.   
        newBullet->setCenter(barrelTip); 
        
        //탱크의 회전값에 의한 발사 방향 계산. 
        float angleRadian=PrimitiveRenderer::getMyRadian(tank.getRotation()); 
        float bulletSpeed=2.0f; 
        myVec velocity(cos(angleRadian) * bulletSpeed, sin(angleRadian)* bulletSpeed, 0.0f); 

        // newBullet->setVelocity(myVec(2.0f, 0.0f, 0.0f));  
        newBullet->setVelocity(velocity); //계산된 방향으로 속도 설정.  
        bullets.push_back(std::move(newBullet)); 
    }

    //1.12 포탄 업데이트 --> 포탄 이동. 
    for(const auto& bullet : bullets){
        bullet->update(dt); 
    }

    //11.19 포탄과 장애물의 충돌 처리. 
    for(auto& bullet : bullets){
        //현재 장애물은 2개. 하나에 5.10 배열의 블록이 각각 있음. 
        for(auto& obstacle: obstacles){
            for(auto& block: obstacle.getObstacleBlocks()){
                if(block.isColliding(bullet-> getCenter()) ){
                    block.health -= bullet->getDamage(); 
                    bullet-> setIsDead(true); 
                    break; 
                }
            }
        }
    }
    //포탄 제거 
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), 
        [this](const std::unique_ptr<Bullet>& bull) { return ( isBulletOutside(bull) || bull->getIsDead() ) ; }), 
         bullets.end()); 

    //obstacle 블록구성 일때  --체력이 0이된 블록 제거 
    for(auto& obstacle : obstacles){
        obstacle.getObstacleBlocks().erase(std::remove_if(obstacle.getObstacleBlocks().begin(), obstacle.getObstacleBlocks().end(), 
        [](const ObstacleBlock& block){ return block.health <=0; } ), obstacle.getObstacleBlocks().end()); 

    }     

    float halfWidth = getTank().getBody().x / 2; 
    float halfHeight = getTank().getBody().y/ 2; 
    
    // //x축의 경계 제한. 
    if(tempVec.x - halfWidth < limitLR.x ) tempVec.x= limitLR.x +halfWidth; //왼쪽경계 
    if(tempVec.x + halfWidth > limitLR.y) tempVec.x= limitLR.y - halfWidth; 
    
    // //y축의 경계 제한 
    if(tempVec.y - halfHeight < limitUD.x) tempVec.y = limitUD.x + halfHeight; 
    if(tempVec.y + halfHeight > limitUD.y) tempVec.y = limitUD.y - halfHeight; 
    

    bool canMove =true; 
    // myVec tankCenter= tempVec; 


    // 장애물 충돌검사 1.25 주석처리 --> 뷰타입에 따라 달리 작동하도록. 
    if(getViewType() == "TopView"){
        for(auto& obstacle: obstacles){
            for(auto& block : obstacle.getObstacleBlocks()){
                // if(obs.isColliding(tankCenter, tank.getBody().x, newBodyY)){
                if(block.isCollidingTank(tempVec, tank.getBody())){
                    canMove=false; 
                    std::cout << "in collide tempVec " << tempVec << std::endl; 
                    break; 
                } 
            }
        }
    }else if(getViewType() == "IsoMetric"){
        //아이소 매트릭 장애물은 여기서
    }

    //장애물이 없다면 지정된 좌표로 이동. 
    if(canMove){
        // std::cout << "in collide tempVec " << tempVec << std::endl; 
        tank.setCenter(tempVec); 
    }
    
    tank.draw(viewType); 

    //장애물 그리기 (활성화)
    if(viewType =="TopView"){
        int mode =2; //블록들 
        for(int i=0; i<obstacles.size(); i++){
            // 큰 장애물 2개 그리기 
            if(mode ==1 ){ //  
                // obstacles[i].draw(obstacles[i].getObsApperance()); 
            }
            //블록이 모인 장애물 2개 .
            else if(mode ==2){
                obstacles[i].draw(obstacles[i].getObstacleBlocks()); 
            }
        } 
    } else if( viewType =="IsoMetric"){

    }

    // 총알 그리기 
    for(const auto& bullet : bullets){
        bullet->draw(); 
    }

}
// 1.13 이동. 
// 1. 18 변경. -- 큰 쟝애물 2개에서, 작은 블럭으로 결합된 블럭 2개. 
void TankGame::initObstracles(){
    //큰 것 2개 일때 
    obstacles.push_back(Obstacle(MyRGB(0, 0, 255), myVec(0.0f, 0.5f, 0.0f))); 
    obstacles.push_back(Obstacle(MyRGB(0, 0, 255), myVec(0.0f, -0.5f, 0.0f))); 
}

//1.21 새로 작성. 작은 블럭이 모인 장애물 2개 .. 
// 
void TankGame::initObstracleBlocks(){
    std::string view = getViewType(); 
    // 작은블록들 묶음으로 2개의 객체 . 
    // 위쪽 경계를 넘어가 포탄이 없이지는 문제 -> 장애물의 시작 위치 낮추기 
    if(view =="TopView"){
        Obstacle obs1 = Obstacle(myVec(-0.05f, 0.4f, 0.0f)); 
        obstacles.push_back(obs1); //y축 위쪽으로 
        Obstacle obs2 = Obstacle(myVec(-0.05f, -0.4f, 0.0f)); 
        obstacles.push_back(obs2); //y축 아래쪽 으로 

    } else if( view =="IsoMetric"  ){

    }
    
}

//1.13 화면상의 경계를 정한다. 
void TankGame::initBoundaries() {
    screenAspect= screenWidth/ screenHeight; 
    // openGL좌표는 -1.0 ~ 1.0을 기준으로 하므로 세로는 -1.0 ~ 1.0, 가로는 -aspect ~ aspect 
    limitLR =myVec(-screenAspect + adjustValue , screenAspect -adjustValue, 0.0f ); 
    //barrel의 길이 때문에 하단과 위단의 차이 발생. --> 
    // limitUD 위와 아래의 경계값.  어떻게 정한 값?? ->  y값도 1.0 에서 -1.0 사이의 값으로 정하는듯.  일단 조정. 
    limitUD = myVec( -0.95f, 0.95f, 0.0f ); //0.84 --> 0.90 --> 0.95 
    // std::cout << " tankgame:: initBoundaries LR -> "<< limitLR << " limitUD -> " << limitUD<< std::endl;  
}

//1.13 람다 함수 분리. update 내부에 std::erase / std::remove_if 에서 조건에 따른 자료를 앞쪽으로 
bool TankGame::isBulletOutside(const std::unique_ptr<Bullet>& bull) const{
    if(!bull) return true; 
    myVec pos =bull->getCenter();
    return (pos.x < limitLR.x || pos.x > limitLR.y || pos.y < limitUD.x || pos.y > limitUD.y); 
}  


