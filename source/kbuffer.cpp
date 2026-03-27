#include "kbuffer.h"

std::unique_ptr<KBuffer> KBuffer::CreateBufferWithData(int32_t bufferType, uint32_t usage, const void* data, size_t dataSize)
{
    auto buffer = std::unique_ptr<KBuffer>(new KBuffer()); 
    if(buffer) buffer->Init( ); 
    return std::move(buffer); 
}

void KBuffer::Bind()
{

}

bool KBuffer::Init(uint32_t bufferType, uint32_t usage, const void *data, size_t dataSize)
{
    return false;
}
