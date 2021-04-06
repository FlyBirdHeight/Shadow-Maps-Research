//
//  block.h
//  ShadowMaps
//
//  Created by adsionli on 2021/3/30.
//

#ifndef block_h
#define block_h
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Block{
public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> shortBlock;
    std::vector<glm::vec3> tallBlock;
    std::vector<glm::vec3> generateData(){
        shortBlock = {
            glm::vec3(130.0, 165.0, 65.0),
            glm::vec3(82.0, 165.0, 225.0),
            glm::vec3(240.0, 165.0, 272.0),
            glm::vec3(130.0, 165.0, 65.0),
            glm::vec3(240.0, 165.0, 272.0),
            glm::vec3(290.0, 165.0, 114.0),

            glm::vec3(290.0, 0.0, 114.0),
            glm::vec3(290.0, 165.0, 114.0),
            glm::vec3(240.0, 165.0, 272.0),
            glm::vec3(290.0, 0.0, 114.0),
            glm::vec3(240.0, 165.0, 272.0),
            glm::vec3(240.0, 0.0, 272.0),

            glm::vec3(130.0, 0.0, 65.0),
            glm::vec3(130.0, 165.0, 65.0),
            glm::vec3(290.0, 165.0, 114.0),
            glm::vec3(130.0, 0.0, 65.0),
            glm::vec3(290.0, 165.0, 114.0),
            glm::vec3(290.0, 0.0, 114.0),

            glm::vec3(82.0, 0.0, 225.0),
            glm::vec3(82.0, 165.0, 225.0),
            glm::vec3(130.0, 165.0, 65.0),
            glm::vec3(82.0, 0.0, 225.0),
            glm::vec3(130.0, 165.0, 65.0),
            glm::vec3(130.0, 0.0, 65.0),

            glm::vec3(240.0, 0.0, 272.0),
            glm::vec3(240.0, 165.0, 272.0),
            glm::vec3(82.0, 165.0, 225.0),
            glm::vec3(240.0, 0.0, 272.0),
            glm::vec3(82.0, 165.0, 225.0),
            glm::vec3(82.0, 0.0, 225.0)
        };
        tallBlock = {
            glm::vec3(423.0, 330.0, 247.0),
            glm::vec3(265.0, 330.0, 296.0),
            glm::vec3(314.0, 330.0, 456.0),
            glm::vec3(423.0, 330.0, 247.0),
            glm::vec3(314.0, 330.0, 456.0),
            glm::vec3(472.0, 330.0, 406.0),

            glm::vec3(423.0, 0.0, 247.0),
            glm::vec3(423.0, 330.0, 247.0),
            glm::vec3(472.0, 330.0, 406.0),
            glm::vec3(423.0, 0.0, 247.0),
            glm::vec3(472.0, 330.0, 406.0),
            glm::vec3(472.0, 0.0, 406.0),

            glm::vec3(472.0, 0.0, 406.0),
            glm::vec3(472.0, 330.0, 406.0),
            glm::vec3(314.0, 330.0, 456.0),
            glm::vec3(472.0, 0.0, 406.0),
            glm::vec3(314.0, 330.0, 456.0),
            glm::vec3(314.0, 0.0, 456.0),

            glm::vec3(314.0, 0.0, 456.0),
            glm::vec3(314.0, 330.0, 456.0),
            glm::vec3(265.0, 330.0, 296.0),
            glm::vec3(314.0, 0.0, 456.0),
            glm::vec3(265.0, 330.0, 296.0),
            glm::vec3(265.0, 0.0, 296.0),

            glm::vec3(265.0, 0.0, 296.0),
            glm::vec3(265.0, 330.0, 296.0),
            glm::vec3(423.0, 330.0, 247.0),
            glm::vec3(265.0, 0.0, 296.0),
            glm::vec3(423.0, 330.0, 247.0),
            glm::vec3(423.0, 0.0, 247.0)
        };

        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

        for(unsigned int i = 0; i < shortBlock.size(); i += 6){
            glm::vec3 normal = this->calculateNormals(shortBlock[i], shortBlock[i + 1], shortBlock[i + 2]);
            for(unsigned int j = 0; j < 6; ++j){
                vertices.push_back(shortBlock[i + j]);
                vertices.push_back(normal);
                vertices.push_back(color);
            }
        }

        for(unsigned int i = 0; i < tallBlock.size(); i += 6){
            glm::vec3 normal = this->calculateNormals(tallBlock[i], tallBlock[i + 1], tallBlock[i + 2]);
            for(unsigned int j = 0; j < 6; ++j){
                vertices.push_back(tallBlock[i + j]);
                vertices.push_back(normal);
                vertices.push_back(color);
            }
        }

        return vertices;
    };

    glm::vec3 calculateNormals(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3){
        return glm::normalize(glm::cross(v1 - v2, v1 - v3));
    }
};


#endif /* block_h */
