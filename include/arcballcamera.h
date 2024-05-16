#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class ArcballCamera {
public:
    ArcballCamera(float width, float height, glm::vec3 target);
    void SetViewportSize(float width, float height);
    void StartDrag(float x, float y);
    void Drag(float x, float y);
    void Zoom(float zoomFactor);
    glm::mat4 GetViewMatrix() const;

private:
    void UpdateViewMatrix();
    glm::vec3 ScreenToSphere(float x, float y) const;
    glm::quat ComputeRotation(const glm::vec3& from, const glm::vec3& to) const;

    float viewportWidth, viewportHeight, radius, zoom, rotationAngle;
    glm::vec3 target, lastMousePos, eye, camPosition, rotationAxis;
    glm::mat4 viewMatrix, rotMatrix;
    glm::quat rotation;
};