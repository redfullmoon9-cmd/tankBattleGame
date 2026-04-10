#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "kcommon.h"
#include "kshader.h"
#include "kprogram.h"
#include "kvertex.h"
#include "kbuffer.h"

class KContext{
public:
    static std::unique_ptr<KContext> CreateContext(uint32_t mode); 
    void Render(); 
    void RenderRef(); //삼각형그리기 참조용 소스 
    void RenderRef2(); //삼각형그리기 참조용 소스 
    void RenderRef3(); //시간의 변화를 이용한 색상변화 애니메이션. 

    ~KContext(); 
    // uint32_t m_program =0; //--> KProgram 
    // std::unique_ptr<KShader> m_shader=nullptr; 
    std::unique_ptr<KProgram> m_programPtr=nullptr; 
    std::unique_ptr<KVertex> m_vertexPtr=nullptr; 
    std::unique_ptr<KBuffer> m_vertexBufferPtr=nullptr; 
    std::unique_ptr<KBuffer> m_indexBufferPtr=nullptr; 

private:
    unsigned int VBO, VAO;//참조소스용 변수. 
    uint32_t m_vertexArrayObect{0}; 
    uint32_t m_vertexBufferObject{0}; 
    uint32_t m_indexBufferObejct{0}; //
    KContext(){}; 
    bool Init(); 
    bool InitRef1(); // 정점3개 삼각형, 정점4개 사각형 그리기 참조소스 
    bool InitRef2(); // 사각형 그리기 참조소스 
    bool InitRef3(); // 리팩토링해서 vertex, prgoram shader 분리. 
}; 


#endif //__CONTEXT_H__