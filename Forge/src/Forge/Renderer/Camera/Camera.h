
#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>

#include "glm/fwd.hpp"
#include "glm/glm.hpp"

namespace Forge {

enum class CameraProjection {
    PerspectiveCamera,
    OrthographicCamera,
    ScreenSpaceCamera,
    Test,
};

// NOTE: Forward Declaration
class CameraController;

class Camera {
public:
    Camera(CameraProjection projection, glm::vec3 position = glm::vec3(0.0, 0.0, 8.0));
    Camera(const Camera& other);
    Camera& operator=(const Camera& other);
    ~Camera();

    void RecalculateProjection(uint32_t width, uint32_t height);

    glm::vec3& GetCameraPosition();
    glm::vec3& GetCameraTarget();

    // NOTE: Camera
    glm::mat4& GetViewProjectionMatrix();
    glm::mat4& GetViewMatrix();
    glm::mat4& GetProjectionMatrix();

    friend class CameraController;

protected:
    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, -3.0f);
    glm::vec3 m_Target = glm::vec3(0.0f);
    float m_RotationYaw = 90.0f;
    float m_RotationPitch = 10.0f;

    float m_RotationSens = 0.2f;
    float m_ScrollSens = 1.0f;
    float m_MovementSpeed = 0.0008;
    float m_Radius = 5.0f;

private:
    void DeepCopy(const Camera& other);

    glm::mat4 m_ProjMatrix = glm::mat4(1.0f);
    glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
    glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

    CameraProjection m_CameraProjection;
    float m_NearClip = 0.001f;
    float m_FarClip = 10000.0f;
    float m_Fov = 45.0f;
};

} // namespace Forge

#endif
