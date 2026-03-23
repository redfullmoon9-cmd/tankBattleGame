#ifndef TANKGAME_HPP
#define TANKGAME_HPP

#include "Game2D.hpp"
#include "Tank.hpp"
#include <spdlog/spdlog.h>
#include "MyVector.hpp"

/** tank controller class  */

class TankGame : public Game2D{
private:
    Tank tank; 
    std::string test=""; 
    tp::Vector3<float> vec3;  
    unsigned int VAO, VBO; 

public:
    TankGame( ); 
    TankGame(const std::string& title, const int& width, const int& height, std::vector<float> color ); 
    ~TankGame( ); 
    static const size_t vertexCount=9; 
    // static const float* getVertices() { return vertices; }

protected: //왜??
    void OnInit() override;  //Template Method Pattern. 
    void OnRender() override; //Template Method Pattern. 
}; 

#endif 