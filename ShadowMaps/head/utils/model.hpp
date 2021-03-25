#ifndef model_hpp
#define model_hpp
#include <stb_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include "mesh.h"
#include "shader.h"
unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
class Model {
public:
    /* 模型数据向量 */
    vector<Mesh> meshes;
    //存储已经加载过的纹理
    vector<Texture> textures_loaded;
    Model(const char *path) {
        loadModel(path);
    }
    void Draw(Shader& shader);
private:
    
    string directory;
    /* 函数 */
    void loadModel(string path);

    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

#endif /* model_hpp */
