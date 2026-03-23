#include "kshader.h"

std::unique_ptr<KShader> KShader::CreateFromFile(const std::string fileName, GLenum shaderType)
{
    auto shader=std::unique_ptr<KShader> (new KShader()); 
    if(!shader -> loadFile(fileName, shaderType)) return nullptr; 

    return std::move(shader); 
}

KShader::~KShader()
{
    if(m_shader) glDeleteShader(m_shader); 
}

bool KShader::loadFile(const std::string fileName, GLenum shaderType)
{
    auto result = loadTextFile(fileName); 
    if(!result.has_value()) return false; 

    auto& code=result.value(); 
    const char* codePtr=code.c_str(); 
    
    int32_t codeLength=(int32_t) code.length(); 

    m_shader=glCreateShader(shaderType); 
    glShaderSource(m_shader, 1, (const GLchar* const* )&codePtr, &codeLength); 
    glCompileShader(m_shader); 

    int success =0; 
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success); 
    if(!success){
        char infolog[1024]; 
        glGetShaderInfoLog(m_shader, 1024, nullptr, infolog); 
        SPDLOG_ERROR("faile to compiled error: {}",fileName); 
        SPDLOG_ERROR("reason: {}",infolog); 

    }

    return false;
}
