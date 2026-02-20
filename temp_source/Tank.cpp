#include "Tank.hpp"
// #include "Render.hpp"

//1. 21 변경. 포인터에서 일반 변수로 
 Tank::Tank(InputManager* _inputManager){
    // this->tankApp =new TankApperance(); //멤버변수에 할당해야 
    tankApp.bodyColor=MyRGB(255, 0, 0); 
    tankApp.turretColor=MyRGB(0, 255, 0); 
    tankApp.barrelColor=MyRGB(0, 0, 255); 
    tankApp.bodySize=t_body; 
    tankApp.turretSize=t_turret; 
    tankApp.barrelSize=t_barrel; 

    inputManager =_inputManager; 

 }

// 1,21 변경. 
 Tank::~Tank(){ 
    /* if(this->tankApp) delete this->tankApp; 
    this->tankApp=nullptr;  */
}; 
  

//1.9 add 
void Tank::setCenter(const MyVector<float>& vec){
    center=vec; 
}

//1.9 add
MyVector<float>& Tank::getCenter(){
    return center; 
} 

//1.9 기본 값으로 생성. --> 몸체만 그려봄 --> 다 그림. 
/**
 * mod==1 topview 2== isometricview 
 */
 void Tank::draw(std::string view ) {
    if(view =="TopView") { //일반 탑뷰 렌더링. 
        renderer.render( this->tankApp, this->center, this->rotation); 
    } 
    else if(view =="IsoMetric") { //아이소메트릭 뷰 중의 타입 1. 
        isoMetricRender.render(tankApp, center, rotation, 1); 
    }
}

//1.10 탱크에 관한 모든 것 구조체(tankApp) 로 넘김. 
void Tank::draw(const TankApperance* tankApp, MyVector<float>& center ){
    renderer.render(this->tankApp, center, this->rotation); 
}

//1. 11 
void Tank::draw(const MyRGB& color, const MyVector<float>& vec, const float& rotation ){
    renderer.render(); 
} 

//1.22 아이소메트릭 렌더러 테스트 
void Tank::draw(const TankApperance& tankApp, MyVector<float>& center ){
    // isoMetricRender.render(tankApp, center, rotation); 
}

//1. 11 
void Tank::setRotation(const float& angle){
    this->rotation= angle; 
} 

//1.16 보정 수치 입력에서 바렐팁 계산으로 변경. 
//1.22 탑뷰방식으로 변경후 다시 보정값 수정. 벡터로 변경.  
//1.26 topView 와 IsoMetricView 분리 
MyVector<float> Tank::getBarrelTipPositionTopView()  {
    float angleRad = PrimitiveRenderer::getMyRadian(rotation);
    float cosAngle = cos(angleRad);
    float sinAngle = sin(angleRad);
    
    float barrel_length = tankApp.barrelSize.x; // 0.14f
    
    myVec localBarrelTipOffset( barrel_length/0.5f, 0.0f, 0.0f); //오프셋 조정값 탱크 중심

    myVec rotatedXY( localBarrelTipOffset.x * cosAngle - localBarrelTipOffset.y * sinAngle, 
        localBarrelTipOffset.x * sinAngle + localBarrelTipOffset.y * cosAngle, 0.0f); 

    myVec barrelTip(center.x + rotatedXY.x, center.y + rotatedXY.y, 0.0f);     
    return MyVector<float>(barrelTip.x, barrelTip.y, 0.0f);
}


//1.26 분리 
MyVector<float> Tank::getBarrelTipPositionIsoMetricView(){
    float angleRad = PrimitiveRenderer::getMyRadian(rotation);
    float cosAngle = cos(angleRad);
    float sinAngle = sin(angleRad);
    
    float turret_radius = 0.0f; // 탑뷰에서는 0.0보정값 없이. 
    float turret_height = 0.06f;
    float barrel_length = tankApp.barrelSize.x; // 0.14f
    
    myVec localBarrelTipOffset(
        0.0f, // X-component: centered
        turret_radius + barrel_length, // Y-component: turret front + full barrel length (forward)
        0.08f + (turret_height / 2.0f) // Z-component: turret base offset + half turret height (vertical center)
    );

    // Rotate this 3D offset by the tank's rotation around Z-axis
    // The tank's visual forward (0 degrees) is along its local +Y axis
    float rotatedX = localBarrelTipOffset.x * cosAngle - localBarrelTipOffset.y * sinAngle;
    float rotatedY = localBarrelTipOffset.x * sinAngle + localBarrelTipOffset.y * cosAngle;
    
    // Final world coordinates are tank's center + rotated 2D offset (Z is discarded for 2D game logic)
    float barrelTipX = center.x + rotatedX;
    float barrelTipY = center.y + rotatedY;
    
    std::cout << "isoMetric barrel tip x, y  "<< barrelTipX  <<"," << barrelTipY << std::endl; 
    
    return MyVector<float>(barrelTipX, barrelTipY, 0.0f);
}

//02. 09 키보드 입력을 이곳에서 통합해서 처리.
void Tank::InputProcess(float dt)
{ 
    myVec tempVec=getCenter(); 
    // std::string  view =getViewType(); //현재 뷰타입 얻기 
    
    float rotationSpeed = 100.0f * dt; // 초당 100도 회전
    float moveSpeed = 0.5f * dt;
    float currentRotation = getRotation();

    MyVector<float> direction{0.f, 0.f, 0.f}; 
    direction.x = inputManager->GetAxisValue("MoveHorizontal"); //_Positive 1.f nega -1.f 
    direction.y = inputManager->GetAxisValue("MoveVertical"); 

    std::cout << "direction " <<  direction << std::endl; 

    // if(isKeyPressed(GLFW_KEY_LEFT)){ 
    //     currentRotation += rotationSpeed;
    // } 
    // if(isKeyPressed(GLFW_KEY_RIGHT)){ 
    //     currentRotation -= rotationSpeed;
    // }

    setRotation(currentRotation);

    // 이동 (상/하 키) 키조작 변경 1.25 - 이동 방향 수정
    float angleRad = PrimitiveRenderer::getMyRadian(currentRotation); // currentRotation은 이제 사용자의 0도(오른쪽) 기준 각도
    // if(isKeyPressed(GLFW_KEY_UP)){ 
    //     tempVec.x += cos(angleRad) * moveSpeed; // X-component adjustment
    //     tempVec.y += sin(angleRad) * moveSpeed;  // Y-component adjustment
    // }
    // if(isKeyPressed(GLFW_KEY_DOWN)){ 
    //     tempVec.x -= cos(angleRad) * moveSpeed; // X-component adjustment
    //     tempVec.y -= sin(angleRad) * moveSpeed;  // Y-component adjustment
    // }


    // if(isKeyPressedAndReleased(GLFW_KEY_SPACE)){
    // auto newBullet=std::make_unique<Bullet>(); 
    // myVec barrelTip; 
    // //테스트를 위해서 분리 
    // if( view == "TopView" ){
    //     barrelTip =tank.getBarrelTipPositionTopView(); 
    // }
    // else if(view =="IsoMetric"){
    //         barrelTip =tank.getBarrelTipPositionIsoMetricView(); 
    // }
    // newBullet->setCenter(barrelTip); 

        
    //탱크의 회전값에 의한 발사 방향 계산. (시각적 방향에 맞게 수정)
    // float bulletSpeed=2.0f; 
    // currentRotation을 기준으로 cos(X), sin(Y) 사용
    // myVec velocity(cos(angleRad) * bulletSpeed, sin(angleRad)* bulletSpeed, 0.0f); 

    // newBullet->setVelocity(myVec(2.0f, 0.0f, 0.0f));  
    // newBullet->setVelocity(velocity); //계산된 방향으로 속도 설정.  
    // bullets.push_back(std::move(newBullet)); 
        
    // }
}
