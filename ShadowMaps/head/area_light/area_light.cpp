#include "area_light.hpp"
static float deltaTime = 0.0;
static float lastFrame = 0.0;
static float fpsFrame = 0.0;
static int frame = 0;

void AreaLight::createLight(){
    Window lightWindow(this->SCR_WIDTH, this->SCR_HEIGHT, "area light");
    GLFWwindow* window = lightWindow.createWindow();
    Shader shader(this->lightVs, this->lightFs);
    camera.Position = glm::vec3(27.8, 27.3, -80.0);
    camera.Front = glm::vec3(0.0, 0.0, 1.0);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        frame++;

        lightWindow.processInput(window, deltaTime);
        
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader.use();
        this->initRender(shader, this->lightPos);
        
        //帧数计算
        if (currentFrame - fpsFrame > 0.25 && frame > 10) {
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

void AreaLight::initRender(const Shader &shader, glm::vec3 lightPos){
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)this->SCR_WIDTH / (float)this->SCR_HEIGHT, 0.1f, 300.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::vec3 viewPos = camera.Position;
    float near_plane = 1.0f;
    float far_plane = 7.5f;
    //设置光源的正交投影矩阵（左、右、下、上、近平面、远平面）
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    //第一个参数是相机位置，第二个参数中心点坐标，第三个参数是top向量
    glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setVec3("viewPos", viewPos);
    shader.setVec3("lightPos", lightPos);
    shader.setBool("is_light", true);
    this->initRoom(shader);
}

void AreaLight::initRoom(const Shader &shader){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.1f));
    shader.setMat4("model", model);
    this->initBox();
    this->initBlock();
    this->initLight();
}

void AreaLight::initBox(){
    if(this->roomVAO == 0){
        Area area;
        std::vector<glm::vec3> returnData = area.generateData();
        float vertices[returnData.size() * 3];
        int count = 9;
        for(unsigned int i = 0; i < returnData.size(); i += 3){
            int j = i / 3;
            vertices[j * count] = returnData[i].x;
            vertices[j * count + 1] = returnData[i].y;
            vertices[j * count + 2] = returnData[i].z;
            vertices[j * count + 3] = returnData[i+1].x;
            vertices[j * count + 4] = returnData[i+1].y;
            vertices[j * count + 5] = returnData[i+1].z;
            vertices[j * count + 6] = returnData[i+2].x;
            vertices[j * count + 7] = returnData[i+2].y;
            vertices[j * count + 8] = returnData[i+2].z;
        }
        glGenVertexArrays(1, &this->roomVAO);
        glGenBuffers(1, &this->roomVBO);
        glBindVertexArray(this->roomVAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->roomVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    glBindVertexArray(this->roomVAO);
    glDrawArrays(GL_TRIANGLES, 0, 30);
    glBindVertexArray(0);
}

void AreaLight::initBlock(){
    if(this->blockVAO == 0){
        Block block;
        std::vector<glm::vec3> returnData = block.generateData();
        float vertices[returnData.size() * 3];
        int count = 9;
        for(unsigned int i = 0; i < returnData.size(); i += 3){
            int j = i / 3;
            vertices[j * count] = returnData[i].x;
            vertices[j * count + 1] = returnData[i].y;
            vertices[j * count + 2] = returnData[i].z;
            vertices[j * count + 3] = returnData[i+1].x;
            vertices[j * count + 4] = returnData[i+1].y;
            vertices[j * count + 5] = returnData[i+1].z;
            vertices[j * count + 6] = returnData[i+2].x;
            vertices[j * count + 7] = returnData[i+2].y;
            vertices[j * count + 8] = returnData[i+2].z;
        }
        glGenVertexArrays(1, &this->blockVAO);
        glGenBuffers(1, &this->blockVBO);
        glBindVertexArray(this->blockVAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->blockVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    glBindVertexArray(this->blockVAO);
    glDrawArrays(GL_TRIANGLES, 0, 60);
    glBindVertexArray(0);
}

void AreaLight::initLight(){
    std::vector<glm::vec3> ceilingWindow = {
        glm::vec3(343.0, 548.7, 227.0),
        glm::vec3(343.0, 548.7, 332.0),
        glm::vec3(213.0, 548.7, 332.0),
        glm::vec3(343.0, 548.7, 227.0),
        glm::vec3(213.0, 548.7, 332.0),
        glm::vec3(213.0, 548.7, 227.0)
    };
    this->lightPos = glm::vec3(27.8, 54.87, 27.95);
    if(this->lightVAO == 0){
        float vertices[ceilingWindow.size() * 6];
        int count = 6;
        for(unsigned int i = 0; i < ceilingWindow.size(); ++i){
            vertices[i * count] = ceilingWindow[i].x;
            vertices[i * count + 1] = ceilingWindow[i].y;
            vertices[i * count + 2] = ceilingWindow[i].z;
            vertices[i * count + 3] = 1.0f;
            vertices[i * count + 4] = 1.0f;
            vertices[i * count + 5] = 1.0f;
        }
        glGenVertexArrays(1, &this->lightVAO);
        glGenBuffers(1, &this->lightVBO);
        glBindVertexArray(this->lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->lightVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    glBindVertexArray(this->lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
