//
//  camera.h
//  opengl
//  摄像机类
//  Created by adsionli on 2020/10/14.
//  Copyright © 2020 adsionli. All rights reserved.
//

#ifndef camera_h
#define camera_h
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    TOP,
    DOWN
};
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;
class Camera{
public:
    //向量属性 1.位置向量 2.方向向量 3.上向量 4.右轴 5.上轴
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    //角度 1.航偏角 2.俯仰角
    float Yaw;
    float Pitch;
    //摄像机属性 1.鼠标移动速度 2.鼠标灵敏度 3.视野范围
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    //初始化摄像机属性
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    //回调时触发摄像机属性更新
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
   {
       Position = glm::vec3(posX, posY, posZ);
       WorldUp = glm::vec3(upX, upY, upZ);
       Yaw = yaw;
       Pitch = pitch;
       updateCameraVectors();
   }
    //获取观察矩阵
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }
    //键盘输入事件
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        //计算所需移动距离，避免移动距离不相同。
        float velocity = MovementSpeed * deltaTime * 6.0;
        switch (direction) {
            case FORWARD:
                Position += Front * velocity;
                break;
            case BACKWARD:
                Position -= Front * velocity;
            break;
            case LEFT:
                Position -= Right * velocity;
            break;
            case RIGHT:
                Position += Right * velocity;
            break;
            case TOP:
                Position.y += Up.y * velocity;
                break;
            case DOWN:
                Position.y -= Up.y * velocity;
                break;
            default:
                break;
        }
    }
    //鼠标移动事件
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;
        //修改俯仰角和航偏角的值
        Yaw   += xoffset;
        Pitch += yoffset;
        //设置俯仰角的最大值，避免超过阈值，发生莫名其妙的错误
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        updateCameraVectors();
    }
    //鼠标滚轴事件
    void ProcessMouseScroll(float yoffset)
    {
        //视野更新
        Zoom -= (float)yoffset;
        //设置视野阈值
        if (Zoom < 1.0f)
        {
            Zoom = 1.0f;
        }else if (Zoom > 45.0f)
        {
            Zoom = 45.0f;
        }
    }
private:
    //更新摄像机的方向向量以及上向量和右轴的位置
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        //更新方向向量
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        //通过叉乘获取右轴
        Right = glm::normalize(glm::cross(Front, WorldUp));
        //通过叉乘获取上轴
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};


#endif /* camera_h */
