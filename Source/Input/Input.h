#pragma once

#include "raylib.h"
#include "InputCodes.h"

namespace RayEngine {
    enum class InputState {
        Pressed,    // Key has been pressed in the current frame
        Down,       // Key is currently being held down
        Released,   // Key has been released in the current frame
        Up          // Key is currently not being pressed
    };
    
    class Input {
    public:
        Input() = default;
        ~Input() = default;

        void Handle(const Vector2& screenSize);
        
        bool GetKey(KeyCode key, InputState state) const;
        bool GetMouseButton(MouseButton button, InputState state) const;
        
        Vector2 GetCursorPosition() const;
        Vector2 GetCursorWorldPosition(const Camera2D& camera) const;
        
    private:
        Vector2 m_ScreenSize{};
    };
}