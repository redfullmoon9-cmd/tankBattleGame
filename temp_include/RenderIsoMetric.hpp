#pragma once
// #ifndef RENDERISOMETRIC_HPP
// #define RENDERISOMETRIC_HPP

#include "ComponentType.hpp"
#include "MyVector.hpp"
#include <string>
#include <cmath>
// #include <GL/glew.h>  //제거 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector> // For std::vector

// 1.22 추가 --> 1.24 render 이동 --> 1.25 RenderIsoMetric.hpp로 분리
class TankIsometricRenderComponent{
private: 
    void drawIsomatrixTank(const TankApperance& tankApp, MyVector<float>& pos, const float& rotation, int type); 
public:
    //1.24 아이소메트릭 뷰의 변경 코드 --> 별도로 빼야 하지 않을까? 
    void render(const TankApperance& tankApp, MyVector<float>& worldPos, const float& rotation, int type); 

}; 

//1.22 아이소메트릭 뷰 전환. -- inline
// 1.25 RenderIsoMetric.hpp로 분리
namespace IsoMetricHelper {
    using myVec = MyVector<float>; 

    // 2D 좌표를 아이소메트릭 화면 좌표로 변환
    inline MyVector<float>  worldToIso(float x, float y){
        MyVector<float> iso; 
        iso.x = (x-y) * 0.866f; // cos(30°) ≈ 0.866
        iso.y= (x+y) *0.5f; //sin(30)
        iso.z = 0.0f; 
        return iso;   
    }

    inline MyVector<float> isoToWorld(float screenX, float screenY){
        MyVector<float> world; 
        world.x = (screenX /0.866f + screenY) / 2.0f; 
        world.y = (screenY -screenX /0.866f) /2.0f; 
        world.z =0.0f; 
        return world; 
    }


    //먼저 3D 공간의 점을 아이소메트릭 평면으로 투영해주는 헬퍼 함수를 정의. 
    // 3D 좌표를 아이소메트릭 2D 좌표로 변환 (2.5D 방식)
    inline MyVector<float> projectIso(float x, float y, float z){
        // 공률(Scale)과 각도를 고려한 표준 아이소메트릭 변환
        // 보통 x, y는 30도 각도로 벌어지고 z는 수직 위로 향합니다.
        
        //표준 아이소메트릭 좌표 변환. 
        //(x-y) * cosf(30도) / (x +y ) * sinf(30도) +z 
        float isoX =(x - y) * cosf(3.141592f /6.0f); // 30 degree 0.866025f 
        float isoY =(x +y ) * sinf(3.141592f /6.0f) +z ; //0.5f
        return myVec(isoX, isoY, 0.0f); 
    }

    //Dimetric 2:1의 더 세워진 뷰..  
    inline MyVector<float> projectDimetric(float x, float y, float z){
        myVec result; 
        result.x = (x - y) *0.5f;
        result.y = z +(x + y) * 0.25f; 
        return result;   
    }

    //비교를 위한 custom 
    inline MyVector<float> projectCustom(float x, float y, float z, float yRotDeg, float xRotDeg){
        // 라디안 변환
        float yRad = yRotDeg * 3.14159f / 180.0f;
        float xRad = xRotDeg * 3.14159f / 180.0f;
        
        float cosY = cos(yRad), sinY = sin(yRad);
        float cosX = cos(xRad), sinX = sin(xRad);
        
        // Y축 회전 먼저
        float x1 = x * cosY + z * sinY;
        float y1 = y;
        float z1 = -x * sinY + z * cosY;
        
        // X축 회전
        float x2 = x1;
        float y2 = y1 * cosX - z1 * sinX;
        float z2 = y1 * sinX + z1 * cosX; // z2 is not used
        
        // 직교 투영 (z는 버림)
        myVec result;
        result.data[0] = x2;
        result.data[1] = y2;
        return result;
    }
    
    //현재 iosmetric에서 dimetric 등의 방식으로 변경해 보아야. 
    inline void drawIsoBox(float width, float length, float height, const MyRGB& color, const std::string& projectType, float rotation = 0.0f){
        float w = width / 2.0f;
        float l = length / 2.0f;
        float h = height;
        float rad = rotation * 3.141592f / 180.0f; // Reverted rotation
        float cosA = cos(rad);
        float sinA = sin(rad);

        // 8개의 정점을 월드 좌표에서 정의
        myVec points[8] = {
            {-w, -l, 0}, { w, -l, 0}, { w,  l, 0}, {-w,  l, 0},
            {-w, -l, h}, { w, -l, h}, { w,  l, h}, {-w,  l, h}
        };

        // Z축 기준으로 회전 적용
        for (int i = 0; i < 8; ++i) {
            float x = points[i].x;
            float y = points[i].y;
            points[i].x = x * cosA - y * sinA;
            points[i].y = x * sinA + y * cosA;
        }

        myVec v1, v2, v3, v4, v5, v6, v7, v8;  
        if(projectType == "ISOMETRIC" ){
            v1 = projectIso(points[0].x, points[0].y, points[0].z);
            v2 = projectIso(points[1].x, points[1].y, points[1].z);
            v3 = projectIso(points[2].x, points[2].y, points[2].z);
            v4 = projectIso(points[3].x, points[3].y, points[3].z);
            v5 = projectIso(points[4].x, points[4].y, points[4].z);
            v6 = projectIso(points[5].x, points[5].y, points[5].z);
            v7 = projectIso(points[6].x, points[6].y, points[6].z);
            v8 = projectIso(points[7].x, points[7].y, points[7].z);
        }
        else if( projectType == "DIMETRIC"){ //projectDimetric
            v1 = projectDimetric(points[0].x, points[0].y, points[0].z);
            v2 = projectDimetric(points[1].x, points[1].y, points[1].z);
            v3 = projectDimetric(points[2].x, points[2].y, points[2].z);
            v4 = projectDimetric(points[3].x, points[3].y, points[3].z);
            v5 = projectDimetric(points[4].x, points[4].y, points[4].z);
            v6 = projectDimetric(points[5].x, points[5].y, points[5].z);
            v7 = projectDimetric(points[6].x, points[6].y, points[6].z);
            v8 = projectDimetric(points[7].x, points[7].y, points[7].z);
        }

        //바닥면은 보이진 않으나 정의. 
        // 옆면은 어두운 색으로 그림자 효과를 가지게 한다. 
        glBegin(GL_QUADS); 
        {
            glColor3f(color.r * 0.6f, color.g * 0.6f, color.b * 0.6f); 
            glVertex2fv(v1.data); 
            glVertex2fv(v2.data); 
            
            glVertex2fv(v6.data); 
            glVertex2fv(v5.data); 
            
            glVertex2fv(v2.data); 
            glVertex2fv(v3.data); 

            glVertex2fv(v7.data); 
            glVertex2fv(v6.data); 
        }
        glEnd(); 

        // 3. 앞면/뒷면 (중간 밝기)
        glBegin(GL_QUADS);{
            glColor3f(color.r * 0.8f, color.g * 0.8f, color.b * 0.8f);
            glVertex2fv(v3.data); glVertex2fv(v4.data); glVertex2fv(v8.data); glVertex2fv(v7.data);
            glVertex2fv(v4.data); glVertex2fv(v1.data); glVertex2fv(v5.data); glVertex2fv(v8.data);
            glEnd();

        }

        // 4. 윗면 (가장 밝은 색 - 하이라이트)
        glBegin(GL_QUADS);
        {
            glColor3f(color.r, color.g, color.b); 
            glVertex2fv(v5.data); glVertex2fv(v6.data); glVertex2fv(v7.data); glVertex2fv(v8.data);
            glEnd();

        }
    }

    // New function: drawIsoCylinder
    inline void drawIsoCylinder(float radius, float height, const MyRGB& color, const std::string& projectType, float rotation = 0.0f, int segments = 16) {
        float rad = rotation * 3.141592f / 180.0f; // Reverted rotation
        float cosA = cos(rad);
        float sinA = sin(rad);

        std::vector<myVec> bottom_projected(segments);
        std::vector<myVec> top_projected(segments);

        for (int i = 0; i < segments; ++i) {
            float theta = 2.0f * 3.141592f * static_cast<float>(i) / static_cast<float>(segments);
            float x_raw = radius * cosf(theta);
            float y_raw = radius * sinf(theta);

            // Apply Z-axis rotation to raw coordinates
            float x_rotated = x_raw * cosA - y_raw * sinA;
            float y_rotated = x_raw * sinA + y_raw * cosA;

            if (projectType == "ISOMETRIC") {
                bottom_projected[i] = projectIso(x_rotated, y_rotated, 0.0f);
                top_projected[i] = projectIso(x_rotated, y_rotated, height);
            } else if (projectType == "DIMETRIC") {
                bottom_projected[i] = projectDimetric(x_rotated, y_rotated, 0.0f);
                top_projected[i] = projectDimetric(x_rotated, y_rotated, height);
            }
        }

        // Draw side faces
        glBegin(GL_QUADS);
        {
            glColor3f(color.r * 0.7f, color.g * 0.7f, color.b * 0.7f); // Slightly darker for sides
            for (int i = 0; i < segments; ++i) {
                int next_i = (i + 1) % segments;
                glVertex2fv(bottom_projected[i].data);
                glVertex2fv(bottom_projected[next_i].data);
                glVertex2fv(top_projected[next_i].data);
                glVertex2fv(top_projected[i].data);
            }
        }
        glEnd();

        // Draw top face
        glBegin(GL_POLYGON); // GL_POLYGON for convex shapes
        {
            glColor3f(color.r, color.g, color.b); // Brightest for top
            for (int i = 0; i < segments; ++i) {
                glVertex2fv(top_projected[i].data);
            }
        }
        glEnd();
    }

    // New function: drawIsoHorizontalCylinder (for barrel)
    inline void drawIsoHorizontalCylinder(float radius, float length, const MyRGB& color, const std::string& projectType, float rotation = 0.0f, int segments = 16) {
        float rad = rotation * 3.141592f / 180.0f; // Reverted rotation
        float cosA = cos(rad);
        float sinA = sin(rad);

        std::vector<myVec> cap1_projected(segments); // back cap
        std::vector<myVec> cap2_projected(segments); // front cap

        float half_len = length / 2.0f;

        for (int i = 0; i < segments; ++i) {
            float theta = 2.0f * 3.141592f * static_cast<float>(i) / static_cast<float>(segments);
            float x_raw = radius * cosf(theta);
            float z_raw = radius * sinf(theta); // Circle is on XZ plane

            // Point on back cap (y = -half_len)
            float p1_x_rot = x_raw * cosA - (-half_len) * sinA;
            float p1_y_rot = x_raw * sinA + (-half_len) * cosA;
            
            // Point on front cap (y = +half_len)
            float p2_x_rot = x_raw * cosA - (half_len) * sinA;
            float p2_y_rot = x_raw * sinA + (half_len) * cosA;

            if (projectType == "ISOMETRIC") {
                cap1_projected[i] = projectIso(p1_x_rot, p1_y_rot, z_raw);
                cap2_projected[i] = projectIso(p2_x_rot, p2_y_rot, z_raw);
            } else { // DIMETRIC
                cap1_projected[i] = projectDimetric(p1_x_rot, p1_y_rot, z_raw);
                cap2_projected[i] = projectDimetric(p2_x_rot, p2_y_rot, z_raw);
            }
        }

        // Draw side faces
        glBegin(GL_QUADS);
        {
            glColor3f(color.r * 0.7f, color.g * 0.7f, color.b * 0.7f);
            for (int i = 0; i < segments; ++i) {
                int next_i = (i + 1) % segments;
                glVertex2fv(cap1_projected[i].data);
                glVertex2fv(cap1_projected[next_i].data);
                glVertex2fv(cap2_projected[next_i].data);
                glVertex2fv(cap2_projected[i].data);
            }
        }
        glEnd();

        // Draw front cap
        glBegin(GL_POLYGON);
        {
            glColor3f(color.r, color.g, color.b);
            for (int i = 0; i < segments; ++i) {
                glVertex2fv(cap2_projected[i].data);
            }
        }
        glEnd();
    }
};