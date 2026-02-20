



#include "Render.hpp"

//1.10 구조변경 tank 내부에 컴포넌트 구조를 포함하고 이곳에서 그리기로. 
void TankRendererComponent::render(){
        glPushMatrix(); 
        {
            glTranslatef(0.0f, 0.0f, 0.0f); 
            PrimitiveRenderer::drawBoxFilled(MyRGB(255, 0, 0), 0.25f, 0.1f); //body
            glTranslatef(-0.02f, 0.1f, 0.0f); 
            PrimitiveRenderer::drawBoxFilled(MyRGB(0, 255, 0), 0.14f, 0.08f); //turret
            glTranslatef(0.15, 0.0f, 0.0f); 
            PrimitiveRenderer::drawBoxFilled(MyRGB(0,0,255), 0.15, 0.03f); //barrel

        }
        glPopMatrix(); 
} 

//1.10 포탑회전 하는 버전 
//rotation 값은 키가 움직임에 따라서 전달됨. 
//1.11 탱크의 바디도 포탑의 회전 처럼 돌리려면. --> 해결. 
//1.21. 변경. 탱크 view를  top View 방식으로 전환. --> 참조용 코드. 
void TankRendererComponent::render(std::string str, const TankApperance& tankApp, MyVector<float>& center, const float& rotation){
    // std::cout << "in rener(p3)" << std::endl; 
    glPushMatrix(); 
    {
        //탱크의 중심으로 이동. 
        glTranslatef(center.x, center.y, center.z); 
        //탱크 포탑만 회전하는 것에서 - 전체 회전으로 --> 원하는 방식이 아님. 
        // glRotatef(rotation, 0, 0, 1); 
        PrimitiveRenderer::drawBoxFilled(tankApp.bodyColor, tankApp.bodySize); 
        // std::cout << "center --> " << center << "tank bodySize :: " << tankApp.bodySize<< std::endl; 

        glPushMatrix(); 
        {   //몸체위로 살짝 들어 올림. 
            glTranslatef(-0.02f, 0.1f, 0.0f); 
            glRotatef(rotation, 0, 0, 1); //탱크 포탑만 회전하는 것에서 - 전체 회전으로 
            PrimitiveRenderer::drawBoxFilled(tankApp.turretColor, tankApp.turretSize); 

            //포신 그리기(포탑기준 상대 좌표)
            glTranslatef(0.15f, 0.0f, 0.0f); 
            PrimitiveRenderer::drawBoxFilled(tankApp.barrelColor, tankApp.barrelSize); 
        }
        glPopMatrix(); 
    }
    glPopMatrix(); 
} 

//1.21 탱크 평면도 형태인 topview로 변경 --사용코드 
void TankRendererComponent::render(const TankApperance& tankApp, MyVector<float>& center, const float& rotation){
    glPushMatrix(); 
    {
        //탱크의 중심으로 이동. 
        glTranslatef(center.x, center.y, center.z); 
        glRotatef(rotation, 0, 0, 1 ); 
        PrimitiveRenderer::drawBoxFilled(tankApp.bodyColor, tankApp.bodySize); 
        
        glPushMatrix(); 
        {   
            glTranslatef(0.0f, 0.0f, 0.0f);  
            PrimitiveRenderer::drawBoxFilled(tankApp.turretColor, tankApp.turretSize); 
            
            glTranslatef(tankApp.turretSize.x-0.01f, 0.0f, 0.0f); 
            PrimitiveRenderer::drawBoxFilled(tankApp.barrelColor, tankApp.barrelSize ) ; 
        }
        glPopMatrix(); 
    }
    glPopMatrix(); 
}   


