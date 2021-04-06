//
//  area.h
//  ShadowMaps
//
//  Created by adsionli on 2021/3/30.
//

#ifndef area_h
#define area_h
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Area{
public:
    std::vector<glm::vec3> bottom;
    std::vector<glm::vec3> top;
    std::vector<glm::vec3> left;
    std::vector<glm::vec3> right;
    std::vector<glm::vec3> back;
    std::vector<glm::vec3> ceilingWindow;
    std::vector<std::vector<glm::vec3>> vertices;
    std::vector<glm::vec3> color;
    std::vector<glm::vec3> generateData(){
        std::vector<glm::vec3> returnData;
        bottom = {
            glm::vec3(552.8, 0.0, 0.0),
            glm::vec3(0.0, 0.0, 0.0),
            glm::vec3(0.0, 0.0, 559.2),
            glm::vec3(552.8, 0.0, 0.0),
            glm::vec3(0.0, 0.0, 559.2),
            glm::vec3(549.6, 0.0, 559.2)
        };
        color.push_back(glm::vec3(1.0, 1.0, 1.0));
        top = {
            glm::vec3(556.0, 548.8, 0.0),
            glm::vec3(556.0, 548.8, 559.2),
            glm::vec3(0.0, 548.8, 559.2),
            glm::vec3(556.0, 548.8, 0.0),
            glm::vec3(0.0, 548.8, 559.2),
            glm::vec3(0.0, 548.8, 0.0)
        };
        color.push_back(glm::vec3(1.0, 1.0, 1.0));
        left = {
            glm::vec3(552.8, 0.0, 0.0),
            glm::vec3(549.6, 0.0, 559.2),
            glm::vec3(556.0, 548.8, 559.2),
            glm::vec3(552.8, 0.0, 0.0),
            glm::vec3(556.0, 548.8, 559.2),
            glm::vec3(556.0, 548.8, 0.0)
        };
        color.push_back(glm::vec3(1.0, 0.0, 0.0));
        back = {
            glm::vec3(549.6, 0.0, 559.2),
            glm::vec3(0.0, 0.0, 559.2),
            glm::vec3(0.0, 548.8, 559.2),
            glm::vec3(549.6, 0.0, 559.2),
            glm::vec3(0.0, 548.8, 559.2),
            glm::vec3(556.0, 548.8, 559.2)
        };
        color.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
        right = {
            glm::vec3(0.0, 0.0, 559.2),
            glm::vec3(0.0, 0.0, 0.0),
            glm::vec3(0.0, 548.8, 0.0),
            glm::vec3(0.0, 0.0, 559.2),
            glm::vec3(0.0, 548.8, 0.0),
            glm::vec3(0.0, 548.8, 559.2)
        };
        color.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        
        vertices.push_back(bottom);
        vertices.push_back(top);
        vertices.push_back(left);
        vertices.push_back(back);
        vertices.push_back(right);
        
        for(unsigned int i = 0; i < vertices.size(); ++i){
            glm::vec3 normal = this->calculateNormal(vertices[i][0], vertices[i][1], vertices[i][2]);
            for(unsigned int j = 0; j < vertices[i].size(); ++j){
                returnData.push_back(vertices[i][j]);
                returnData.push_back(normal);
                returnData.push_back(color[i]);
            }
        }
        
        
        return returnData;
    };
    
    glm::vec3 calculateNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3){
        return glm::normalize(glm::cross(v1 - v2, v1 - v3));
    };
};

#endif /* area_h */
