#ifndef TANK_HPP
#define TANK_HPP

#include <glad/glad.h> //glGenVertexArrays??
#include <GLFW/glfw3.h> //TankGame 에서 객체에서 gl함수 사용하기로 

class Tank{
    private:
        int speed; 
        // unsigned int VBO, VAO; 
        // static constexpr float vertices[] = { 
        //     -0.5f, -0.5f, 0.0f, 
        //     0.5f, -0.5f, 0.0f,  
        //     0.0f, 0.5f, 0.0f  } ; 
        // std::array<float, 9> vertices_temp ={ // 테스트 코드 
        //     -0.5f, -0.5f, 0.0f, 
        //     0.5f, -0.5f, 0.0f,  
        //     0.0f, 0.5f, 0.0f 
        // };     
    public :

    Tank(); 
    ~Tank(); 
    static constexpr size_t vertexCount=9; 
    // static const float* getVerteces(){return vertices; }; 

}; 

#endif 