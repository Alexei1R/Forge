
#ifndef BUFFER_CPP
#define BUFFER_CPP

#include "Buffer.h"

namespace Forge {

unsigned int GetDataTypeSize(BufferDataType type)
{
    switch (type)
    {
        case BufferDataType::Float: return 4;
        case BufferDataType::Float2: return 4 * 2;
        case BufferDataType::Float3: return 4 * 3;
        case BufferDataType::Float4: return 4 * 4;
        case BufferDataType::Mat3: return 4 * 3 * 3;
        case BufferDataType::Mat4: return 4 * 4 * 4;
        case BufferDataType::Int: return 4;
        case BufferDataType::Int2: return 4 * 2;
        case BufferDataType::Int3: return 4 * 3;
        case BufferDataType::Int4: return 4 * 4;
        case BufferDataType::Bool: return 1;
        case BufferDataType::None: return 0;
    }
    LOG_TRACE("There is not a similar data type");
    return 0;
}


BufferLayout::BufferLayout() : m_Stride(0) {}

BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements) : m_Elements(elements)
{
    CalculateOffsetsAndStride();
}

uint32_t BufferLayout::GetStride() const
{
    return m_Stride;
}

const std::vector<BufferElement>& BufferLayout::GetElements() const
{
    return m_Elements;
}

std::vector<BufferElement>::iterator BufferLayout::begin()
{
    return m_Elements.begin();
}

std::vector<BufferElement>::iterator BufferLayout::end()
{
    return m_Elements.end();
}

std::vector<BufferElement>::const_iterator BufferLayout::begin() const
{
    return m_Elements.begin();
}

std::vector<BufferElement>::const_iterator BufferLayout::end() const
{
    return m_Elements.end();
}

void BufferLayout::CalculateOffsetsAndStride()
{
    size_t offset = 0;
    m_Stride = 0;
    for (auto& element : m_Elements)
    {
        element.offset = offset;
        offset += element.size;
        m_Stride += element.size;
    }
}

}  // namespace Forge

#endif
