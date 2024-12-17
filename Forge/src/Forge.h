#ifndef FORGE_H
#define FORGE_H

// Include this file to include all forge modules

// Core Includes
#include "Forge/Core/Handle.h"
#include "Forge/Core/Hash.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Core/Module/Module.h"
#include "Forge/Core/Module/ModuleStack.h"
#include "Forge/Core/Utils.h"

// Event System
#include "Forge/Events/Event.h"
#include "Forge/Events/ImplEvent.h"
#include "Forge/Events/KeyCodes.h"

// File Watch
/*#include "Forge/FileWatch/FileWatch.h"*/

// Renderer System
#include "Forge/Renderer/Buffer/Buffer.h"
#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "Forge/Renderer/Font.h"
#include "Forge/Renderer/Framebuffer.h"
#include "Forge/Renderer/RenderCommand.h"
#include "Forge/Renderer/Renderer.h"
#include "Forge/Renderer/Shader.h"
#include "Forge/Renderer/ShaderManager.h"
#include "Forge/Renderer/Window.h"
// Application
#include "Forge/ForgeApplication.h"

// GLM
#include <glm/gtc/type_ptr.hpp>

#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"

// Camera
#include "Forge/Renderer/Camera/Camera.h"
#include "Forge/Renderer/Camera/CameraController.h"

// UI
#include "Forge/BFUI/Button.h"
#include "Forge/BFUI/Column.h"
#include "Forge/BFUI/DrawList.h"
#include "Forge/BFUI/Row.h"
#include "Forge/BFUI/TextView.h"
#include "Forge/BFUI/Theme.h"
#include "Forge/BFUI/Window.h"

// Model
#include "Forge/Renderer/Material.h"
#include "Forge/Renderer/Model.h"

#endif // FORGE_H
