#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <spdlog/spdlog.h>

/*제미니 쉐이더 */
class Shader
{
public:
    unsigned int ID; // 쉐이더 프로그램 아이디.

    // 소스코드의 경로를 받아 쉐이더 빌드
    Shader(const char *vertexPath, const char *fragmentPath);
    void use();

    // uniform 변수 설정용 유틸리티 함수.cpu의 데이터를 GPU(쉐이더 소스코드) 안에 있는 uniform 변수로 전달. 
    // uniform으로 선언된 변수는 실시간에 변경 가능. 
    void setBool(const std::string &name, bool value) const; //glUniform1i를 사용해서 0, 1값을 보냄. 
    void setInt(const std::string &name, int value) const; //쉐이더 내에 int변수(텍스쳐 슬롯 값)를 변경. 
    void setFloat(const std::string &name, float value) const; //float변수값(투영도 시간의 변화량..)

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif