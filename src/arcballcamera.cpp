#include "arcballcamera.h"

#include <iostream>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Reference: https://github.com/dvsku/devue

static float SPEED_TRANSLATE = 0.005f;
static float SPEED_ROTATE = 0.1f;

static
std::ostream& operator<<(std::ostream& os, const glm::vec3& v) {
    os << v.x << " : " << v.y << " : " << v.z;
    return os;
}


ArcballCamera::ArcballCamera(float width, float height) :
    target {glm::vec3{0.0f, 0.0f, 0.0f}},
    eye(glm::vec3{ 0.0f, 0.0f, -1.0f }),
    front{glm::vec3{0.0f, 0.0f, -1.0f}},
    up{ glm::vec3{0.0f, 1.0f, 0.0f} }
{
    Translate(0.0f, 0.0f);
    Rotate(0.0f, 0.0f);
    Zoom(-2.0f);
}

void ArcballCamera::Rotate(float x, float y) {
    yaw -= x * SPEED_ROTATE;
    pitch -= y * SPEED_ROTATE;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    float distance = glm::length(eye - target);
    eye.x = target.x + distance * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    eye.y = target.y + distance * sin(glm::radians(pitch));
    eye.z = target.z + distance * sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(target - eye);
}

glm::mat4 ArcballCamera::GetViewMatrix() const {
    return glm::lookAt(eye, eye + front, glm::vec3{ 0.0f, 1.0f, 0.0f });;
}

void ArcballCamera::Zoom(float dval) {
    float distance = glm::length(eye - target);

    bool zoom_blocked = (static_cast<int>(distance) >= 1000.0f && dval < 0.0f) ||
        (static_cast<int>(distance) <= 0.1f && dval > 0);

    if (zoom_blocked) return;

    distance -= dval;

    glm::vec3 direction = glm::normalize(eye - target);
    eye = target + direction * distance;
}

void ArcballCamera::Translate(float dx, float dy) {
    glm::vec3 cameraRight = glm::normalize(glm::cross(front, up));

    eye -= dx * SPEED_TRANSLATE * cameraRight;
    target -= dx * SPEED_TRANSLATE * cameraRight;

    eye -= dy * SPEED_TRANSLATE * up;
    target -= dy * SPEED_TRANSLATE * up;
}