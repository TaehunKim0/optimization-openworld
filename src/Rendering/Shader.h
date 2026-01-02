#pragma once
#include <string>

class Shader
{
public:
    int ID;

public:
    Shader(const char* InVertexPath, const char* InFragmentPath);
    ~Shader();

    void Use() const;
    void SetBool(const std::string& InName, bool bInValue) const;

private:
    void CheckCompileErrors(int InShader, const std::string& InType) const;
};