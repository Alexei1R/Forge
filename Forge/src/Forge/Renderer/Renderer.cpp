//
// Created by toor on 2024-11-18.
//


#include "Renderer.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Core/Utils.h"

namespace Forge {

bool Renderer::m_IsInScope = false;
bool Renderer::m_IsInitialized = false;


bool Renderer::Initialize()
{
    if (!m_IsInitialized)
    {
        LOG_CRITICAL("The Renderer Context is already Initialized")
        return false;
    }

    // NOTE: Here Begin Initialization


    // NOTE: End
    m_IsInitialized = true;
    return true;
}
bool Renderer::Shutdown()
{
    if (!m_IsInitialized)
    {
        LOG_CRITICAL("The Renderer Context is not Initialized , Make shure to Initialize Before")
        return false;
    }
    // NOTE: Here Begin Shutdown


    // NOTE: End
    m_IsInitialized = false;
    return true;
}


void Renderer::CheckScope()
{
    if (!m_IsInScope)
    {
        F_ASSERT(false, "The Submit function is not in scope of Begin/End")
        return;
    }
}


void Renderer::Begin()
{
    if (!m_IsInitialized || m_IsInScope)
    {
        LOG_CRITICAL("Make shure to you call the Renderer::End() before you call Begin")
        return;
    }
    m_IsInScope = true;


    // NOTE: Here Begin Shutdown


    // NOTE: End
}


void Renderer::End()
{
    if (!m_IsInScope)
    {
        LOG_CRITICAL("Make shure to you call the Renderer::Begin() before you call End")
        return;
    }
    m_IsInScope = false;

    // NOTE: Here Begin Shutdown


    // NOTE: End
}
void Renderer::Submit()
{
    CheckScope();
}


}  // namespace Forge
