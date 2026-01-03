#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Color;

    Vertex(glm::vec3 pos = glm::vec3(0.0f),
        glm::vec3 color = glm::vec3(0.0f))
        : Position(pos) ,Color(color) {
    }
};

class Mesh {
public:
    std::vector<Vertex>       Vertices;
    std::vector<unsigned int> Indices;
    unsigned int VAO;

private:
    unsigned int VBO, EBO;

public:
    Mesh(std::vector<Vertex> InVertices, std::vector<unsigned int> InIndices);
    void Draw();

private:
    void SetupMesh();
};