#include "TankGame.hpp"

// 원래는 이것을 없애고 CppOenGLTest에서 실습하려고 했던것 어쩌다 다시 .. 
namespace ref{
    int  testMain(); 
}

int main(){
    unsigned int mode =1; 
    //이전 shader변경  testMain으로 변경해서 namespace관리. 
    if(mode == 1){
        return ref::testMain(); 
    } 
    //이곳에 코드를 채워넣는 것이 목적 이었음... 
    else if(mode ==2 ){
        TankGame tGame("test tank game ", 1024, 768, std::vector{0.f, 0.f, 0.f} ); 
        tGame.init(); 
        tGame.run(); 

    }
}