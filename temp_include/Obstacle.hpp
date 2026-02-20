#pragma once 
#include <vector> 
#include "MyVector.hpp"


using myVec =MyVector<float>;  

struct ObsApperance{
    MyRGB color; 
    myVec pos; 
    myVec size; 
}; 

struct ObstacleBlock{
    MyVector<float> pos; //위치 
    float width =0.03f; //가로 
    float height =0.03f; //세로 
    int health = 3; 

    MyRGB getCurrentColor() const {
        if(health == 3) return MyRGB(0, 0, 255); //파랑
        if(health == 2) return MyRGB(100, 100, 255); //연파랑
        if(health == 1) return MyRGB(255,  100, 100); //빨강
        return MyRGB(0, 0, 0); 
    } 
    // 총알 좌표를 받아 Obstacle block의 충돌 검사.  
    bool isColliding(const myVec& bulletPos) const {
        return (bulletPos.x > pos.x - width / 2 && bulletPos.x < pos.x + width / 2 &&
                bulletPos.y > pos.y - height / 2 && bulletPos.y < pos.y + height / 2);
    }
    //탱크와 Obstacle block의 충돌 검사. 
    //  문제 1) 탱크가 꽤 지나가야 충돌 인식. --계산 잘못 ,  오타 -- 수정 제대로 동작. 
    //  문제 2) 상단 첫번째 블록 2줄 인식않됨. 
    bool isCollidingTank( const myVec& tankPos, const myVec& tankBody){
        return (tankPos.x -tankBody.x/2.0f < pos.x + width/2.0f && tankPos.x + tankBody.x /2.0f >  pos.x - width/2 && 
                tankPos.y -tankBody.y /2.0f < pos.y + height /2  && tankPos.y + tankBody.y/2.0f > pos.y -height/2); 
    }
}; 

class ObstacleRenderComponent {
public: 
    void render(std::vector<ObstacleBlock>& obstacleBlocks); 
    void render(ObsApperance& obs); 
}; 


//26.1.12 장애물 클래스 추가.  같은 장애물을 2곳 배치. 위치를 달리해서 
// --> 향후 작은 박스로 특정 행렬로 쌓고 포탄에 의해 없어지는 로직 구성. 
// -- 구조체 포인터 사용으로 복사/대입 연산자 작성. -- 실제 포인터 쓸일이 아닐 것.
// -- 그냥 레퍼런스를 쓰거나/ 스마트 포인터를 사용하는 것이 ... 
//25.11.18 작은 단위로 여러개 배치 그다음 포탄 발사에 의한 충돌, 변화 적용. 
//  작은 단위들 스마트 포인터로  
//  apperance는 그냥 객체로. 
//  renderComponet구조로 

//--> 25.11.18 별도 파일 분리 TankExampleTest로 부터 분리. 
// 구조체 분리, 구조체 포인터에서 일반 객체로   

using myVec= MyVector<float>; 
class Obstacle {
private: 
    const int row =5,  col=10; //block의 행렬 값 정의 
    ObsApperance obs; //장애물 규격의 구조체  
    ObstacleRenderComponent obstacleRender; 
    std::vector<ObstacleBlock> obstacleBlocks; 
public : 
    //블록 단위 장애물 5*10 의 구조가 2개 있을때 
    Obstacle(); 

    //장애물 큰것 2개 있을 때 구조 
    Obstacle(MyRGB color,  myVec vec); 

    //시작 위치만  
    Obstacle( myVec vec); 

    // 26.1.12 구조체 포인터로 인해 복사 생성자 추가. --> 깊은 복사를 해야함. 
    // Obstacle(const Obstacle& other); 포인터 --> 일반 변수 필요 없어짐. 
    // 26.1.12 구조체 포인터로 인한 대입연산자 추가 --> 이동.  
    // Obstacle& operator=(const Obstacle& other); 포인터 --> 일반 변수 필요 없어짐. 
    //소멸자에서 메모리 해제 --현재 기능 필요 없어짐. 
    ~Obstacle(){} // 

    //뭐가 잘 않되서 여기에서 구현. -->glPushMatrix 생략으로 좌표 누적 문제 발생. 
    // 향후 렌더러 작성해서 위임 구조로 변경해야. --> render구조 변경. 
    void draw(); 
    void draw(std::vector<ObstacleBlock>& obstacleBlocks); // 블록단위 박스 그리기  
    void draw(ObsApperance& obs); //큰 박스 2개 

    // void draw(ObsApperance* obs);  
    void update(){} 
    ObsApperance& getObsApperance(){ return obs; }  

    // 충돌검사 함수. --> 큰 장애물 2개일 때 --> 변경. 사용하지 않음. 
    bool isColliding(const myVec& targetPos, float targetW, float targetH ) ; 

    //5* 10의 행렬구조로 묶는다. 
    // startPos 그려질 위치 
    void initObstaclesBlocks(myVec startPos, int row, int col, float padding); 
    // initObstracleBlocks
    std::vector<ObstacleBlock>& getObstacleBlocks(); 
}; 