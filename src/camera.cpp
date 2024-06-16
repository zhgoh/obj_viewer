#include "camera.hpp"

#include <iostream>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

// Camera code referenced from
// Reference: https://github.com/dvsku/devue

static float SPEED_TRANSLATE = 0.005f;
static float SPEED_ROTATE = 0.1f;

static
std::ostream& operator<<(std::ostream& os, const glm::vec3& v) 
{
    os << v.x << " : " << v.y << " : " << v.z;
    return os;
}

Camera::Camera(float width, float height) :
    target {glm::vec3{0.0f, 0.0f, 0.0f}},
    eye(glm::vec3{ 0.0f, 0.0f, -1.0f }),
    front{glm::vec3{0.0f, 0.0f, -1.0f}},
    up{ glm::vec3{0.0f, 1.0f, 0.0f} },
    lastMousePos{glm::vec2{0.0f, 0.0f}},
    yaw {}, pitch{}
{
    Translate(0.0f, 0.0f);
    SetRotation(0.0f, 0.0f);
    Zoom(-2.0f);
}

void Camera::StartDrag(double mouseX, double mouseY) 
{
    lastMousePos = glm::vec2{ 
        static_cast<float>(mouseX), 
        static_cast<float>(mouseY) 
    };
}

void Camera::Drag(double mouseX, double mouseY) 
{
    float dx = lastMousePos.x - static_cast<float>(mouseX);
    float dy = lastMousePos.y - static_cast<float>(mouseY);
    lastMousePos = glm::vec2{ mouseX, mouseY };
    SetRotation(dx, dy);
}

void Camera::SetRotation(float x, float y) 
{
    yaw -= x * SPEED_ROTATE;
    pitch -= y * SPEED_ROTATE;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
        
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    float distance = glm::length(eye - target);

    const auto cosPitch = cos(glm::radians(pitch));
    eye.x = target.x + distance * cos(glm::radians(yaw)) * cosPitch;
    eye.y = target.y + distance * sin(glm::radians(pitch));
    eye.z = target.z + distance * sin(glm::radians(yaw)) * cosPitch;

    front = glm::normalize(target - eye);
}

glm::mat4 Camera::GetViewMatrix() const 
{
    return glm::lookAt(eye, eye + front, glm::vec3{ 0.0f, 1.0f, 0.0f });;
}

void Camera::Zoom(float zoom) 
{
    float distance = glm::length(eye - target);

    bool isBlocked = 
        (distance >= 110.0f && zoom < -20.0f) ||
        (distance <= 3.0f && zoom > 0.0f);

    if (isBlocked)
    {
        return;
    }
        

    distance -= zoom;

    glm::vec3 direction = glm::normalize(eye - target);
    eye = target + direction * distance;
}

void Camera::Translate(float dx, float dy) 
{
    glm::vec3 cameraRight = glm::normalize(glm::cross(front, up));

    eye -= dx * SPEED_TRANSLATE * cameraRight;
    target -= dx * SPEED_TRANSLATE * cameraRight;

    eye -= dy * SPEED_TRANSLATE * up;
    target -= dy * SPEED_TRANSLATE * up;
}