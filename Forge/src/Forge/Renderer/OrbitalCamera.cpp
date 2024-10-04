//
// Created by toor on 2024-09-12.
//

#include "OrbitalCamera.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Events/Event.h"
#include "Forge/Events/ImplEvent.h"
#include "Forge/Events/KeyCodes.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Forge {

// Constructors and Destructor
OrbitalCamera::OrbitalCamera(glm::vec3 target, int width, int height) : m_Target(target)
{
    RecalculateProjection(width, height);
    CameraRotate(90, 10);
}

glm::mat4& OrbitalCamera::GetViewProjectionMatrix()
{
    m_ViewMatrix = glm::lookAt(m_Position, m_Target, m_Up);
    m_ViewProjectionMatrix = m_ProjMatrix * m_ViewMatrix;
    return m_ViewProjectionMatrix;
}

void OrbitalCamera::SetCameraProjection(CameraProjection proj)
{
    m_CameraAtribProjection = proj;
    auto viewport_size = ApplicationStats::GetApplicationSize();
    RecalculateProjection(viewport_size.first, viewport_size.second);
}

void OrbitalCamera::SetCameraFov(int degrees)
{
    m_FOV = degrees;
}

glm::vec3 OrbitalCamera::GetCameraPosition()
{
    return m_Position;
}

glm::vec3 OrbitalCamera::GetCameraTarget()
{
    return m_Target;
}
void OrbitalCamera::Resize(int width, int height)
{
    RecalculateProjection(width, height);
}


void OrbitalCamera::CameraRotate(float yaw, float pitch)
{
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    m_Yaw = yaw;
    m_Pitch = pitch;

    m_Position = glm::vec3(m_Radius * cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
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

void OrbitalCamera::SetCameraRadius(float distance)
{
    m_Radius = distance;
}


void OrbitalCamera::SetCameraTarget(glm::vec3 target)
{
    m_Target = target;
}

// Component Interface Methods
void OrbitalCamera::OnEvent(const Event& event)
{
    if (event.GetType() == EventType::Window)
    {
    }
    if (event.GetType() == EventType::Mouse)
    {
        MouseEvent mouse_event = static_cast<const MouseEvent&>(event);
        if (mouse_event.GetAction() == Action::MouseScroll)
        {
            m_Radius = m_Radius + mouse_event.GetY() * m_ScrollSens;
            if (m_Radius <= 0)
            {
                m_Radius = 0;
            }
            m_Position =
                glm::vec3(m_Radius * cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
                          m_Radius * sin(glm::radians(m_Pitch)),
                          m_Radius * sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)));
        }
    }
}


void OrbitalCamera::OnAttach() {}

void OrbitalCamera::OnDetach() {}

void OrbitalCamera::OnUpdate(DeltaTime dt)
{
    if (m_AllowRotation)
    {
        if (Keyboard::IsKeyPressed(Key::LeftMouse))
        {
            auto mouse_delta = Mouse::GetMouseDeltaMovement();
            if (m_FirstMouseTouch)
            {
                mouse_delta.first = 0;
                mouse_delta.second = 0;
                m_FirstMouseTouch = false;
            }
            float sensitivity = m_MovementSens * dt;
            m_MovementYaw += mouse_delta.first * sensitivity;
            m_MovementPitch += mouse_delta.second * sensitivity;
            if (m_MovementPitch <= -89)
            {
                m_MovementPitch = -89;
            }
            if (m_MovementPitch >= 89)
            {
                m_MovementPitch = 89;
            }
            CameraRotate(m_MovementYaw, m_MovementPitch);
        }
        else
        {
            m_FirstMouseTouch = true;
        }
    }
}

void OrbitalCamera::RecalculateProjection(int width, int height)
{
    if (m_CameraAtribProjection == CameraProjection::PerspectiveCamera)
    {
        m_ProjMatrix = glm::perspective(
            glm::radians(m_FOV),
            static_cast<float>(width) / static_cast<float>(height),
            0.1f,
            10000.0f);
    }
    else
    {
        m_ProjMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, m_NearClip, m_FarClip);
    }
}


void OrbitalCamera::AllowMovement(bool state)
{
    m_AllowMoving = state;
}
void OrbitalCamera::AllowRotation(bool state)
{
    m_AllowRotation = state;
}

glm::mat4& OrbitalCamera::GetViewMatrix()
{
    return m_ViewMatrix;
}
glm::mat4& OrbitalCamera::GetProjectionMatrix()
{
    return m_ProjMatrix;
}


}  // namespace Forge
