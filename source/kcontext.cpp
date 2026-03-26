#include "kcontext.h"

std::unique_ptr<KContext> KContext::CreateContext()
{
    auto context =std::unique_ptr<KContext> (new KContext()); 
    if(!context->Init()) return nullptr; 
    return std::move(context); 
}

bool KContext::Init()
{
    
    return true;
}
