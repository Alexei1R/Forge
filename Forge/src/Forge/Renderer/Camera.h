//
// Created by toor on 2024-09-12.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "glm/fwd.hpp"

namespace Forge {

enum class CameraProjection
{
    PerspectiveCamera,
    OrthographicCamera
};


class Camera
{
public:
    virtual glm::mat4& GetViewProjectionMatrix() = 0;
    virtual glm::mat4& GetViewMatrix() = 0;
    virtual glm::mat4& GetProjectionMatrix() = 0;
    virtual glm::vec3 GetCameraPosition() = 0;
    virtual glm::vec3 GetCameraTarget() = 0;
    virtual void SetCameraProjection(CameraProjection proj) = 0;
    virtual void SetCameraFov(int degrees) = 0;

    virtual float GetNearClip() = 0;
    virtual float GetFarClip() = 0;


    virtual void Resize(int width, int height) = 0;
    virtual void AllowMovement(bool state) = 0;
    virtual void AllowRotation(bool state) = 0;
    virtual void AllowScroll(bool state) = 0;
};
}  // namespace Forge

#endif
