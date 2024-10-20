//
// Created by toor on 2024-08-25.
//

#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>
namespace Forge {

enum class EventType
{
    None = 0,
    Window,
    Key,
    Mouse,
    Drop
};
enum class Action
{
    // KeyBoard And Mouse
    None = 0,
    KeyPress,
    KeyRelease,
    KeyRepeat,
    RegisterKeyChar,
    // Mouse
    MouseScroll,
    MouseMove,
    // Window
    Resize,
    Move,
    Close,
    Focus,
    LoseFocus,
    Iconify,
    Restore,
    Maximize,
    FramebufferResize,
    // Drop a file
    Drop
};

class Event
{
public:
    virtual ~Event() = default;
    virtual EventType GetType() const = 0;
    virtual Action GetAction() const = 0;
    virtual std::string ToString() const = 0;
};

//========================================================
//=== Keyboard Get Key ===================================
//========================================================

class Keyboard
{
public:
    static bool const IsKeyPressed(int key);
    friend class KeyEvent;

protected:
    static void SetKey(int key, bool IsKeyPressed);

private:
    static std::vector<bool> m_Keys;
};

//========================================================
//==== Mouse Get Values ==================================
//========================================================

class Mouse
{
public:
    friend class MouseEvent;

    static std::pair<double, double> const GetMousePosition();

    static std::pair<double, double> const GetMouseDeltaMovement();

    static std::pair<double, double> const GetMouseDeltaScroll();

protected:
    static void SetCursorPosition(double _x, double _y);
    static void SetWheelScroll(double dx, double dy);

private:
    static double lastx;
    static double lasty;
    static double dx;
    static double dy;

    static double x;
    static double y;

    static double scrollDx;
    static double scrollDy;
};

//========================================================
//====  Window Stats =====================================
//========================================================

class ApplicationStats
{
public:
    friend class WindowEvent;

    static std::pair<double, double> const GetApplicationPosition();
    static std::pair<double, double> const GetApplicationSize();

    static bool IsFullscreen();

    static bool IsFloating();

    static bool IsFocused();

protected:
    static void SetApplicationPosition(double x, double y);
    static void SetApplicationSize(double width, double height);
    static void SetFullscreen(bool fullscreen);
    static void SetFloating(bool floating);
    static void SetFocused(bool focused);

private:
    static double appPosX;
    static double appPosY;
    static double appWidth;
    static double appHeight;
    static bool fullscreen;
    static bool floating;
    static bool focused;
};

}  // namespace Forge

#endif
