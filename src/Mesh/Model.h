#pragma once

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

#include "Mesh.h"

class Model {
public:
    Model(const std::string& InPath);

    void Draw(unsigned int InShaderProgram);

private:
    std::vector<Mesh> Meshes;
    std::string Directory;
    std::vector<Texture> TexturesLoaded;

    void LoadModel(const std::string& InPath);
    void ProcessNode(aiNode* InNode, const aiScene* InScene);
    Mesh ProcessMesh(aiMesh* InMesh, const aiScene* InScene);
};