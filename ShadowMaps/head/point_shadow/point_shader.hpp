#ifndef point_shader_hpp
#define point_shader_hpp
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
using namespace std;
class PointShadow {
public:
    //创建窗口
    void createWindow();
    std::string LightShaderVs = FileSystem::getPath("shader/vs/point_shadow/light.vs");
    std::string LightShaderGeo = FileSystem::getPath("shader/geometry/point_shadow/light.geo");
    std::string LightShaderFs = FileSystem::getPath("shader/fs/point_shadow/light.fs");
    std::string ObjShaderVs = FileSystem::getPath("shader/vs/point_shadow/obj.vs");
    std::string ObjShaderFs = FileSystem::getPath("shader/fs/point_shadow/obj.fs");
private:
    //帧缓冲对象与CubeMap深度纹理对象
    unsigned int CubeFbo, CubeDepthTex;
    unsigned int Vao, Vbo;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    //创建屏幕渲染
    void initScene(GLFWwindow *window);
    //创建箱体
    void createCube();
    //生成深度测试图
    void generateCubeMap();
};


#endif /* point_shader_hpp */
