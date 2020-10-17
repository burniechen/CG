//
//  Camera.cpp
//  Test
//
//  Created by 陳柏丞 on 2020/10/15.
//
#include <iostream>
#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup)
{
    Position = position;
    WorldUp = worldup;
    Forward = glm::normalize(target - position);
    Right = glm::normalize(glm::cross(Forward, WorldUp));
    Up = glm::normalize(glm::cross(Forward, Right));
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup)
{
    Position = position;
    WorldUp = worldup;
    Pitch = pitch;
    Yaw = yaw;
    Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
    Forward.y = glm::sin(Pitch);
    Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
    Right = glm::normalize(glm::cross(Forward, WorldUp));
    Up = glm::normalize(glm::cross(Forward, Right));
}

Camera::~Camera(){}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Forward, WorldUp);
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY)
{
    Pitch -= deltaY * SenseY;
    Yaw -= deltaX * SenseX;
    UpdateCameraVectors();
}

void Camera::ProcessKeyMoveX(bool type)
{

    if (type) Yaw += 1.0f * SenseX * 10; // right
    else Yaw -= 1.0f * SenseX * 10; //left
    
    UpdateCameraVectors();
}

void Camera::ProcessKeyMoveY(bool type)
{
    if (type) Pitch += 1.0f * SenseY * 10; // up
    else Pitch -= 1.0f * SenseY * 10; // down

    UpdateCameraVectors();
}

void Camera::UpdateCameraPos()
{
    Position += Forward * speedZ * 0.1f;
}

void Camera::UpdateCameraVectors()
{
    Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
    Forward.y = glm::sin(Pitch);
    Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
    Right = glm::normalize(glm::cross(Forward, WorldUp));
    Up = glm::normalize(glm::cross(Forward, Right));
}
