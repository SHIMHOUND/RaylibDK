#include "Input.h"

void RayEngine::Input::Handle(const Vector2& screenSize) {
    m_ScreenSize = screenSize;
}

bool RayEngine::Input::GetKey(KeyCode key, InputState state) const {
    const int keyValue = static_cast<int>(key);
    
    switch (state) {
        case InputState::Pressed:
            return IsKeyPressed(keyValue);
        case InputState::Down:
            return IsKeyDown(keyValue);
        case InputState::Released:
            return IsKeyReleased(keyValue);
        case InputState::Up:
            return IsKeyUp(keyValue);
        default:
            return false;
    }
}

bool RayEngine::Input::GetMouseButton(MouseButton button, InputState state) const {
    const int buttonValue = static_cast<int>(button);
    
    switch (state) {
        case InputState::Pressed:
            return IsMouseButtonPressed(buttonValue);
        case InputState::Down:
            return IsMouseButtonDown(buttonValue);
        case InputState::Released:
            return IsMouseButtonReleased(buttonValue);
        case InputState::Up:
            return IsMouseButtonUp(buttonValue);
        default:
            return false;
    }
}

Vector2 RayEngine::Input::GetCursorPosition() const {
    const Vector2 mousePos = GetMousePosition();
    return {
        mousePos.x / m_ScreenSize.x,
        mousePos.y / m_ScreenSize.y
    };
}

Vector2 RayEngine::Input::GetCursorWorldPosition(const Camera2D& camera) const {
    return GetScreenToWorld2D(GetMousePosition(), camera);
}