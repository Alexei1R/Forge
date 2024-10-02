//
// Created by toor on 2024-08-23.
//

#ifndef FORGE_H
#define FORGE_H

// Include this file to include all forge modules

// Core Includes
#include "Forge/Core/Log/Log.h"
#include "Forge/Core/Utils.h"
#include "Forge/Core/Components/Component.h"

// Event System
#include "Forge/Events/Event.h"
#include "Forge/Events/ImplEvent.h"
#include "Forge/Events/KeyCodes.h"

// File Watch
#include "Forge/FileWatch/FileWatch.h"

// Renderer System
#include "Forge/Renderer/BufferImpl.h"
#include "Forge/Renderer/Buffer.h"
#include "Forge/Renderer/Shader.h"
#include "Forge/Renderer/Window.h"
#include "Forge/Renderer/Framebuffer.h"
#include "Forge/Renderer/OrbitalCamera.h"
#include "Forge/Renderer/Renderer.h"

// Application
#include "Forge/ForgeApplication.h"

// GUI
#include "imgui.h"
#include "ImGuizmo.h"

// GLM
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include <glm/gtc/type_ptr.hpp>


#endif  // FORGE_H
