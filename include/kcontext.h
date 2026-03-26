#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <memory>
class KContext{
public:
    std::unique_ptr<KContext> CreateContext(); 
    ~KContext(); 
private:
    KContext(){}; 
    bool Init(); 
}; 


#endif //__CONTEXT_H__