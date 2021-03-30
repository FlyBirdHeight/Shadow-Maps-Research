#include "area_light.hpp"
static float deltaTime = 0.0;
static float lastFrame = 0.0;
static float fpsFrame = 0.0;
static int frame = 0;

void AreaLight::createLight(){
    Window lightWindow(this->SCR_WIDTH, this->SCR_HEIGHT, "area light");
    GLFWwindow* window = lightWindow.createWindow();
    Shader shader(this->lightVs.c_str(), this->lightFs.c_str());
    shader.use();
    shader.setInt("planeTexture", 0);
    glm::vec3 lightPos = glm::vec3(0.0, 0.0, 0.0);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        frame++;
        
        glClearColor(0.4, 0.4, 0.4, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
        model = glm::scale(model, glm::vec3(10.0f));
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)this->SCR_WIDTH / (float)this->SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::vec3 viewPos = camera.Position;
        shader.setMat4("projection", projection);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setVec3("viewPos", viewPos);
        shader.setVec3("lightPos", lightPos);
        this->initRoom();
        
        
        
        
        if (currentFrame - lastFrame > 0.25 && frame > 10) {
            double fps = (double) frame / (currentFrame - fpsFrame);
            fpsFrame = currentFrame;
            frame = 0;
            
            std::cout << "当前屏幕帧数：" << fps << std::endl;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}

void AreaLight::initRender(){
    
}

void AreaLight::initRoom(){
    if(this->roomVAO == 0){
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, (float)224/255, (float)186/255, (float)157/255, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, (float)224/255, (float)186/255, (float)157/255, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, (float)224/255, (float)186/255, (float)157/255, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, (float)224/255, (float)186/255, (float)157/255, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, (float)224/255, (float)186/255, (float)157/255, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, (float)224/255, (float)186/255, (float)157/255, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, (float)214/255, (float)44/255, (float)35/255, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, (float)214/255, (float)44/255, (float)35/255, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, (float)214/255, (float)44/255, (float)35/255, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, (float)214/255, (float)44/255, (float)35/255, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, (float)214/255, (float)44/255, (float)35/255, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, (float)214/255, (float)44/255, (float)35/255, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, (float)85/255, (float)151/255, (float)57/255, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, (float)85/255, (float)151/255, (float)57/255, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, (float)85/255, (float)151/255, (float)57/255, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, (float)85/255, (float)151/255, (float)57/255, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, (float)85/255, (float)151/255, (float)57/255, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, (float)85/255, (float)151/255, (float)57/255, 0.0f, 0.0f, // bottom-left
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, (float)224/255, (float)186/255, (float)157/255, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, (float)224/255, (float)186/255, (float)157/255, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, (float)224/255, (float)186/255, (float)157/255, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, (float)224/255, (float)186/255, (float)157/255, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, (float)224/255, (float)186/255, (float)157/255, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, (float)224/255, (float)186/255, (float)157/255, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, (float)224/255, (float)186/255, (float)157/255, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, (float)224/255, (float)186/255, (float)157/255, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, (float)224/255, (float)186/255, (float)157/255, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, (float)224/255, (float)186/255, (float)157/255, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, (float)224/255, (float)186/255, (float)157/255, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, (float)224/255, (float)186/255, (float)157/255, 0.0f, 0.0f  // bottom-left
        };
        glGenBuffers(1, &this->roomVBO);
        glGenVertexArrays(1, &this->roomVAO);
        glBindVertexArray(this->roomVAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->roomVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    glBindVertexArray(this->roomVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
