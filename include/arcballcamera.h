#pragma once

#include <glm/glm.hpp>

class ArcballCamera {
public:
    ArcballCamera(float width, float height);
    void Rotate(float x, float y);
    void Zoom(float dval);
    void Translate(float dx, float dy);
    glm::mat4 GetViewMatrix() const;

private:
    glm::vec3 target, eye, front, up;
    float yaw, pitch;
};