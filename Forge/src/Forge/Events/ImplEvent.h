
#ifndef IMPLEVENT_H
#define IMPLEVENT_H

#include "Event.h"
#include "Forge/Events/KeyCodes.h"
#include <sstream>
#include <string>
#include <vector>

namespace Forge {

class KeyEvent : public Event
{
public:
    KeyEvent(int key, Action action) : key_(key)
    {
        if (key == Key::LeftMouse || key == Key::MiddleMouse || key == Key::RightMouse)
        {
            if (action == Action::KeyPress)
            {
                action_ = Action::MousePress;
                Keyboard::SetKey(key, true);
            }
            if (action == Action::KeyRelease)
            {
                action_ = Action::MouseRelease;
                Keyboard::SetKey(key, false);
            }
        }
        else
        {
            action_ = action;
            if (action == Action::KeyPress)
            {
                Keyboard::SetKey(key, true);
            }
            if (action == Action::KeyRelease)
            {
                Keyboard::SetKey(key, false);
            }
        }
    };

    int GetKey() const
    {
        return key_;
    }
    EventType GetType() const override
    {
        return EventType::Key;
    }
    Action GetAction() const override
    {
        return action_;
    };
    virtual std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyEvent: " << key_ << "\n";
        return ss.str();
    };

private:
    int key_;
    Action action_;
};

class MouseEvent : public Event
{
public:
    MouseEvent(double x, double y, Action action) : x_(x), y_(y), action_(action)
    {
        if (action == Action::MouseMove)
        {
            Mouse::SetCursorPosition(x, y);
        }
        if (action == Action::MouseScroll)
        {
            Mouse::SetWheelScroll(x, y);
        }
    }
    double GetX() const
    {
        return x_;
    }
    double GetY() const
    {
        return y_;
    }

    EventType GetType() const override
    {
        return EventType::Mouse;
    }
    Action GetAction() const override
    {
        return action_;
    };

    virtual std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseEvent: x: " << x_ << " y: " << y_ << "\n";
        return ss.str();
    };

private:
    double x_, y_;
    Action action_;
};

class WindowEvent : public Event
{
public:
    WindowEvent(int x, int y, Action action) : x_(x), y_(y), action_(action)
    {
        switch (action)
        {
            case Action::Move: ApplicationStats::SetApplicationPosition(x, y); break;
            case Action::Resize: ApplicationStats::SetApplicationSize(x, y); break;
            case Action::Maximize: ApplicationStats::SetFullscreen(true); break;
            case Action::Restore: ApplicationStats::SetFullscreen(false); break;
            case Action::Iconify: ApplicationStats::SetFullscreen(false); break;
            case Action::Focus: ApplicationStats::SetFocused(true); break;
            case Action::LoseFocus: ApplicationStats::SetFocused(false); break;
            default: break;
        }
    }
    double GetX() const
    {
        return x_;
    }
    double GetY() const
    {
        return y_;
    }

    EventType GetType() const override
    {
        return EventType::Window;
    }
    Action GetAction() const override
    {
        return action_;
    };

    virtual std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowEvent: x: " << x_ << " y: " << y_ << "\n";
        return ss.str();
    };

private:
    int x_, y_;
    Action action_;
};


class DropEvent : public Event
{
public:
    DropEvent(const std::vector<std::string>& filePaths, Action action) :
        filePaths_(filePaths), action_(action)
    {
    }

    std::vector<std::string> GetFiles() const
    {
        return filePaths_;
    }
    EventType GetType() const override
    {
        return EventType::Drop;
    }

    Action GetAction() const override
    {
        return action_;
    }

    virtual std::string ToString() const override
    {
        std::stringstream ss;
        for (const auto& file : filePaths_)
        {
            ss << "File: " << file << "\n";
        }
        return ss.str();
    }

private:
    std::vector<std::string> filePaths_;
    Action action_;
};


}  // namespace Forge

#endif
