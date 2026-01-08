#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* InVertexPath, const char* InFragmentPath)
    : ID(0)
{
    std::string VertexCode;
    std::string FragmentCode;
    std::ifstream VShaderFile;
    std::ifstream FShaderFile;

    VShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    FShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        VShaderFile.open(InVertexPath);
        FShaderFile.open(InFragmentPath);

        std::stringstream VShaderStream, FShaderStream;
        VShaderStream << VShaderFile.rdbuf();
        FShaderStream << FShaderFile.rdbuf();

        VShaderFile.close();
        FShaderFile.close();

        VertexCode = VShaderStream.str();
        FragmentCode = FShaderStream.str();
    }
    catch (const std::ifstream::failure& E)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << E.what() << std::endl;
        return;
    }

    const char* VShaderCode = VertexCode.c_str();
    const char* FShaderCode = FragmentCode.c_str();

    // 1. 셰이더 컴파일
    const int Vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Vertex, 1, &VShaderCode, nullptr);
    glCompileShader(Vertex);
    CheckCompileErrors(Vertex, "VERTEX");

    const int Fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(Fragment, 1, &FShaderCode, nullptr);
    glCompileShader(Fragment);
    CheckCompileErrors(Fragment, "FRAGMENT");

    // 2. 셰이더 프로그램 링크
    ID = glCreateProgram();
    glAttachShader(ID, Vertex);
    glAttachShader(ID, Fragment);
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");

    // 3. 링크 후 개별 셰이더 삭제
    glDeleteShader(Vertex);
    glDeleteShader(Fragment);
}

Shader::~Shader()
{
    if (ID != 0)
    {
        glDeleteProgram(ID);
    }
}

void Shader::Use() const
{
    glUseProgram(ID);
}

void Shader::SetBool(const std::string& InName, bool bInValue) const
{
    glUniform1i(glGetUniformLocation(ID, InName.c_str()), static_cast<int>(bInValue));
}

void Shader::SetMat4(const std::string& InName, const glm::mat4& InMat)
{
    unsigned int loc = glGetUniformLocation(ID, InName.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(InMat));
}

void Shader::SetVec3(const std::string& InName, const glm::vec3& InVec3)
{
    glUniform3fv(glGetUniformLocation(ID, InName.c_str()), 1, &InVec3[0]);
}

void Shader::CheckCompileErrors(int InShader, const std::string& InType) const
{
    int Success;
    char InfoLog[1024];

    if (InType != "PROGRAM")
    {
        glGetShaderiv(InShader, GL_COMPILE_STATUS, &Success);
        if (!Success)
        {
            glGetShaderInfoLog(InShader, 1024, nullptr, InfoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << InType << "\n" << InfoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(InShader, GL_LINK_STATUS, &Success);
        if (!Success)
        {
            glGetProgramInfoLog(InShader, 1024, nullptr, InfoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << InType << "\n" << InfoLog << std::endl;
        }
    }
}