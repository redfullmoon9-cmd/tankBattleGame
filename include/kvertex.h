#ifndef __KVERTEX_H__
#define __KVERTEX_H__
#include <memory>
class KVertex{
public:
    std::unique_ptr<KVertex> CreateVertex(); 
    ~KVertex(); 
private:
    KVertex(){}
}; 
#endif //__KVERTEX_H__