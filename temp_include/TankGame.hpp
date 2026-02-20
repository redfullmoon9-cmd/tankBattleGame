#pragma once 

#include <memory>
#include "MyGame2D.hpp"
#include "MyVector.hpp"
#include "Render.hpp"
#include "Obstacle.hpp"
#include "Tank.hpp"
#include "Bullet.hpp"
 
/** 1.19  tank, bullet, obstacle 분리  -- 화일 크기 커지는 이유로 
 * 
*/

using myVec= MyVector<float>;  

//메인 클래스: 탱크 클래스와 블릿 클래스를 가지고 있다. 게임화면의 초기화, 주로 키 입력 당당. 
//그리기 등은 각각의 클래스에서 담당. 탱크의 움직임은 탱크 클래스에서 포탄은 포탄 클래스에서 
class TankGame: public MyGame2D{
private: 
    Tank tank; 
    std::vector<std::unique_ptr<Bullet>> bullets;  
    std::vector<Obstacle> obstacles; 
    myVec limitLR, limitUD; 
    float screenWidth =1024.0f, screenHeight=768.0f; 
    float screenAspect=0.0f, adjustValue = 0.1f;
    // unsigned int MODE = 1;
    std::string viewType =" "; // TopView, IsoMetric 

public: 
    TankGame(); 
    TankGame(std::string viewType); //TopView IsoMetric  
    ~TankGame(){ }

    // 컨트롤이 메인 -- 이곳에서 모든 그리기 상태 관리 등이 모두 이루어 진다. 
    void update(const float& dt) override; 
    Tank& getTank(){ return tank; }
    void initObstracles(); //장애물 초기화 : 큰 장애물 
    void initObstracleBlocks();  //1.21 블록으로 이루어진 장애물. 
    void initBoundaries(); //화면 경계 초기화  

    //1.13 람다 함수 분리 --포탄이 외부에 있는지 확인. 그렇지 않은 것들을 
    //자료구조 앞으로 이동. 
    bool isBulletOutside(const std::unique_ptr<Bullet>& bull) const;  

    void setViewType(std::string newType ) { viewType =newType; }
    std::string getViewType(){ return viewType;  }
}; 
