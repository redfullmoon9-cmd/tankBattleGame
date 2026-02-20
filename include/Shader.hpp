#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader
{
public:
    unsigned int ID; // 쉐이더 프로그램 아이디.

    // 소스코드의 경로를 받아 쉐이더 빌드
    Shader(const char *vertexPath, const char *fragmentPath);
    void use();

    // uniform 변수 설정용 유틸리티 함수.
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif