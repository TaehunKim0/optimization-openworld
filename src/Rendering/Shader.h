#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    int ID;

public:
    Shader(const char* InVertexPath, const char* InFragmentPath);
    ~Shader();

    void Use() const;
    void SetBool(const std::string& InName, bool bInValue) const;
    void SetMat4(const std::string& InName, const glm::mat4& InMat);
    
private:
    void CheckCompileErrors(int InShader, const std::string& InType) const;
};