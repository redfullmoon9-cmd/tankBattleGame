#ifndef __KVERTEX_H__
#define __KVERTEX_H__

#include "kcommon.h"

class KVertex{
public:
    static std::unique_ptr<KVertex> CreateVertex(); 
    ~KVertex(); 
    void Bind() const;  //uint32_t attributeIndex, int count, uint32_t type, bool normalized, size_t stride, uint64_t offset
    void setAttribute(uint32_t attribueIndex, int count, uint32_t type, bool normalized, size_t stride, uint64_t offset) const; 
    void UnBind() const; 
private:
    KVertex(){}
    void Init(); 
    uint32_t m_vertexArrayObject{0}; 
}; 

#endif //__KVERTEX_H__