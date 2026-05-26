#ifndef __KPROGRAM_H__
#define __KPROGRAM_H__

#include <memory>
#include <vector>
#include "kshader.h"

class KProgram {
public:
    static std::unique_ptr<KProgram> CreateProgram(const std::vector<std::shared_ptr<KShader>>& shaders ); 
    uint32_t Get(){return m_program; }
    ~KProgram();
    void Use() const; 

    void setUniform(const std::string& name, int value) const; 
    void setUniform(const std::string& name, const glm::mat4& value) const;
private:
    KProgram(){} 
    bool LinkProgram(const std::vector<std::shared_ptr<KShader>>& shaders); 
    uint32_t m_program { 0 }; 

}; 

#endif //__KPROGRAM_H__