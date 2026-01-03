#include "Model.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"

Model::Model(const std::string& InPath)
{
    LoadModel(InPath);
}

void Model::Draw(unsigned int InShaderProgram)
{
    for (Mesh& MeshObj : Meshes)
    {
        MeshObj.Draw(InShaderProgram);
    }
}

void Model::LoadModel(const std::string& InPath)
{
    Assimp::Importer Importer;
    // aiProcess_Triangulate: 폴리곤을 삼각형으로 강제 변환
    // aiProcess_FlipUVs: OpenGL 좌표계에 맞게 텍스처 Y축 반전
    const aiScene* Scene = Importer.ReadFile(InPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

    if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
    {
        std::cerr << "ASSIMP ERROR: " << Importer.GetErrorString() << std::endl;
        return;
    }

    Directory = InPath.substr(0, InPath.find_last_of('/'));

    ProcessNode(Scene->mRootNode, Scene);
}

void Model::ProcessNode(aiNode* InNode, const aiScene* InScene)
{
    // 1. 현재 노드에 할당된 메시들을 모두 처리
    for (unsigned int i = 0; i < InNode->mNumMeshes; i++)
    {
        aiMesh* MeshData = InScene->mMeshes[InNode->mMeshes[i]];
        Meshes.push_back(ProcessMesh(MeshData, InScene));
    }

    // 2. 자식 노드가 있다면 재귀적으로 동일 작업 수행
    for (unsigned int i = 0; i < InNode->mNumChildren; i++)
    {
        ProcessNode(InNode->mChildren[i], InScene);
    }
}

Mesh Model::ProcessMesh(aiMesh* InMesh, const aiScene* InScene)
{
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    // 1. 정점 데이터 추출 (Position, Normal, TexCoords)
    for (unsigned int i = 0; i < InMesh->mNumVertices; i++)
    {
        Vertex VertexObj;
        // 위치
        VertexObj.Position = glm::vec3(InMesh->mVertices[i].x, InMesh->mVertices[i].y, InMesh->mVertices[i].z);
        // 법선
        if (InMesh->HasNormals())
            VertexObj.Normal = glm::vec3(InMesh->mNormals[i].x, InMesh->mNormals[i].y, InMesh->mNormals[i].z);
        // UV
        if (InMesh->mTextureCoords[0])
            VertexObj.TexCoords = glm::vec2(InMesh->mTextureCoords[0][i].x, InMesh->mTextureCoords[0][i].y);
        else
            VertexObj.TexCoords = glm::vec2(0.0f, 0.0f);

        Vertices.push_back(VertexObj);
    }

    // 2. 인덱스 데이터 추출
    for (unsigned int i = 0; i < InMesh->mNumFaces; i++)
    {
        aiFace Face = InMesh->mFaces[i];
        for (unsigned int j = 0; j < Face.mNumIndices; j++)
            Indices.push_back(Face.mIndices[j]);
    }

    return Mesh(Vertices, Indices);
}