//
//  mesh.h
//  opengl
//
//  Created by adsionli on 2021/1/5.
//  Copyright © 2021 adsionli. All rights reserved.
//

#ifndef mesh_h
#define mesh_h
#include <vector>
#include <string>
#include "shaders.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using std::string;
using std::vector;
struct Vertex {
    //位置向量
    glm::vec3 Position;
    //法向量
    glm::vec3 Normal;
    //纹理向量
    glm::vec2 TexCoords;
};
struct Texture {
    unsigned int ID;
    //类型:diffuse 还是 specular
    string type;
    aiString path;
};
class Mesh {
    public:
        unsigned int VAO;
        //构造方法
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;

            setupMesh();
        };
        vector<Texture> textures;
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        void Draw(Shader& shader) {
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int reflectionNr = 1;
            for (unsigned int i = 0; i < textures.size(); i++) {
                glActiveTexture(GL_TEXTURE + i);
                string number;
                string name = textures[i].type;
                if (name == "texture_diffuse") {
                    number = std::to_string(diffuseNr++);
                }
                else if (name == "texture_reflection") {
                    number = std::to_string(reflectionNr++);
                }
                else {
                    number = std::to_string(specularNr++);
                }
                shader.setFloat(("material." + name + number).c_str(), i);
                glBindTexture(GL_TEXTURE_2D, textures[i].ID);
            }
            glActiveTexture(GL_TEXTURE0);
            // 绘制网格
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        };
private:
    //顶点对象
    unsigned int VBO, EBO;
    //初始化
    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //绑定顶点缓冲对象数据
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        //绑定顶点索引对象数据
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
            &indices[0], GL_STATIC_DRAW);

        // 顶点数组位置
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // 顶点法线
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // 顶点纹理坐标
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }
};

#endif /* mesh_h */
