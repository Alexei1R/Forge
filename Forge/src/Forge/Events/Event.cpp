
#include "Event.h"

namespace Forge {

std::vector<bool> Keyboard::m_Keys(512, false);

bool const Keyboard::IsKeyPressed(int key) {
    if (key >= 0 && key < m_Keys.size()) {
        return m_Keys[key];
    }
    return false;
}

void Keyboard::SetKey(int key, bool IsKeyPressed) {
    if (key >= 0 && key < m_Keys.size()) {
        m_Keys[key] = IsKeyPressed;
    }
}

// Define the static member variables
double Mouse::x = 1.0;
double Mouse::y = 1.0;

double Mouse::lastx = 1.0;
double Mouse::lasty = 1.0;

double Mouse::dx = 1.0;
double Mouse::dy = 1.0;

double Mouse::scrollDx = 1.0;
double Mouse::scrollDy = 1.0;

void Mouse::SetCursorPosition(double _x, double _y) {
    x = _x;
    y = _y;
}

void Mouse::SetWheelScroll(double dx, double dy) {
    scrollDx = dx;
    scrollDy = dy;
}

std::pair<double, double> const Mouse::GetMousePosition() {
    return {x, y};
}

std::pair<double, double> const Mouse::GetMouseDeltaMovement() {
    dx = x - lastx;
    dy = y - lasty;
    lastx = x;
    lasty = y;
    return {dx, dy};
}

std::pair<double, double> const Mouse::GetMouseDeltaScroll() {
    double tempScrollDx = scrollDx;
    double tempScrollDy = scrollDy;

    scrollDx = 0.0;
    scrollDy = 0.0;

    return {tempScrollDx, tempScrollDy};
}

// Initialize static member variables
double ApplicationStats::appPosX = 0.0;
double ApplicationStats::appPosY = 0.0;
double ApplicationStats::appWidth = 800.0;  // Default width
double ApplicationStats::appHeight = 600.0; // Default height
bool ApplicationStats::fullscreen = false;
bool ApplicationStats::floating = true;
bool ApplicationStats::focused = false;

std::pair<double, double> const ApplicationStats::GetApplicationPosition() {
    return {appPosX, appPosY};
}

std::pair<double, double> const ApplicationStats::GetApplicationSize() {
    return {appWidth, appHeight};
}

bool ApplicationStats::IsFullscreen() {
    return fullscreen;
}

bool ApplicationStats::IsFloating() {
    return floating;
}

bool ApplicationStats::IsFocused() {
    return focused;
}

void ApplicationStats::SetApplicationPosition(double x, double y) {
    appPosX = x;
    appPosY = y;
}

void ApplicationStats::SetApplicationSize(double width, double height) {
    appWidth = width;
    appHeight = height;
}

void ApplicationStats::SetFullscreen(bool isFullscreen) {
    fullscreen = isFullscreen;
}

void ApplicationStats::SetFloating(bool isFloating) {
    floating = isFloating;
}

void ApplicationStats::SetFocused(bool isFocused) {
    focused = isFocused;
}
} // namespace Forge
