#include "kbuffer.h"

std::unique_ptr<KBuffer> KBuffer::CreateBufferWithData(int32_t bufferType, uint32_t usage, const void* data, size_t dataSize)
{
    // SPDLOG_INFO(" in Buffer "); 
    auto buffer = std::unique_ptr<KBuffer>(new KBuffer()); 
    if(!buffer->Init(bufferType, usage, data, dataSize ))
    { 
        return nullptr;  
    }
    return std::move(buffer); 
}

KBuffer::~KBuffer()
{
    if(m_buffer) glDeleteBuffers(1, &m_buffer); 
}

void KBuffer::Bind()
{
    glBindBuffer(m_bufferType, m_buffer); 
}

bool KBuffer::Init(uint32_t bufferType, uint32_t usage, const void *data, size_t dataSize)
{
    m_bufferType=bufferType; 
    m_usage=usage; 
    glGenBuffers(1, &m_buffer); 
    Bind(); 
    glBufferData(m_bufferType, dataSize, data, usage); 
    return true;
}
