#include "Mesh.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

Mesh::Mesh(std::vector<Vertex> InVertices, std::vector<unsigned int> InIndices)
{
    this->Vertices = InVertices;
    this->Indices = InIndices;

    SetupMesh();
}

void Mesh::Draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(Indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::SetupMesh()
{
    // 1. 버퍼 생성
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // 2. VBO 데이터 채우기 (정점 데이터)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

    // 3. EBO 데이터 채우기 (인덱스 데이터 - 삼각형 구성 방식)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

    // 4. 정점 속성 설정
    GLsizei stride = 6 * sizeof(float);

    // 위치 (Position)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

    // 컬러 (Color)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
}