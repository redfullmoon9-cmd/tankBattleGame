#ifndef __KSHADER_H__
#define __KSHADER_H__


#include "kcommon.h"

class KShader {
public:
    static std::unique_ptr<KShader> CreateFromFile(const std::string& fileName, GLenum shaderType); 
    ~KShader(); 
    uint32_t Get(){ return m_shader ; }
private: 
    KShader(){}
    bool loadFile(const std::string& fileName, GLenum shaderType); 
    uint32_t m_shader { 0 }; //쉐이더 아이디.
    uint32_t m_program {0 }; // 프로그램 아이디. 

};

#endif //f __KSHADER_H__
