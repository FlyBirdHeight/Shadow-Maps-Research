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
#include "area.h"
#include "block.h"
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
    unsigned int blockVAO = 0, blockVBO = 0;
    unsigned int lightVAO = 0, lightVBO = 0;
    glm::vec3 lightPos;
    std::string lightVs = FileSystem::getPath("shader/vs/area_light/area_light.vs");
    std::string lightFs = FileSystem::getPath("shader/fs/area_light/area_light.fs");
    void initRender(const Shader &shader, glm::vec3 lightPos);
    void initRoom(const Shader &shader);
    void initBox();
    void initBlock();
    void initLight();
};

#endif /* area_light_hpp */
