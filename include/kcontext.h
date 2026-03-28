#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "kcommon.h"
#include "kshader.h"
#include "kprogram.h"
#include "kvertex.h"
#include "kbuffer.h"

class KContext{
public:
    static std::unique_ptr<KContext> CreateContext(); 
    void Render(); 
    void RenderRef(); //삼각형그리기 참조용 소스 
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
    KContext(){}; 
    bool Init(); 
    bool InitRef(); //참조소스 
}; 


#endif //__CONTEXT_H__