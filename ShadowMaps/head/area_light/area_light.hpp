#ifndef area_light_hpp
#define area_light_hpp
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "camera.h"
#include "window.h"
#include "filesystem.h"
struct AreaLightData{
    glm::vec3 position;
    glm::mat3 basis;
    glm::vec2 size;
    glm::vec3 intensity;
    float attenuation;
};
class AreaLight {
public:
    void createLight();
    unsigned int SCR_HEIGHT = 600, SCR_WIDTH = 800;
private:
    unsigned int roomVAO = 0, roomVBO = 0;
    std::string lightVs = FileSystem::getPath("shader/vs/area_light/area_light.vs");
    std::string lightFs = FileSystem::getPath("shader/vs/area_light/area_light.fs");
    void initRender();
    void initRoom();
};

#endif /* area_light_hpp */
