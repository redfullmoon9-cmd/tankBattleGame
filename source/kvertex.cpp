#include "kvertex.h"

std::unique_ptr<KVertex> KVertex::CreateVertex()
{
    // SPDLOG_INFO("in Vertext"); 
    auto vertex =std::unique_ptr<KVertex> (new KVertex());  
    if(vertex){
        vertex->Init(); 
    }
    return std::move(vertex); 
}

KVertex::~KVertex()
{
    if(m_vertexArrayObject) 
    glDeleteVertexArrays(1, &m_vertexArrayObject); 
}

void KVertex::Bind() const
{
    glBindVertexArray(m_vertexArrayObject); 
}

void KVertex::setAttribute(uint32_t attribueIndex, int count, uint32_t type, bool normalized, size_t stride, uint64_t offset) const
{
    glEnableVertexAttribArray(attribueIndex); 
    glVertexAttribPointer(attribueIndex, count, type, normalized, stride, (const void*) offset); 
}

// 모든 설정이 끝난경우 언바이딩을 호출 하는 것이 자원 관리에 용이. 
void KVertex::UnBind() const
{
    glBindVertexArray(0); 
}

void KVertex::Init()
{
    glGenVertexArrays(1, &m_vertexArrayObject); 
    Bind(); 
    SPDLOG_INFO( "vertext id {}", m_vertexArrayObject);  
}
