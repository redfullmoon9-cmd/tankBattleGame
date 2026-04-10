#include "kprogram.h"

std::unique_ptr<KProgram> KProgram::CreateProgram(const std::vector<std::shared_ptr<KShader>> &shaders)
{
    SPDLOG_INFO("in Create Program");
    auto program = std::unique_ptr<KProgram>(new KProgram()); 
    if(!program->LinkProgram(shaders)){
        return nullptr; 
    }
    return std::move(program);
}

KProgram::~KProgram()
{
    if(m_program) glDeleteProgram(m_program); 
}

void KProgram::Use() const
{
    // SPDLOG_INFO("in program use:");
    glUseProgram(m_program); 
}

bool KProgram::LinkProgram(const std::vector<std::shared_ptr<KShader>> &shaders)
{
    SPDLOG_INFO(" in Link Program shader size {}", shaders.size()); 
    m_program = glCreateProgram(); 
    for(auto& shader: shaders){
        glAttachShader(m_program, shader->Get()); 
    }

    glLinkProgram(m_program); 
    
    int success=0; 
    glGetProgramiv(m_program, GL_LINK_STATUS, &success); 
    if(!success){
        char infolog[1024]; 
        glGetProgramInfoLog(m_program, 1012, nullptr, infolog); 
        SPDLOG_INFO("Failed to link KProgram {}", infolog); 
        return false; 
    }
    return true;
}
