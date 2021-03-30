#include "area_light.hpp"
static float deltaTime = 0.0;
static float lastFrame = 0.0;
static float fpsFrame = 0.0;
static int frame = 0;

void AreaLight::createLight(){
    Window lightWindow(this->SCR_WIDTH, this->SCR_HEIGHT, "area light");
    GLFWwindow* window = lightWindow.createWindow();
    Shader shader(this->lightVs, this->lightFs);
    glm::vec3 lightPos = glm::vec3(0.0, 1.0, 0.0);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        frame++;

        lightWindow.processInput(window, deltaTime);
        glClearColor((float) 119 / 255, (float)209 / 255, (float) 249 / 255, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f));
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)this->SCR_WIDTH / (float)this->SCR_HEIGHT, 0.1f, 300.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::vec3 viewPos = camera.Position;
        shader.setMat4("projection", projection);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setVec3("viewPos", viewPos);
        shader.setVec3("lightPos", lightPos);
        shader.setBool("is_light", true);
        this->initRoom();

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

void AreaLight::initRender(){
    
}

void AreaLight::initRoom(){
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
