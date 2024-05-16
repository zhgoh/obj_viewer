#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class ArcballCamera {
public:
    ArcballCamera(float width, float height);
    void SetViewportSize(float width, float height);
    void StartDrag(float x, float y);
    void StopDrag();
    void Drag(float x, float y);
    glm::mat4 GetViewMatrix() const;

private:
    glm::vec3 ScreenToSphere(float x, float y) const;
    void Rotation();

    float viewportWidth, viewportHeight, cosValue_2, angle;
    glm::vec3 startPos, currentPos, rotationalAxis, rotationalAxis_2, position;
    glm::quat currentQuaternion, lastQuaternion;
};