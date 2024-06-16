#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    Camera(float width, float height);
    void StartDrag(double mouseX, double mouseY);
    void Drag(double mouseX, double mouseY);
    void Zoom(float zoom);
    void Translate(float dx, float dy);
    glm::mat4 GetViewMatrix() const;
    

private:
    void SetRotation(float x, float y);
    

    glm::vec3 target, eye, front, up;
    glm::vec2 lastMousePos;
    float yaw, pitch;
};