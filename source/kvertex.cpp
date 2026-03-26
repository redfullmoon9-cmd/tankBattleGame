#include "kvertex.h"

std::unique_ptr<KVertex> KVertex::CreateVertex()
{
    auto vertex =std::unique_ptr<KVertex> (new KVertex());  
    return std::move(vertex); 
}