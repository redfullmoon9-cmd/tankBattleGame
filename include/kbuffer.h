#ifndef _KBUFFER_H__
#define _KBUFFER_H__

#include "kcommon.h"

class KBuffer{
public:
    static std::unique_ptr<KBuffer> CreateBufferWithData(int32_t bufferType, uint32_t usage, const void* data, size_t dataSize); 
    ~KBuffer(); 
    void Bind(); 
private:
    KBuffer(){}; 
    bool Init(uint32_t bufferType, uint32_t usage, const void* data, size_t dataSize ); 
    uint32_t m_buffer{0}; 
    uint32_t m_bufferType{0}; 
    uint32_t m_usage{0}; 
}; 

#endif //_KBUFFER_H__