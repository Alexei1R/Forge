//
// Created by toor on 2024-07-30 .
//

#include "GraphicsBackend.h"

namespace Forge {

// Initialize the static members
BackendTarget GraphicsBackend::m_BackendTarget = BackendTarget::None;
std::mutex GraphicsBackend::m_Mutex;

}  // namespace Forge
