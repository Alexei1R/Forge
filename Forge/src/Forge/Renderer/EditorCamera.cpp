//
// Created by toor on 2024-09-12.
//


#include "EditorCamera.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Events/Event.h"
#include "Forge/Events/ImplEvent.h"
#include "Forge/Events/KeyCodes.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Forge {

EditorCamera::EditorCamera(glm::vec3 target, int width, int height) : m_Target(target)
{
    RecalculateProjection(width, height);
    CameraRotate(m_Yaw, m_Pitch);
}

glm::mat4& EditorCamera::GetViewProjectionMatrix()
{
    m_ViewMatrix = glm::lookAt(m_Position, m_Target, m_Up);
    m_ViewProjectionMatrix = m_ProjMatrix * m_ViewMatrix;
    return m_ViewProjectionMatrix;
}

void EditorCamera::SetCameraProjection(CameraProjection proj)
{
    m_CameraAtribProjection = proj;
    auto viewport_size = ApplicationStats::GetApplicationSize();
    RecalculateProjection(viewport_size.first, viewport_size.second);
}

void EditorCamera::SetCameraFov(int degrees)
{
    m_FOV = static_cast<float>(degrees);
    auto viewport_size = ApplicationStats::GetApplicationSize();
    RecalculateProjection(viewport_size.first, viewport_size.second);
}

glm::vec3 EditorCamera::GetCameraPosition()
{
    return m_Position;
}

glm::vec3 EditorCamera::GetCameraTarget()
{
    return m_Target;
}

void EditorCamera::Resize(int width, int height)
{
    RecalculateProjection(width, height);
}

void EditorCamera::CameraRotate(float yaw, float pitch)
{
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    m_Yaw = yaw;
    m_Pitch = pitch;

    m_Position =
        m_Target + glm::vec3(m_Radius * cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
                             m_Radius * sin(glm::radians(m_Pitch)),
                             m_Radius * sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)));

    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

void EditorCamera::SetCameraRadius(float distance)
{
    m_Radius = distance;
    // Recalculate the camera position
    CameraRotate(m_Yaw, m_Pitch);
}

void EditorCamera::SetCameraTarget(const glm::vec3& target)
{
    m_Target = target;
    // Recalculate the camera position
    CameraRotate(m_Yaw, m_Pitch);
}

void EditorCamera::OnEvent(const Event& event)
{
    if (m_AllowScroll)
    {
        if (event.GetType() == EventType::Mouse)
        {
            const MouseEvent& mouse_event = static_cast<const MouseEvent&>(event);
            if (mouse_event.GetAction() == Action::MouseScroll)
            {
                m_Radius += mouse_event.GetY() * -m_ScrollSens;
                if (m_Radius <= 0.1f)
                {
                    m_Radius = 0.1f;
                }
                m_Position =
                    m_Target +
                    glm::vec3(m_Radius * cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
                              m_Radius * sin(glm::radians(m_Pitch)),
                              m_Radius * sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)));
            }
        }
    }
}

void EditorCamera::OnAttach() {}

void EditorCamera::OnDetach() {}

void EditorCamera::OnUpdate(DeltaTime dt)
{
    if (m_AllowRotation || m_AllowMoving)
    {
        if (Keyboard::IsKeyPressed(Key::LeftShift))
        {
            if (Keyboard::IsKeyPressed(Key::LeftMouse))
            {
                auto mouse_delta = Mouse::GetMouseDeltaMovement();
                if (m_FirstMouseTouchMovement)
                {
                    mouse_delta.first = 0.0f;
                    mouse_delta.second = 0.0f;
                    m_FirstMouseTouchMovement = false;
                }
                m_Target += (float)mouse_delta.first * (m_Right * m_MovementSpeed * dt);
                m_Position += (float)mouse_delta.first * (m_Right * m_MovementSpeed * dt);

                m_Target += (float)mouse_delta.second * (m_Up * m_MovementSpeed * dt);
                m_Position += (float)mouse_delta.second * (m_Up * m_MovementSpeed * dt);
            }
            else
            {
                m_FirstMouseTouchMovement = true;
            }
        }
        else
        {
            if (Keyboard::IsKeyPressed(Key::LeftMouse))
            {
                auto mouse_delta = Mouse::GetMouseDeltaMovement();
                if (m_FirstMouseTouch)
                {
                    mouse_delta.first = 0.0f;
                    mouse_delta.second = 0.0f;
                    m_FirstMouseTouch = false;
                }
                float sensitivity = m_RotationSens * dt;
                m_RotationYaw += mouse_delta.first * sensitivity;
                m_RotationPitch += mouse_delta.second * sensitivity;

                if (m_RotationPitch <= -89.0f)
                    m_RotationPitch = -89.0f;
                if (m_RotationPitch >= 89.0f)
                    m_RotationPitch = 89.0f;

                CameraRotate(m_RotationYaw, m_RotationPitch);
            }
            else
            {
                m_FirstMouseTouch = true;
            }
        }
    }
}

void EditorCamera::RecalculateProjection(int width, int height)
{
    if (m_CameraAtribProjection == CameraProjection::PerspectiveCamera)
    {
        m_ProjMatrix = glm::perspective(
            glm::radians(m_FOV),
            static_cast<float>(width) / static_cast<float>(height),
            m_NearClip,
            m_FarClip);
    }
    else
    {
        float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        float orthoHeight = 10.0f;
        float orthoWidth = orthoHeight * aspectRatio;
        m_ProjMatrix = glm::ortho(
            -orthoWidth / 2.0f,
            orthoWidth / 2.0f,
            -orthoHeight / 2.0f,
            orthoHeight / 2.0f,
            m_NearClip,
            m_FarClip);
    }
}

void EditorCamera::AllowMovement(bool state)
{
    m_AllowMoving = state;
}

void EditorCamera::AllowRotation(bool state)
{
    m_AllowRotation = state;
}

void EditorCamera::AllowScroll(bool state)
{
    m_AllowScroll = state;
}

glm::mat4& EditorCamera::GetViewMatrix()
{
    m_ViewMatrix = glm::lookAt(m_Position, m_Target, m_Up);
    return m_ViewMatrix;
}

glm::mat4& EditorCamera::GetProjectionMatrix()
{
    return m_ProjMatrix;
}

}  // namespace Forge
