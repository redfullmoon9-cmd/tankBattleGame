#include "Bullet.hpp"
#include "Render.hpp"
// #include <GL/glew.h>
// #include <GLFW/glfw3.h>

// 26.1. 11 
void Bullet::draw(){
    glPushMatrix(); 
    {
        glTranslatef(center.x, center.y, center.z);
        PrimitiveRenderer::drawRegularConvexPolygonFilled(MyRGB(255, 255,0), 0.02f, 45.0f, 8);  
        PrimitiveRenderer::drawRegularConvexPolygonWired(MyRGB(128, 128,128), 0.02f, 45.0f, 8);  
    }
    glPopMatrix(); 
}


// 26.1.12 --> hpp로 옮겨봄. 상관 없지 않나? 
// void Obstacle::draw(){
//     draw(getObsApperance()); 
// } 

//1.12 장애물을 중앙에 두개를 그린다 
//glPushMatrix 생략으로 좌표가 누적되는 문제 발생.. -->전혀 이상한 곳에 --> 일단 주석 

/* void Obstacle::draw(ObsApperance* obs){
    glPushMatrix();
    {
        glTranslatef(obs->pos.x, obs->pos.y, obs->pos.z); 
        PrimitiveRenderer::drawBoxFilled(obs->color, obs->size ); 
    }
    glPopMatrix(); 
} */  
