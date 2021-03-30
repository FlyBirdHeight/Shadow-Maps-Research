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
    glm::vec3 color;
    std::vector<glm::vec3> generateData();
};


#endif /* block_h */
