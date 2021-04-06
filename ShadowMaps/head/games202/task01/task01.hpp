//作业一
#ifndef task01_hpp
#define task01_hpp
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <math.h>
#include <vector>
#include "camera.h"
#include "filesystem.h"
#include "model.hpp"
#include "window.h"
static unsigned int SCR_HEIGHT = 600, SCR_WIDTH = 800;
static unsigned int SHADOW_HEIGHT = 1024, SHADOW_WIDTH = 1024;
class Task01 {
public:

    GLFWwindow* createWindow();
    void render();
private:
    unsigned int quadVAO = 0, quadVBO = 0;
    unsigned int lightVAO = 0, lightVBO = 0;
    unsigned int depthFBO, depthMap;
    std::string objVs = FileSystem::getPath("shader/vs/games202/task01/obj.vs");
    std::string objFs = FileSystem::getPath("shader/fs/games202/task01/obj.fs");
    std::string lightVs = FileSystem::getPath("shader/vs/games202/task01/light.vs");
    std::string lightFs = FileSystem::getPath("shader/fs/games202/task01/light.fs");
    std::string shadowVs = FileSystem::getPath("shader/vs/games202/task01/shadow.vs");
    std::string shadowFs = FileSystem::getPath("shader/fs/games202/task01/shadow.fs");
    std::string MarryPath = FileSystem::getPath("resources/model/marry/Marry.obj");
    void initQuad();
    void initLight();
    void generateShadowMap();
};

#endif /* task01_hpp */
