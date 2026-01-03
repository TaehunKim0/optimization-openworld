#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    Vertex() : Position(0.f), Normal(0.f), TexCoords(0.f) {}
};

struct Texture {
    unsigned int ID;
    std::string Type; 
    std::string Path; 
};

class Mesh {
public:
    std::vector<Vertex>       Vertices;
    std::vector<unsigned int> Indices;
    unsigned int VAO;

private:
    unsigned int VBO, EBO;

public:
    Mesh(const std::vector<Vertex>& InVertices, const std::vector<unsigned int>& InIndices);
    void Draw(unsigned int InShaderProgram);

private:
    void SetupMesh();
};