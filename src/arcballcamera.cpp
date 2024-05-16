#include <utility>
#include <iostream>

#include "arcballcamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::ostream& operator<<(std::ostream& os, const glm::vec3& v) {
    os << v.x << " : " << v.y << " : " << v.z;
    return os;
}

ArcballCamera::ArcballCamera(float width, float height, glm::vec3 target)
    : viewportWidth{ width },
    viewportHeight{ height },
    radius{ 10 },
    zoom{ 1.0f },
    target{ target },
    lastMousePos{},
    eye{ target + glm::vec3(0.0f, 0.0f, radius) * zoom },
    viewMatrix{},
    rotation{ glm::quat {1.0f, 0.0f, 0.0f, 0.0f} }
{
    UpdateViewMatrix();
}

void ArcballCamera::SetViewportSize(float width, float height) {
    viewportWidth = width;
    viewportHeight = height;
    radius = std::min(width * 0.5f, height * 0.5f);
}

void ArcballCamera::StartDrag(float x, float y) {
    lastMousePos = ScreenToSphere(x, y);
}

void ArcballCamera::Drag(float x, float y) {
    glm::vec3 currentMousePos = ScreenToSphere(x, y);
    glm::quat q = ComputeRotation(lastMousePos, currentMousePos);
    rotation = q * rotation;
    // eye = q * eye;
    lastMousePos = currentMousePos;
    UpdateViewMatrix();
}

void ArcballCamera::Zoom(float zoomFactor) {
    radius *= zoomFactor;
    UpdateViewMatrix();
}

glm::mat4 ArcballCamera::GetViewMatrix() const {
    return viewMatrix;
}

void ArcballCamera::UpdateViewMatrix() {
    glm::mat4 rotationMatrix = glm::mat4_cast(glm::normalize(rotation));
    glm::vec3 eye = target + glm::vec3(0.0f, 0.0f, radius) * zoom;
    eye = glm::vec3(rotationMatrix * glm::vec4(eye, 1.0f));
    viewMatrix = glm::lookAt(eye, target, glm::vec3(0, 1.0f, 0));
}

glm::vec3 ArcballCamera::ScreenToSphere(float x, float y) const {
    float ndcX = (2.0f * x - viewportWidth) / viewportWidth;
    float ndcY = (viewportHeight - 2.0f * y) / viewportHeight;
    ndcX = -ndcX;
    ndcY = -ndcY;
    float z2 = 1.0f - ndcX * ndcX - ndcY * ndcY;
    float z = z2 > 0.0f ? sqrtf(z2) : 0.0f;
    return glm::normalize(glm::vec3(ndcX, ndcY, z));
}

glm::quat ArcballCamera::ComputeRotation(const glm::vec3& from, const glm::vec3& to) const {
    if (glm::length(from) == 0.0f || glm::length(to) == 0.0f) {
        return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    }

    glm::vec3 fromNorm = glm::normalize(from);
    glm::vec3 toNorm = glm::normalize(to);

    float cosTheta = glm::dot(fromNorm, toNorm);
    glm::vec3 rotationAxis;

    if (cosTheta < -1 + 0.001f) {
        rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), fromNorm);
        if (glm::length(rotationAxis) < 0.01f) {
            rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), fromNorm);
        }
        rotationAxis = glm::normalize(rotationAxis);
        return glm::angleAxis(glm::pi<float>(), rotationAxis);
    }

    rotationAxis = glm::cross(fromNorm, toNorm);
    float s = sqrt((1 + cosTheta) * 2);
    float invs = 1 / s;

    return glm::quat(s * 0.5f, rotationAxis.x * invs, rotationAxis.y * invs, rotationAxis.z * invs);
}
