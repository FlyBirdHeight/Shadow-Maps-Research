#include "task01.hpp"
static void processInput(GLFWwindow *window);
static float deltaTime = 0.0;
static float lastframe = 0.0;
static float far_plane = 15.0f;
static float near_plane = 0.0f;

GLFWwindow* Task01::createWindow(){
    Window window(SCR_WIDTH, SCR_HEIGHT, "pcss", false);
    
    return  window.createWindow();
}

void Task01::render(){
    GLFWwindow* window = this->createWindow();
    
    this->generateShadowMap();
    
    Shader lightShader(this->lightVs.c_str(), this->lightFs.c_str());
    Shader shader(this->objVs.c_str(), this->objFs.c_str());
    Shader shadowShader(this->shadowVs.c_str(), this->shadowFs.c_str());
    Model MarryModel(this->MarryPath.c_str());
    shader.use();
    shader.setInt("texture_diffuse1", 0);
    shader.setInt("depthMap", 1);
    glm::vec3 lightPos = glm::vec3(0.0, 5.0, 5.0);
    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastframe;
        lastframe = currentFrame;
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        
        //设置光源的正交投影矩阵（左、右、下、上、近平面、远平面）
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        //第一个参数是相机位置，第二个参数中心点坐标，第三个参数是top向量
        glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //获得世界空间坐标转换光照空间坐标的变换矩阵
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        shadowShader.use();
        shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        shadowShader.setVec3("lightPos", lightPos);
        shadowShader.setFloat("far_plane", far_plane);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, this->depthFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        MarryModel.Draw(shadowShader);
        this->initQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//        lightShader.use();
//        lightShader.setVec3("lightColor", glm::vec3(1.0, 1.0, 1.0));
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(0.0f, 5.0f, 5.0f));
//        model = glm::scale(model, glm::vec3(0.4f));
//        lightShader.setMat4("model", model);
//        lightShader.setMat4("view", camera.GetViewMatrix());
//        lightShader.setMat4("projection", projection);
//        lightShader.setVec3("lightColor", glm::vec3(1.0, 1.0, 1.0));
//        this->initLight();
        
        shader.use();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->depthMap);
        shader.setMat4("model", glm::mat4(1.0f));
        shader.setMat4("view", camera.GetViewMatrix());
        shader.setMat4("projection", projection);
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("viewPos", camera.Position);
        shader.setMat4("lightMatrix", lightSpaceMatrix);
        this->initQuad();
        MarryModel.Draw(shader);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
}

void Task01::generateShadowMap(){
    glGenFramebuffers(1, &this->depthFBO);
    glGenTextures(1, &this->depthMap);
    glBindTexture(GL_TEXTURE_2D, this->depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border[] = { 1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glBindFramebuffer(GL_FRAMEBUFFER, this->depthFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Task01::initQuad(){
    float planeVertices[] = {
        // positions            // normals         // texcoords
         10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,  0.0,  0.0f,
        -10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,

         10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,  0.0f,  0.0f,
        -10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
         10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f
    };
    if(this->quadVAO == 0){
        glGenBuffers(1, &this->quadVBO);
        glGenVertexArrays(1, &this->quadVAO);
        glBindVertexArray(this->quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);
    }
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Task01::initLight(){
    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        // front face
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        // left face
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        // right face
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        // bottom face
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        // top face
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f , 1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
    };
    if(this->lightVAO == 0){
        glGenBuffers(1, &this->lightVBO);
        glGenVertexArrays(1, &this->lightVAO);
        glBindVertexArray(this->lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->lightVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }
    
    glBindVertexArray(this->lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

static void processInput(GLFWwindow *window)
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


