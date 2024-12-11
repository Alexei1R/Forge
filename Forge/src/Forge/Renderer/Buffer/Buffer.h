
#ifndef BUFFER_H
#define BUFFER_H


#include <glad/glad.h>
#include <string>
#include <vector>
#include <memory>
#include "Forge/Core/Log/Log.h"


namespace Forge {


enum class BufferDataType
{

    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool

};


unsigned int GetDataTypeSize(BufferDataType type);


struct BufferElement
{
    std::string name;
    BufferDataType type;
    unsigned int size;
    unsigned int offset;

    BufferElement(BufferDataType type, std::string name) :
        name(name), type(type), size(GetDataTypeSize(type)), offset(0)
    {
    }
};


class BufferLayout
{
public:
    BufferLayout();
    BufferLayout(std::initializer_list<BufferElement> elements);

    uint32_t GetStride() const;
    const std::vector<BufferElement>& GetElements() const;

    std::vector<BufferElement>::iterator begin();
    std::vector<BufferElement>::iterator end();
    std::vector<BufferElement>::const_iterator begin() const;
    std::vector<BufferElement>::const_iterator end() const;

private:
    void CalculateOffsetsAndStride();

private:
    std::vector<BufferElement> m_Elements;
    uint32_t m_Stride = 0;
};


class Buffer
{
public:
    virtual ~Buffer() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

private:
};


}  // namespace Forge


#endif
