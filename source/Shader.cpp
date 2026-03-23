#include "Shader.hpp"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    SPDLOG_INFO("shader constructor"); 
    // 1 소스 코드 읽기.
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    //26. 03.06 파일을 읽는 동안 문제가 생기면 예외를 발생시키도록 함. 
    // failbit :파일을 찾을 수 없거나 읽다가 오류가 나는 경우. 
    // badbit 하드웨어문제등 치명적 입출력 오류가 발생하는 경우. 
    //ifstream의 경우 파일 읽기 실패해도 실패상태으 비트만 저장 따라서 if(!file.is_open())방식으로 검사해야. 
    //이것을 대신해 주는것임. 
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        // SPDLOG_INFO("file open success "); 

        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e)
    {
        std::cout << "ERROR ::SHADER::FILE_NOT_SUCCESSFULLY_READ " << std::endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // 2. 컴파일 및 링크
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // Fragmenet shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    // SPDLOG_INFO("in shader use()"); 
    glUseProgram(ID);
}

/**id와 이름을 통해서 해당 변수의 이치를 찾고 이를 glUmiform1i로 전딜해서 값을 변경함. 
 * 그래픽 카드에 변수의 변경값을 전달함. setBool값을 통해 0, 1 값을 보냄. 
*/
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

//
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value );
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (float)value);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        }
    }
}