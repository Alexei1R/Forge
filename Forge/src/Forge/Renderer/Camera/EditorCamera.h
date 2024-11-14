//
// Created by toor on 2024-09-12.
//


#ifndef ORBITALCAMERA_H
#define ORBITALCAMERA_H

#include <glm/glm.hpp>
#include "Forge/Core/Module/Module.h"
#include "Forge/Core/Time.h"
#include "Forge/Events/Event.h"
#include "Camera.h"

namespace Forge {


class EditorCamera : public Module, public Camera
{
public:
    EditorCamera(glm::vec3 target = glm::vec3(0.0f), int width = 1920, int height = 1080);
    virtual ~EditorCamera() = default;


    // NOTE: Camera
    glm::mat4& GetViewProjectionMatrix() override;
    glm::mat4& GetViewMatrix() override;
    glm::mat4& GetProjectionMatrix() override;


    // NOTE: Module
    void OnEvent(const Event& event) override;
    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(DeltaTime dt) override;


    void SetCameraProjection(CameraProjection proj);
    void SetCameraFov(int degrees);
    glm::vec3 GetCameraPosition();
    glm::vec3 GetCameraTarget();
    void Resize(int width, int height);
    void AllowMovement(bool state);
    void AllowRotation(bool state);
    void AllowScroll(bool state);

    float GetNearClip()
    {
        return m_NearClip;
    }
    float GetFarClip()
    {
        return m_FarClip;
    }

    void CameraRotate(float yaw, float pitch);
    void SetCameraRadius(float dist);
    void SetCameraTarget(const glm::vec3& target);


private:
    void RecalculateProjection(int width, int height);

    CameraProjection m_CameraAtribProjection = CameraProjection::PerspectiveCamera;
    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, -3.0f);
    glm::vec3 m_Target = glm::vec3(0.0f);
    glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_Right;
    glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float m_Yaw = 90.0f;
    float m_Pitch = 10.0f;
    float m_NearClip = 0.001f;
    float m_FarClip = 1000.0f;
    float m_FOV = 45.0f;
    float m_Radius = 5.0f;

    glm::mat4 m_ProjMatrix = glm::mat4(1.0f);
    glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
    glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
    glm::mat4 m_ScreenSpaceMatrix = glm::mat4(1.0f);

    bool m_FirstMouseTouch = true;

    float m_RotationYaw = 90.0f;
    float m_RotationPitch = 10.0f;
    float m_RotationSens = 0.08f;

    float m_ScrollSens = 1.0f;

    bool m_AllowMoving = true;
    bool m_AllowRotation = true;
    bool m_AllowScroll = true;


    float m_MovementSpeed = 0.0008;
    bool m_FirstMouseTouchMovement = true;


    int m_Width;
    int m_Height;
};

}  // namespace Forge

#endif  // ORBITALCAMERA_H
