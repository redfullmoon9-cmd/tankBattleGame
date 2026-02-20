#include "RenderIsoMetric.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//1.22 변경. 사용코드 
void TankIsometricRenderComponent::render(const TankApperance& tankApp, MyVector<float>& pos, 
    const float& rotation, int type){
    glPushMatrix();//또 한다고? 
    {
        drawIsomatrixTank(tankApp, pos, rotation, type); 
    }
    glPopMatrix();
}


//1.22 -->> 원하는 방식의 뷰가 아님. 
//gemini변경. --> 원하는 뷰로 -- 중심좌표 . 
//뷰모양 바꾸어 보기 DIMETRI타입으로 
void TankIsometricRenderComponent::drawIsomatrixTank( const TankApperance& tankApp, MyVector<float>& pos, 
    const float& rotation, int type){
    //몸체 : 크기 0.2 0.3 0.1 --> 이미 크기가 있는데 또 ? 
    
    glTranslatef(pos.x, pos.y,  0.0f); 
    //원래 탱크사이즈와 비교 == 0.25f, 0.15f, 0.0f DIMETRIC
    if(type == 1){
        IsoMetricHelper::drawIsoBox(0.16f, 0.24f, 0.08f, tankApp.bodyColor, "ISOMETRIC", rotation); 

    }else if(type ==2 ){
        IsoMetricHelper::drawIsoBox(0.16f, 0.24f, 0.08f, tankApp.bodyColor, "DIMETRIC", rotation); 
    }
    //포탑. 
    glPushMatrix(); 
    {   
        // 포탑을 몸체 높이(0.08)만큼 위로 올림
        // 아이소메트릭 변환에서 '위'는 Y축 방향이므로 projectIso 내부의 Z값을 사용
        MyVector<float> turretPos=IsoMetricHelper::projectIso(0, 0, 0.08f); 
        glTranslatef(turretPos.x, turretPos.y, 0.0f); 
        if(type == 1){
            IsoMetricHelper::drawIsoCylinder(0.05f, 0.06f, tankApp.turretColor, "ISOMETRIC", rotation);  

        }else if(type ==2){
            IsoMetricHelper::drawIsoCylinder(0.05f, 0.06f, tankApp.turretColor, "DIMETRIC", rotation);  
        }
        
        //향후 포신을 여기에 gemini 1.15 add 
        glPushMatrix(); // Re-insert this line for barrel's transformations
        { 
            // 포신 중심이 포탑 중앙에 오도록 3D 오프셋 계산
            // 포탑 반지름: 0.05f, 포탑 높이: 0.06f
            float turret_radius = 0.05f; 
            float turret_height = 0.06f; 
            float barrel_length = tankApp.barrelSize.x; // 0.14f
            float barrel_radius = 0.01f;

            // 로컬 3D 공간에서의 포신 중심 오프셋 (포탑 밑면 중앙 기준)
            float local_3d_offsetX = 0.0f;
            float local_3d_offsetY = turret_radius + (barrel_length / 2.0f) + 0.01f; // 포탑 앞면 + 포신 길이의 절반 + 약간 더 앞으로 (시각적 부착 개선)
            float local_3d_offsetZ = turret_height / 2.0f; // 포탑 밑면 중앙 기준 포탑 높이의 절반 (수직 중앙)

            // 탱크의 회전 각도를 라디안으로 변환
            float rad = rotation * 3.141592f / 180.0f;
            float cosA = cos(rad);
            float sinA = sin(rad);

            // 로컬 3D 오프셋의 XY 구성 요소에 회전 적용
            float rotated_offsetX = local_3d_offsetX * cosA - local_3d_offsetY * sinA;
            float rotated_offsetY = local_3d_offsetX * sinA + local_3d_offsetY * cosA;

            // 이 회전된 3D 오프셋 (XYZ 포함)을 2D 화면 공간으로 투영
            MyVector<float> screen_translation_vec;
            if (type == 1) { // ISOMETRIC
                screen_translation_vec = IsoMetricHelper::projectIso(rotated_offsetX, rotated_offsetY, local_3d_offsetZ);
            } else if (type == 2) { // DIMETRIC
                screen_translation_vec = IsoMetricHelper::projectDimetric(rotated_offsetX, rotated_offsetY, local_3d_offsetZ);
            }
            
            // 계산된 변환 적용
            glTranslatef(screen_translation_vec.x, screen_translation_vec.y, 0.0f);

            // 포신 그리기: 수평 원통 함수 사용
            if(type == 1){
                // IsoMetricHelper::drawIsoHorizontalCylinder(barrelRadius, barrelLength, tankApp.barrelColor, "ISOMETRIC", rotation);
                IsoMetricHelper::drawIsoHorizontalCylinder(barrel_radius, barrel_length, tankApp.barrelColor, "ISOMETRIC", rotation);
            } else if(type == 2){
                // IsoMetricHelper::drawIsoHorizontalCylinder(barrelRadius, barrelLength, tankApp.barrelColor, "DIMETRIC", rotation);
                IsoMetricHelper::drawIsoHorizontalCylinder(barrel_radius, barrel_length, tankApp.barrelColor, "DIMETRIC", rotation);
            }
        }
    glPopMatrix();
    }
    glPopMatrix(); 
}