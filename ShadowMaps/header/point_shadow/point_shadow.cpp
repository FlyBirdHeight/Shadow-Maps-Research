//
//  point_shadow.cpp
//  ShadowMaps
//
//  Created by adsionli on 2021/3/18.
//

#include "point_shadow.hpp"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

PointShadow::PointShadow(){
    this->camera.Position = glm::vec3(0.0, 0.0, 3.0);
    this->deltaTime = 0.0;
    this->lastFrame = 0.0;
}

void PointShadow::createWindow(){
    glfwInit();
    glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    this->window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Point Shadow", NULL, NULL);
    glfwMakeContextCurrent(this->window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    
    glfwSetScrollCallback(this->window, scroll_callback);
    glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
    glfwSetCursorPosCallback(this->window, mouse_callback);
    
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}

void PointShadow::render(){
    glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
    this->objShader.use();
    this->objShader.setInt("diffuseTexture", 0);
    this->objShader.setInt("shadowMap", 1);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        this->deltaTime = currentFrame - this->lastFrame;
        this->lastFrame = currentFrame;
        
        processInput(window);
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 100.0f;
        lightProjection = glm::ortho(-10.0, 10.0f, -10.0, 10.0, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        this->lightSceneShader.use();
        this->lightSceneShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glViewport(0, 0, this->SHADOW_DEPTH_WIDTH, this->SHADOW_DEPTH_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        renderScene(this->lightSceneShader);
        glBindFramebuffer(0);
        
        glViewport(0.0, 0.0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        this->objShader.use();
        this->objShader.setMat4("projection", glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f));
        this->objShader.setMat4("view", camera.GetViewMatrix());
        this->objShader.setVec3("viewPos", camera.Position);
        this->objShader.setVec3("lightPos", lightPos);
        this->objShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->depthTexture);
    }
}

void PointShadow::initDepthTexture(){
    glGenFramebuffers(1, &this->fbo);
    glGenTextures(1, &this->depthTexture);
    glBindTexture(GL_TEXTURE_2D, this->depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->SHADOW_DEPTH_WIDTH, this->SHADOW_DEPTH_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float[] border = { 1.0, 1.0, 1.0, 1.0};
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, this->SHADOW_DEPTH_WIDTH, this->SHADOW_DEPTH_HEIGHT);
}

void PointShadow::renderScene(Shader &shdaer){
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    this->InitPlace();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0, 1.5, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    this->InitBox();
    shader.setMat4("model", model);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    this->InitBox();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.25));
    shader.setMat4("model", model);
    this->InitBox();
}

void PointShadow::InitBox(){
    if(this->boxVao == 0){
        glGenBuffers(1, &this->boxVbo);
        glGenVertexArrays(1, &this->boxVao);
        glBindVertexArray(this->boxVao);
        glBindBuffer(GL_ARRAY_BUFFER, this->boxVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) * (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) * (6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    glBindVertexArray(this->boxVao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void PointShadow::InitPlace(){
    if(this->placeVao == 0){
        glGenBuffers(1, &this->placeVbo);
        glGenVertexArrays(1, &this->placeVao);
        glBindVertexArray(this->placeVao);
        glBindBuffer(GL_ARRAY_BUFFER, this->placeVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->planeVertices), this->planeVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) * (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) * (6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    glBindVertexArray(this->placeVao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

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
