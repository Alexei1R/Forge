//
// Created by toor on 2024-10-21.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
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
};


}  // namespace Forge


#endif
