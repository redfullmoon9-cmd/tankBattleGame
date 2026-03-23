#ifndef __KSHADER_H__
#define __KSHADER_H__


#include <memory>
#include <glad/glad.h>
#include <string>
#include <optional>
#include <spdlog/spdlog.h>

class KShader {
public:
    static std::unique_ptr<KShader> CreateFromFile(const std::string fileName, GLenum shaderType); 
    ~KShader(); 
    uint32_t Get(){ return m_shader ; }
private: 
    KShader(){}
    bool loadFile(const std::string fileName, GLenum shaderType); 
    std::optional<std::string> loadTextFile(const std::string fileName); 
    uint32_t m_shader { 0 }; 

};

#endif //f __KSHADER_H__
