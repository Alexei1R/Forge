//
// Created by toor on 2024-08-23.
//

#ifndef FORGE_H
#define FORGE_H

// Include this file to include all forge modules

// Core Includes
#include "Forge/Core/Log/Log.h"
#include "Forge/Core/Utils.h"
#include "Forge/Core/Module/Module.h"
#include "Forge/Core/Module/ModuleStack.h"

// Event System
#include "Forge/Events/Event.h"
#include "Forge/Events/ImplEvent.h"
#include "Forge/Events/KeyCodes.h"

// File Watch
/*#include "Forge/FileWatch/FileWatch.h"*/

// Renderer System
#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "Forge/Renderer/Buffer/Buffer.h"
#include "Forge/Renderer/Shader.h"
#include "Forge/Renderer/Window.h"
#include "Forge/Renderer/Framebuffer.h"
#include "Forge/Renderer/Renderer3D.h"
#include "Forge/Renderer/Renderer2D.h"
#include "Forge/Renderer/RenderCommand.h"
#include "Forge/Renderer/Model/Mesh.h"

// Application
#include "Forge/ForgeApplication.h"


// GLM
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include <glm/gtc/type_ptr.hpp>

// Camera
#include "Forge/Renderer/Camera/Camera.h"
#include "Forge/Renderer/Camera/EditorCamera.h"


// Text
#include "Forge/Renderer/Text/Text.h"


#endif  // FORGE_H
