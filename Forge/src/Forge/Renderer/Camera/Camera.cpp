

#include "Camera.h"

#include "Forge/Core/Log/Log.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/fwd.hpp"

namespace Forge {

// Default constructor
Camera::Camera(CameraProjection projection, glm::vec3 position)
    : m_Position(position) {
    m_CameraProjection = projection;
}

Camera::Camera(const Camera& other) {
    DeepCopy(other);
}

Camera& Camera::operator=(const Camera& other) {
    if (this != &other) {
        DeepCopy(other);
    }
    return *this;
}

// Destructor
Camera::~Camera() {}

void Camera::RecalculateProjection(uint32_t width, uint32_t height) {
    if (m_CameraProjection == CameraProjection::PerspectiveCamera) {
        m_ProjMatrix =
            glm::perspective(glm::radians(m_Fov), static_cast<float>(width) / static_cast<float>(height), m_NearClip, m_FarClip);
    } else if (m_CameraProjection == CameraProjection::ScreenSpaceCamera) {
        float left = 0.0f;
        float right = static_cast<float>(width);
        float bottom = static_cast<float>(height);
        float top = 0.0f;

        m_ProjMatrix = glm::ortho(left, right, bottom, top, m_NearClip, m_FarClip);
    } else {
        float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        float orthoHeight = static_cast<float>(height);
        float orthoWidth = orthoHeight * aspectRatio;
        m_ProjMatrix =
            glm::ortho(-orthoWidth / 2.0f, orthoWidth / 2.0f, -orthoHeight / 2.0f, orthoHeight / 2.0f, m_NearClip, m_FarClip);
    }
    m_ViewMatrix = glm::mat4(1.0);
}

void Camera::DeepCopy(const Camera& other) {
    m_RotationYaw = other.m_RotationYaw;
    m_RotationPitch = other.m_RotationPitch;
    m_RotationSens = other.m_RotationSens;
    m_ProjMatrix = other.m_ProjMatrix;
    m_ViewMatrix = other.m_ViewMatrix;
    m_ViewProjectionMatrix = other.m_ViewProjectionMatrix;
    m_Position = other.m_Position;
    m_Target = other.m_Target;
    m_CameraProjection = other.m_CameraProjection;
}

glm::mat4& Camera::GetViewProjectionMatrix() {
    m_ViewProjectionMatrix = m_ProjMatrix * m_ViewMatrix;
    return m_ViewProjectionMatrix;
}
glm::mat4& Camera::GetViewMatrix() {
    return m_ViewMatrix;
}

glm::mat4& Camera::GetProjectionMatrix() {
    return m_ProjMatrix;
}

glm::vec3& Camera::GetCameraPosition() {
    return m_Position;
}
glm::vec3& Camera::GetCameraTarget() {
    return m_Target;
}

} // namespace Forge
