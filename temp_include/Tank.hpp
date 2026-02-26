#pragma once 
#include "MyVector.hpp"
// #include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ComponentType.hpp"
#include "Render.hpp"
#include "RenderIsoMetric.hpp"
#include "InputManager.hpp"
// #include "IsoMetricView.hpp"


/** 26.1.19 Tank.hpp로 분리  
 *  26.1.21 탱크 평면도 방식 top view로 변환. 
 *  26.2.9 키보드 이벤트 tankgame 에서 이동함. 
*/
namespace ref{

    using myVec =MyVector<float>; 
    
    // class TankRendererComponent; 
    
    class Tank{
    private:
        // 탱크가 그려질 중심점. -- 중앙으로 부터 약간 왼쪽에서 위치 하도록 
        MyVector<float> center=MyVector<float>(-1.0f, 0.0f, 0.0f); 
        
        float rotation =0.0f; 
        float speed=0.0f; 
        TankRendererComponent renderer; //일반 평면도형의 렌더러. 
        TankIsometricRenderComponent isoMetricRender; //아이소메트릭 렌더러.  
        MyRGB color; 
    
        //구조체로 묶어서 한통으로 관리. --> 이부분 없애고 생성자에서 ... 
        MyVector<float> t_body= MyVector<float>(0.25f, 0.15f, 0.0f); //tank body size 
        MyVector<float> t_turret= MyVector<float>(0.16f, 0.09f, 0.0f); //tank turret size  
        MyVector<float> t_barrel= MyVector<float>(0.14f, 0.03f, 0.0f); //tank barrel size
        TankApperance tankApp;
    
        InputManager* inputManager; 
    protected: 
        
    public: 
        Tank()=default; 
        Tank(InputManager* inputManager); 
        ~Tank(); 
        //선택적으로 탑뷰 1 와 Isoview 2 
        void draw(std::string view);
        // 구조체로 탱크의 apperance 한꺼번에 넘기기  
        void draw(const TankApperance* tankApp, MyVector<float>& center ); 
        void draw(const MyRGB& color, const MyVector<float>& vec, const float& rotation ); 
    
        //isoMetricRender 사용 예제
        void draw(const TankApperance& tankApp, MyVector<float>& center); 
    
        // void update(){ }
        void setCenter(const MyVector<float>& vec); 
        MyVector<float>& getCenter(); 
        MyVector<float>& getBody() { return t_body; }
        MyVector<float>& getTurret() { return t_turret; }
        MyVector<float>& getBarrel() { return t_barrel; }
    
        void setRotation(const float& angle); 
        const float& getRotation(){return rotation; }
        bool canMoveto(){ return true; }; 
        // MyVector<float> getBarrelTipPosition3(); //이동 -> 회전 코드 
        MyVector<float> getBarrelTipPositionTopView(); //렌더링 순서로 회전 이동 코드  
        MyVector<float> getBarrelTipPositionIsoMetricView(); //1.26 코드 분리 
        
        void InputProcess(float dt); 
    };
}
