#pragma once 
#include "MyVector.hpp"

using myVec =MyVector<float>; 

class Bullet{
private: 
    myVec center = myVec(0.0f, 0.0f, 0.0f); 
    myVec velocity =myVec(0.0f, 0.0f, 0.0f); 
    int damage=1; //포탄의 공격력
    bool isDead =false; 

public: 
    Bullet()=default; 
    ~Bullet(){}
    void draw(); 
    void update(const float& dt){ center += velocity * dt; }
    myVec& getCenter(){ return center; }
    void setCenter(myVec vec) { this->center = vec; }
    myVec& getVelocity() { return velocity; }
    void setVelocity(myVec vec){ this->velocity = vec; }
    int getDamage(){ return damage;  } 
    void setIsDead( bool yesNo) { isDead = yesNo; }
    bool getIsDead() {return isDead; }

}; 