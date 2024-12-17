#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>

#include "Forge/Config/Entry.h"
#include "Forge/Core/Log/Log.h"
#include "glm/fwd.hpp"

namespace Forge {
namespace Config {

using EntryPtr = std::shared_ptr<Entry>;

class Config {
public:
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = EntryPtr;
        using difference_type = std::ptrdiff_t;
        using pointer = EntryPtr*;
        using reference = EntryPtr&;

        Iterator(typename std::unordered_map<std::string, EntryPtr>::iterator it)
            : m_It(it) {}

        Iterator& operator++() {
            ++m_It;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        bool operator==(const Iterator& other) const {
            return m_It == other.m_It;
        }
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
        reference operator*() {
            return m_It->second;
        }

    private:
        typename std::unordered_map<std::string, EntryPtr>::iterator m_It;
    };

    Config() = default;
    ~Config() = default;

    template <typename T>
    void AddEntry(std::string_view name, const T& value) {
        auto entry = std::make_shared<TypedEntry<T>>(value);
        entry->SetEntryName(std::string(name));
        m_Entries[std::string(name)] = entry;
    }

    template <typename T>
    [[nodiscard]] TypedEntry<T>* GetEntry(std::string_view name) const {
        auto it = m_Entries.find(std::string(name));
        if (it != m_Entries.end()) {
            return dynamic_cast<TypedEntry<T>*>(it->second.get());
        }
        return nullptr;
    }

    [[nodiscard]] bool SaveToFile(const fs::path& filename) const {
        std::error_code ec;
        if (!fs::exists(filename.parent_path())) {
            fs::create_directories(filename.parent_path(), ec);
            if (ec)
                return false;
        }

        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) // Verificăm dacă fișierul s-a deschis cu succes
            return false;

        for (const auto& [name, entry] : m_Entries) {
            file << entry->Serialize() << '\n';
        }
        return true;
    }

    bool LoadFromFile(const fs::path& filename) {
        if (!fs::exists(filename))
            return false;

        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) // Verificăm dacă fișierul s-a deschis cu succes
            return false;

        m_Entries.clear();
        std::string line;
        while (std::getline(file, line)) {
            ParseEntry(line);
        }
        return true;
    }

    // Iterator support
    [[nodiscard]] Iterator begin() {
        return Iterator(m_Entries.begin());
    }
    [[nodiscard]] Iterator end() {
        return Iterator(m_Entries.end());
    }
    [[nodiscard]] std::size_t size() const noexcept {
        return m_Entries.size();
    }
    [[nodiscard]] bool empty() const noexcept {
        return m_Entries.empty();
    }

    // Range-based for loop support
    [[nodiscard]] auto entries() const {
        return std::ranges::subrange(m_Entries.begin(), m_Entries.end());
    }

private:
    void ParseEntry(std::string_view line) {
        auto typeStart = line.find('"') + 1;
        auto typeEnd = line.find('"', typeStart);
        auto nameStart = line.find('"', typeEnd + 2) + 1;
        auto nameEnd = line.find('"', nameStart);
        if (typeStart == std::string_view::npos || typeEnd == std::string_view::npos || nameStart == std::string_view::npos ||
            nameEnd == std::string_view::npos)
            return;

        auto type = line.substr(typeStart, typeEnd - typeStart);
        auto name = line.substr(nameStart, nameEnd - nameStart);

        // Create appropriate entry based on type
        EntryPtr entry;
        if (type == "Int")
            entry = std::make_shared<TypedEntry<int>>();
        else if (type == "Float")
            entry = std::make_shared<TypedEntry<float>>();
        else if (type == "Double")
            entry = std::make_shared<TypedEntry<double>>();
        else if (type == "Bool")
            entry = std::make_shared<TypedEntry<bool>>();
        else if (type == "String")
            entry = std::make_shared<TypedEntry<std::string>>();
        else if (type == "Vec2")
            entry = std::make_shared<TypedEntry<glm::vec2>>();
        else if (type == "Vec3")
            entry = std::make_shared<TypedEntry<glm::vec3>>();
        else if (type == "Vec4")
            entry = std::make_shared<TypedEntry<glm::vec4>>();
        else if (type == "Mat3")
            entry = std::make_shared<TypedEntry<glm::mat3>>(); // Fixed: was vec3
        else if (type == "Mat4")
            entry = std::make_shared<TypedEntry<glm::mat4>>(); // Fixed: was vec4
        else
            return;

        if (!entry->Deserialize(line))
            return;

        // Handle each type with appropriate dynamic_cast
        bool success = false;
        if (type == "Int")
            success = SetEntryName<int>(entry, name);
        else if (type == "Float")
            success = SetEntryName<float>(entry, name);
        else if (type == "Double")
            success = SetEntryName<double>(entry, name);
        else if (type == "Bool")
            success = SetEntryName<bool>(entry, name);
        else if (type == "String")
            success = SetEntryName<std::string>(entry, name);
        else if (type == "Vec2")
            success = SetEntryName<glm::vec2>(entry, name);
        else if (type == "Vec3")
            success = SetEntryName<glm::vec3>(entry, name);
        else if (type == "Vec4")
            success = SetEntryName<glm::vec4>(entry, name);
        else if (type == "Mat3")
            success = SetEntryName<glm::mat3>(entry, name);
        else if (type == "Mat4")
            success = SetEntryName<glm::mat4>(entry, name);

        if (success) {
            m_Entries[std::string(name)] = entry;
        }
    }

    // Helper template function to handle the dynamic_cast and name setting
    template <typename T>
    bool SetEntryName(const EntryPtr& entry, std::string_view name) {
        if (auto* typedEntry = dynamic_cast<TypedEntry<T>*>(entry.get())) {
            typedEntry->SetEntryName(std::string(name));
            return true;
        }
        return false;
    }

    std::unordered_map<std::string, EntryPtr> m_Entries;
};

} // namespace Config
} // namespace Forge
#endif
