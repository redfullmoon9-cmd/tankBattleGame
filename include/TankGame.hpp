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
    // static constexpr float vertices[]={
    //     -0.5f, -0.5f, 0.0f, 
    //     0.5f, -0.5f, 0.0f,  
    //     0.0f, 0.5f, 0.0f  
    // }; 
public:
    TankGame( ); 
    TankGame(const std::string& title, const int& width, const int& height, std::vector<float> color ); 
    ~TankGame( ); 
    void Update() override; 
    void Render() override; 
    // static const size_t vertexCount=9; 
    // static const float* getVertices() { return vertices; }
}; 

#endif 