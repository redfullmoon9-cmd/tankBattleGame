#include "TankGame.hpp"

namespace ref{
    int  testMain(); 
}

int main(){

    unsigned int mode =2; 
    //이전 shader변경코드 testMain으로 변경해서 namespace관리. 
    if(mode == 1){
        return ref::testMain(); 
    } else if(mode ==2 ){
        TankGame tGame("test tank game ", 1024, 768, std::vector{0.f, 0.f, 0.f} ); 
        tGame.init(); 
        tGame.run(); 

    }
}