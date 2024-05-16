#include "arcballcamera.h"

#include <iostream>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/scalar_constants.hpp>

// Code reference from https://github.com/oguz81/ArcballCamera/blob/main/main.cpp

static
std::ostream& operator<<(std::ostream& os, const glm::vec3& v) {
    os << v.x << " : " << v.y << " : " << v.z;
    return os;
}

static
glm::quat& axis(glm::quat& q, glm::vec3 v) {
    q.x = v.x;
    q.y = v.y;
    q.z = v.z;
    return q;
}

static
glm::vec3 axis(const glm::quat& q) {
    return glm::vec3(q.x, q.y, q.z);
}

ArcballCamera::ArcballCamera(float width, float height) : 
    viewportWidth{ width },
    viewportHeight{ height },
    cosValue_2{},
    angle{ glm::pi<float>() },
    startPos{},
    currentPos{},
    rotationalAxis{ glm::vec3(1.0f, 0.0f, 0.0f) },
    rotationalAxis_2 {},
    position{ glm::vec3(0.0f, 0.0f, -10.0f) },
    currentQuaternion{},
    lastQuaternion {glm::quat {0.0f, 1.0f, 0.0f, 0.0f}}
{
}

void ArcballCamera::SetViewportSize(float width, float height) {
    viewportWidth = width;
    viewportHeight = height;
}

void ArcballCamera::StartDrag(float x, float y) {
    startPos = ScreenToSphere(x, y);
}

void ArcballCamera::StopDrag() {
    lastQuaternion.w = cosValue_2;
    axis(lastQuaternion, rotationalAxis_2);
}

void ArcballCamera::Drag(float x, float y) {
    currentPos = ScreenToSphere(x, y);
    Rotation();
}

glm::mat4 ArcballCamera::GetViewMatrix() const {
    glm::mat4 view = glm::mat4(1.0f);// this command must be in the loop. Otherwise, the object moves if there is a glm::rotate func in the lop.    
    view = glm::translate(view, position);// this, too.  
    view = glm::rotate(view, angle, rotationalAxis);
    return view;
}

glm::vec3 ArcballCamera::ScreenToSphere(float x, float y) const {
    const float ndcX = (2.0f * x - viewportWidth) / viewportWidth;
    const float ndcY = (viewportHeight - 2.0f * y) / viewportHeight;
    const float zSq = 1.0f - ndcX * ndcX - ndcY * ndcY;
    const float z = zSq > 0.0f ? sqrt(zSq) : 0.0f;
    return glm::normalize(glm::vec3(ndcX, ndcY, z));
}

void ArcballCamera::Rotation() {
    const auto startPosUnitVector = glm::normalize(startPos);
    const auto currentPosUnitVector = glm::normalize(currentPos);

    axis(currentQuaternion, glm::normalize(glm::cross(startPos, currentPos)));

    auto cosValue = glm::dot(startPosUnitVector, currentPosUnitVector);
    cosValue = (cosValue > 1.0f) ? 1.0f : cosValue;

    const auto theta = acos(cosValue);
    const auto halfTheta = theta * 0.5f;
    const auto sinHalfTheta = sin(halfTheta);

    currentQuaternion.w = cos(halfTheta);
    currentQuaternion.x = currentQuaternion.x * sinHalfTheta;
    currentQuaternion.y = currentQuaternion.y * sinHalfTheta;
    currentQuaternion.z = currentQuaternion.z * sinHalfTheta;

    cosValue_2 = (currentQuaternion.w * lastQuaternion.w) - glm::dot(axis(currentQuaternion), axis(lastQuaternion));
    const auto temporaryVector = glm::cross(axis(currentQuaternion), axis(lastQuaternion));

    rotationalAxis_2.x = (currentQuaternion.w * lastQuaternion.x) +
        (lastQuaternion.w * currentQuaternion.x) +
        temporaryVector.x;

    rotationalAxis_2.y = (currentQuaternion.w * lastQuaternion.y) +
        (lastQuaternion.w * currentQuaternion.y) +
        temporaryVector.y;

    rotationalAxis_2.z = (currentQuaternion.w * lastQuaternion.z) +
        (lastQuaternion.w * currentQuaternion.z) +
        temporaryVector.z;

    angle = acos(cosValue_2) * 2.0f;
    const auto halfAngle = angle * 0.5f;

    rotationalAxis.x = rotationalAxis_2.x / sin(halfAngle);
    rotationalAxis.y = rotationalAxis_2.y / sin(halfAngle);
    rotationalAxis.z = rotationalAxis_2.z / sin(halfAngle);
}
