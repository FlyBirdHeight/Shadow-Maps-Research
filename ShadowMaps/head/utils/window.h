#ifndef window_h
#define window_h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include "camera.h"
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// camera
static float lastX = 0.0;
static float lastY = 0.0;
static bool firstMouse = true;

static Camera camera(glm::vec3(0.0, 0.0, 3.0));
class Window{
public:
    unsigned int SCR_WIDTH;
    unsigned int SCR_HEIGHT;
    std::string WINDOW_TITLE;
    Window(unsigned int width = 800, unsigned int height = 600, std::string title = "window"): SCR_WIDTH(width), SCR_HEIGHT(height), WINDOW_TITLE(title){
        lastX = (float)width / 2.0;
        lastY = (float)height / 2.0;
    }
    
    GLFWwindow* createWindow(){
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
        this->getMonitorInfo();
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_TITLE.c_str(), NULL, NULL);
        if(window == NULL){
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return NULL;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
        glEnable(GL_DEPTH_TEST);
        return window;
    }
    
    void getMonitorInfo(){
        int monitorCount;
        GLFWmonitor** pMonitor = glfwGetMonitors(&monitorCount);
        std::cout << "当前屏幕个数" << monitorCount << std::endl;
        for(int i = 0; i < monitorCount; ++i){
            int scene_x, scene_y;
            const GLFWvidmode * model = glfwGetVideoMode(pMonitor[i]);
            scene_x = model->width;
            scene_y = model->height;
            std::cout << "当前屏幕" << i << "的尺寸为：" << scene_x << "*" << scene_y << std::endl;
        }
    }
    
    void processInput(GLFWwindow *window, float deltaTime)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
        else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.ProcessKeyboard(LEFT, deltaTime);
        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }
        else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera.ProcessKeyboard(TOP, deltaTime);
        }
        else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            camera.ProcessKeyboard(DOWN, deltaTime);
        }
    }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

#endif /* window_h */
