
#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <memory>

#include "Forge/Renderer/Camera/Camera.h"

namespace Forge {

enum class ControllerMode { FreeCamera = 0, OrbitalCamera };

class CameraController {
public:
    CameraController() = delete;
    CameraController(const CameraController&) = delete;
    CameraController(CameraController&&) = delete;
    CameraController& operator=(const CameraController&) = delete;
    CameraController& operator=(CameraController&&) = delete;
    ~CameraController() = delete;

    static void Update(std::shared_ptr<Camera>& camera);

private:
    static glm::vec3 m_Right;
    static glm::vec3 m_Front;
    static glm::vec3 m_Up;
    static glm::vec3 m_WorldUp;
    static bool m_FirstMouseTouch;
    static bool m_FirstMouseTouchMovement;
};

} // namespace Forge

#endif
