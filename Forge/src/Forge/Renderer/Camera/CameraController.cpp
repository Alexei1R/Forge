

#include "CameraController.h"
#include "Forge/Events/Event.h"
#include "Forge/Events/KeyCodes.h"

#include "Forge/Renderer/Camera/Camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"


namespace Forge {


glm::vec3 CameraController::m_Right = glm::vec3(0.0f);
glm::vec3 CameraController::m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 CameraController::m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 CameraController::m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
bool CameraController::m_FirstMouseTouch = true;
bool CameraController::m_FirstMouseTouchMovement = true;

void CameraController::Update(std::shared_ptr<Camera>& camera)
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


            camera->m_Target +=
                (float)mouse_delta.first * (m_Right * camera->m_MovementSpeed) * (camera->m_Radius);
            camera->m_Position +=
                (float)mouse_delta.first * (m_Right * camera->m_MovementSpeed) * (camera->m_Radius);

            camera->m_Target +=
                (float)mouse_delta.second * (m_Up * camera->m_MovementSpeed) * (camera->m_Radius);
            camera->m_Position +=
                (float)mouse_delta.second * (m_Up * camera->m_MovementSpeed) * (camera->m_Radius);
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
            float sensitivity = camera->m_RotationSens;
            camera->m_RotationYaw += mouse_delta.first * sensitivity;
            camera->m_RotationPitch += mouse_delta.second * sensitivity;

            if (camera->m_RotationPitch <= -89.0f)
                camera->m_RotationPitch = -89.0f;
            if (camera->m_RotationPitch >= 89.0f)
                camera->m_RotationPitch = 89.0f;

            /*camera->otate(camera->m_RotationYaw, camera->m_RotationPitch);*/

            if (camera->m_RotationPitch > 89.0f)
                camera->m_RotationPitch = 89.0f;
            if (camera->m_RotationPitch < -89.0f)
                camera->m_RotationPitch = -89.0f;

            camera->m_RotationYaw = fmod(camera->m_RotationYaw, 360.0f);


            camera->m_Position =
                camera->m_Target +
                glm::vec3(camera->m_Radius * cos(glm::radians(camera->m_RotationYaw)) *
                              cos(glm::radians(camera->m_RotationPitch)),
                          camera->m_Radius * sin(glm::radians(camera->m_RotationPitch)),
                          camera->m_Radius * sin(glm::radians(camera->m_RotationYaw)) *
                              cos(glm::radians(camera->m_RotationPitch)));

            glm::vec3 front;
            front.x = cos(glm::radians(camera->m_RotationYaw)) *
                      cos(glm::radians(camera->m_RotationPitch));
            front.y = sin(glm::radians(camera->m_RotationPitch));
            front.z = sin(glm::radians(camera->m_RotationYaw)) *
                      cos(glm::radians(camera->m_RotationPitch));
            m_Front = glm::normalize(front);
            m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
            m_Up = glm::normalize(glm::cross(m_Right, m_Front));
        }
        else
        {
            m_FirstMouseTouch = true;
        }
    }

    auto [scrollX, scrollY] = Mouse::GetMouseDeltaScroll();


    if (camera->m_CameraProjection != CameraProjection::ScreenSpaceCamera)
    {
        camera->m_Radius += scrollY * -camera->m_ScrollSens;
        if (camera->m_Radius <= 0.01f)
        {
            camera->m_Radius = 0.01f;
        }
        camera->m_Position =
            camera->m_Target +
            glm::vec3(camera->m_Radius * cos(glm::radians(camera->m_RotationYaw)) *
                          cos(glm::radians(camera->m_RotationPitch)),
                      camera->m_Radius * sin(glm::radians(camera->m_RotationPitch)),
                      camera->m_Radius * sin(glm::radians(camera->m_RotationYaw)) *
                          cos(glm::radians(camera->m_RotationPitch)));
    }


    camera->m_ViewMatrix = glm::lookAt(camera->m_Position, camera->m_Target, m_Up);
}


}  // namespace Forge
