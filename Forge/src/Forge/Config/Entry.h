#ifndef ENTRY_H
#define ENTRY_H

#include <algorithm>
#include <concepts>
#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <ranges>
#include <span>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Forge {
namespace Config {

namespace fs = std::filesystem;

class Entry {
public:
    Entry() = default;
    virtual ~Entry() = default;

    [[nodiscard]] virtual std::string Serialize() const = 0;
    virtual bool Deserialize(std::string_view data) = 0;
    [[nodiscard]] virtual std::string GetTypeInfo() const = 0;

    [[nodiscard]] const std::string& GetTypeName() const noexcept {
        return m_TypeName;
    }
    void SetTypeName(std::string_view name) {
        m_TypeName = name;
    }

protected:
    std::string m_TypeName;
};

////////////////////////////////////
// NOTE: TypedEntry ////////////////
////////////////////////////////////

template <typename DataType>
class TypedEntry : public Entry {
public:
    TypedEntry() {
        m_TypeName = GetTypeName();
    }

    explicit TypedEntry(const DataType& data)
        : m_Data(data) {
        m_TypeName = GetTypeName();
    }

    [[nodiscard]] const DataType& GetData() const noexcept {
        return m_Data;
    }
    void SetData(const DataType& data) noexcept {
        m_Data = data;
    }

    [[nodiscard]] std::string Serialize() const override {
        std::stringstream ss;
        ss << "[\"" << m_TypeName << "\":\"" << m_EntryName << "\"{" << SerializeValue() << "}]";
        return ss.str();
    }

    bool Deserialize(std::string_view data) override {
        // Basic format: ["TypeName":"EntryName"{value}]
        auto typeStart = data.find('"') + 1;
        auto typeEnd = data.find('"', typeStart);
        auto nameStart = data.find('"', typeEnd + 2) + 1;
        auto nameEnd = data.find('"', nameStart);
        auto valueStart = data.find('{') + 1;
        auto valueEnd = data.rfind('}');

        if (typeStart == std::string_view::npos || typeEnd == std::string_view::npos || nameStart == std::string_view::npos ||
            nameEnd == std::string_view::npos || valueStart == std::string_view::npos || valueEnd == std::string_view::npos)
            return false;

        auto valueStr = data.substr(valueStart, valueEnd - valueStart);
        return DeserializeValue(std::string(valueStr));
    }

    [[nodiscard]] std::string GetTypeInfo() const override {
        return std::string(GetTypeName());
    }

    void SetEntryName(std::string_view name) {
        m_EntryName = name;
    }
    [[nodiscard]] const std::string& GetEntryName() const noexcept {
        return m_EntryName;
    }

protected:
    DataType m_Data;
    std::string m_EntryName;

private:
    [[nodiscard]] static std::string GetTypeName() {
        if constexpr (std::is_same_v<DataType, int>)
            return "Int";
        if constexpr (std::is_same_v<DataType, float>)
            return "Float";
        if constexpr (std::is_same_v<DataType, double>)
            return "Double";
        if constexpr (std::is_same_v<DataType, bool>)
            return "Bool";
        if constexpr (std::is_same_v<DataType, std::string>)
            return "String";
        return "Unknown";
    }

    [[nodiscard]] std::string SerializeValue() const {
        std::stringstream ss;
        if constexpr (std::is_same_v<DataType, std::string>) {
            ss << "\"" << m_Data << "\"";
        } else if constexpr (std::is_same_v<DataType, bool>) {
            ss << (m_Data ? "true" : "false");
        } else {
            ss << m_Data;
        }
        return ss.str();
    }

    bool DeserializeValue(const std::string& value) {
        std::stringstream ss(value);
        if constexpr (std::is_same_v<DataType, std::string>) {
            if (value.length() < 2)
                return false;
            m_Data = value.substr(1, value.length() - 2); // Remove quotes
            return true;
        } else if constexpr (std::is_same_v<DataType, bool>) {
            std::string boolStr = value;
            std::transform(boolStr.begin(), boolStr.end(), boolStr.begin(), ::tolower);
            if (boolStr != "true" && boolStr != "false")
                return false;
            m_Data = (boolStr == "true");
            return true;
        } else if constexpr (std::is_same_v<DataType, int>) {
            try {
                m_Data = std::stoi(value);
                return true;
            } catch (...) {
                return false;
            }
        } else if constexpr (std::is_same_v<DataType, float>) {
            try {
                m_Data = std::stof(value);
                return true;
            } catch (...) {
                return false;
            }
        } else if constexpr (std::is_same_v<DataType, double>) {
            try {
                m_Data = std::stod(value);
                return true;
            } catch (...) {
                return false;
            }
        } else {
            ss >> m_Data;
            return !ss.fail();
        }
    }
};

// Specialization for glm::vec2
template <>
class TypedEntry<glm::vec2> : public Entry {
public:
    TypedEntry() {
        m_TypeName = GetTypeName();
    }
    explicit TypedEntry(const glm::vec2& data)
        : m_Data(data) {
        m_TypeName = GetTypeName();
    }

    [[nodiscard]] const glm::vec2& GetData() const noexcept {
        return m_Data;
    }
    void SetData(const glm::vec2& data) noexcept {
        m_Data = data;
    }

    [[nodiscard]] std::string Serialize() const override {
        std::stringstream ss;
        ss << "[\"" << m_TypeName << "\":\"" << m_EntryName << "\"{" << SerializeValue() << "}]";
        return ss.str();
    }

    bool Deserialize(std::string_view data) override {
        auto valueStart = data.find('{') + 1;
        auto valueEnd = data.rfind('}');
        if (valueStart == std::string_view::npos || valueEnd == std::string_view::npos)
            return false;
        auto valueStr = data.substr(valueStart, valueEnd - valueStart);
        return DeserializeValue(std::string(valueStr));
    }

    [[nodiscard]] std::string GetTypeInfo() const override {
        return "Vec2";
    }
    void SetEntryName(std::string_view name) {
        m_EntryName = name;
    }
    [[nodiscard]] const std::string& GetEntryName() const noexcept {
        return m_EntryName;
    }

protected:
    glm::vec2 m_Data;
    std::string m_EntryName;

private:
    [[nodiscard]] static std::string GetTypeName() {
        return "Vec2";
    }

    [[nodiscard]] std::string SerializeValue() const {
        std::stringstream ss;
        ss << m_Data.x << "," << m_Data.y;
        return ss.str();
    }

    bool DeserializeValue(const std::string& value) {
        std::stringstream ss(value);
        char comma;
        ss >> m_Data.x >> comma >> m_Data.y;
        return !ss.fail();
    }
};

// Specialization for glm::vec3
template <>
class TypedEntry<glm::vec3> : public Entry {
public:
    TypedEntry() {
        m_TypeName = GetTypeName();
    }
    explicit TypedEntry(const glm::vec3& data)
        : m_Data(data) {
        m_TypeName = GetTypeName();
    }

    [[nodiscard]] const glm::vec3& GetData() const noexcept {
        return m_Data;
    }
    void SetData(const glm::vec3& data) noexcept {
        m_Data = data;
    }

    [[nodiscard]] std::string Serialize() const override {
        std::stringstream ss;
        ss << "[\"" << m_TypeName << "\":\"" << m_EntryName << "\"{" << SerializeValue() << "}]";
        return ss.str();
    }

    bool Deserialize(std::string_view data) override {
        auto valueStart = data.find('{') + 1;
        auto valueEnd = data.rfind('}');
        if (valueStart == std::string_view::npos || valueEnd == std::string_view::npos)
            return false;
        auto valueStr = data.substr(valueStart, valueEnd - valueStart);
        return DeserializeValue(std::string(valueStr));
    }

    [[nodiscard]] std::string GetTypeInfo() const override {
        return "Vec3";
    }
    void SetEntryName(std::string_view name) {
        m_EntryName = name;
    }
    [[nodiscard]] const std::string& GetEntryName() const noexcept {
        return m_EntryName;
    }

protected:
    glm::vec3 m_Data;
    std::string m_EntryName;

private:
    [[nodiscard]] static std::string GetTypeName() {
        return "Vec3";
    }

    [[nodiscard]] std::string SerializeValue() const {
        std::stringstream ss;
        ss << m_Data.x << "," << m_Data.y << "," << m_Data.z;
        return ss.str();
    }

    bool DeserializeValue(const std::string& value) {
        std::stringstream ss(value);
        char comma;
        ss >> m_Data.x >> comma >> m_Data.y >> comma >> m_Data.z;
        return !ss.fail();
    }
};

// Specialization for glm::vec4
template <>
class TypedEntry<glm::vec4> : public Entry {
public:
    TypedEntry() {
        m_TypeName = GetTypeName();
    }
    explicit TypedEntry(const glm::vec4& data)
        : m_Data(data) {
        m_TypeName = GetTypeName();
    }

    [[nodiscard]] const glm::vec4& GetData() const noexcept {
        return m_Data;
    }
    void SetData(const glm::vec4& data) noexcept {
        m_Data = data;
    }

    [[nodiscard]] std::string Serialize() const override {
        std::stringstream ss;
        ss << "[\"" << m_TypeName << "\":\"" << m_EntryName << "\"{" << SerializeValue() << "}]";
        return ss.str();
    }

    bool Deserialize(std::string_view data) override {
        auto valueStart = data.find('{') + 1;
        auto valueEnd = data.rfind('}');
        if (valueStart == std::string_view::npos || valueEnd == std::string_view::npos)
            return false;
        auto valueStr = data.substr(valueStart, valueEnd - valueStart);
        return DeserializeValue(std::string(valueStr));
    }

    [[nodiscard]] std::string GetTypeInfo() const override {
        return "Vec4";
    }
    void SetEntryName(std::string_view name) {
        m_EntryName = name;
    }
    [[nodiscard]] const std::string& GetEntryName() const noexcept {
        return m_EntryName;
    }

protected:
    glm::vec4 m_Data;
    std::string m_EntryName;

private:
    [[nodiscard]] static std::string GetTypeName() {
        return "Vec4";
    }

    [[nodiscard]] std::string SerializeValue() const {
        std::stringstream ss;
        ss << m_Data.x << "," << m_Data.y << "," << m_Data.z << "," << m_Data.w;
        return ss.str();
    }

    bool DeserializeValue(const std::string& value) {
        std::stringstream ss(value);
        char comma;
        ss >> m_Data.x >> comma >> m_Data.y >> comma >> m_Data.z >> comma >> m_Data.w;
        return !ss.fail();
    }
};

// Specialization for glm::mat3
template <>
class TypedEntry<glm::mat3> : public Entry {
public:
    TypedEntry() {
        m_TypeName = GetTypeName();
    }
    explicit TypedEntry(const glm::mat3& data)
        : m_Data(data) {
        m_TypeName = GetTypeName();
    }

    [[nodiscard]] const glm::mat3& GetData() const noexcept {
        return m_Data;
    }
    void SetData(const glm::mat3& data) noexcept {
        m_Data = data;
    }

    [[nodiscard]] std::string Serialize() const override {
        std::stringstream ss;
        ss << "[\"" << m_TypeName << "\":\"" << m_EntryName << "\"{" << SerializeValue() << "}]";
        return ss.str();
    }

    bool Deserialize(std::string_view data) override {
        auto valueStart = data.find('{') + 1;
        auto valueEnd = data.rfind('}');
        if (valueStart == std::string_view::npos || valueEnd == std::string_view::npos)
            return false;
        auto valueStr = data.substr(valueStart, valueEnd - valueStart);
        return DeserializeValue(std::string(valueStr));
    }

    [[nodiscard]] std::string GetTypeInfo() const override {
        return "Mat3";
    }
    void SetEntryName(std::string_view name) {
        m_EntryName = name;
    }
    [[nodiscard]] const std::string& GetEntryName() const noexcept {
        return m_EntryName;
    }

protected:
    glm::mat3 m_Data;
    std::string m_EntryName;

private:
    [[nodiscard]] static std::string GetTypeName() {
        return "Mat3";
    }

    [[nodiscard]] std::string SerializeValue() const {
        std::stringstream ss;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                ss << m_Data[i][j];
                if (i != 2 || j != 2)
                    ss << ",";
            }
        }
        return ss.str();
    }

    bool DeserializeValue(const std::string& value) {
        std::stringstream ss(value);
        char comma;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                ss >> m_Data[i][j];
                if (i != 2 || j != 2)
                    ss >> comma;
            }
        }
        return !ss.fail();
    }
};

// Specialization for glm::mat4
template <>
class TypedEntry<glm::mat4> : public Entry {
public:
    TypedEntry() {
        m_TypeName = GetTypeName();
    }
    explicit TypedEntry(const glm::mat4& data)
        : m_Data(data) {
        m_TypeName = GetTypeName();
    }

    [[nodiscard]] const glm::mat4& GetData() const noexcept {
        return m_Data;
    }
    void SetData(const glm::mat4& data) noexcept {
        m_Data = data;
    }

    [[nodiscard]] std::string Serialize() const override {
        std::stringstream ss;
        ss << "[\"" << m_TypeName << "\":\"" << m_EntryName << "\"{" << SerializeValue() << "}]";
        return ss.str();
    }

    bool Deserialize(std::string_view data) override {
        auto valueStart = data.find('{') + 1;
        auto valueEnd = data.rfind('}');
        if (valueStart == std::string_view::npos || valueEnd == std::string_view::npos)
            return false;
        auto valueStr = data.substr(valueStart, valueEnd - valueStart);
        return DeserializeValue(std::string(valueStr));
    }

    [[nodiscard]] std::string GetTypeInfo() const override {
        return "Mat4";
    }
    void SetEntryName(std::string_view name) {
        m_EntryName = name;
    }
    [[nodiscard]] const std::string& GetEntryName() const noexcept {
        return m_EntryName;
    }

protected:
    glm::mat4 m_Data;
    std::string m_EntryName;

private:
    [[nodiscard]] static std::string GetTypeName() {
        return "Mat4";
    }

    [[nodiscard]] std::string SerializeValue() const {
        std::stringstream ss;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                ss << m_Data[i][j];
                if (i != 3 || j != 3)
                    ss << ",";
            }
        }
        return ss.str();
    }

    bool DeserializeValue(const std::string& value) {
        std::stringstream ss(value);
        char comma;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                ss >> m_Data[i][j];
                if (i != 3 || j != 3)
                    ss >> comma;
            }
        }
        return !ss.fail();
    }
};

} // namespace Config
} // namespace Forge

#endif // ENTRY_H
