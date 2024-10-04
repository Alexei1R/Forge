//
// Created by toor on 2024-09-12.
//


#ifndef ORBITALCAMERA_H
#define ORBITALCAMERA_H

#include "Camera.h"
#include <glm/glm.hpp>
#include "Forge/Events/Event.h"
#include "Forge/Core/Components/Component.h"

namespace Forge {

class OrbitalCamera : public Component, public Camera
{
public:
    // Constructors and Destructor
    OrbitalCamera(glm::vec3 target = glm::vec3(0.0f), int width = 1920, int height = 1080);
    virtual ~OrbitalCamera() = default;

    // Camera Interface Methods
    glm::mat4& GetViewProjectionMatrix() override;
    glm::mat4& GetViewMatrix() override;
    glm::mat4& GetProjectionMatrix() override;
    void SetCameraProjection(CameraProjection proj) override;
    void SetCameraFov(int degrees) override;
    glm::vec3 GetCameraPosition() override;
    glm::vec3 GetCameraTarget() override;
    void Resize(int width, int height) override;
    void AllowMovement(bool state) override;
    void AllowRotation(bool state) override;
    void AllowScroll(bool state) override;

    float GetNearClip() override
    {
        return m_NearClip;
    }
    float GetFarClip() override
    {
        return m_FarClip;
    }

    // Camera Control Methods
    void CameraRotate(float yaw, float pitch);
    void SetCameraRadius(float dist);
    void SetCameraTarget(const glm::vec3& target);

    // Component Interface Methods
    void OnEvent(const Event& event) override;
    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(DeltaTime dt) override;

private:
    // Helper Methods
    void RecalculateProjection(int width, int height);

    // Camera Attributes
    CameraProjection m_CameraAtribProjection = CameraProjection::PerspectiveCamera;
    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, -3.0f);
    glm::vec3 m_Target = glm::vec3(0.0f);
    glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_Right;
    glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Camera Parameters
    float m_Yaw = 90.0f;
    float m_Pitch = 0.0f;
    float m_NearClip = 0.1f;
    float m_FarClip = 1000.0f;
    float m_FOV = 45.0f;
    float m_Radius = 5.0f;

    // Matrices
    glm::mat4 m_ProjMatrix = glm::mat4(1.0f);
    glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
    glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

    // State
    bool m_FirstMouseTouch = true;

    // Rotation Settings
    float m_MovementYaw = 90.0f;
    float m_MovementPitch = 10.0f;
    float m_MovementSens = 30.0f;

    // Scroll Settings
    float m_ScrollSens = 0.5f;

    // Controls
    bool m_AllowMoving = false;
    bool m_AllowRotation = false;
    bool m_AllowScroll = false;
};

}  // namespace Forge

#endif  // ORBITALCAMERA_H
