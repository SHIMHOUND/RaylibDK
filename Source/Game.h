#pragma once

#include <string>
#include "raylib.h"
#include "Visuals/AnimatedSprite.h"
#include "Input/Input.h"

namespace RayEngine {
    struct GameSpec {
        std::string WindowTitle;
        Vector2 WindowSize;
    };

    class Game {
    public:
        Game() = default;

        void Init(const GameSpec& gameSpec);
        void Run();
        void Shutdown();

    private:
        void Update(double deltaTime, const Input& input);
        void Render();
        void RenderUI(const Vector2 &screenSize) const;
        void SetTargetFPS(int fps);

        // Helper function for color alpha blending
        Color ColorAlpha(Color color, float alpha) const;

    private:
        bool m_bRunning = false;
        Camera2D m_RenderCamera{};
        Vector2 m_WindowSize{};
        Texture2D m_GemTexture{};
        Texture2D m_FoxTexture{};
        AnimatedSprite* m_FoxIdleAnimation = nullptr;
        AnimatedSprite* m_FoxWalkAnimation = nullptr;
        Shader m_GrayscaleShader{};
        float m_SpriteRotation = 0.0f;
        float m_FPS = 0.0f;

        // New game state variables for input handling
        Vector2 m_CharacterPosition{0.0f, 0.0f};
        bool m_bCharacterMoving = false;
        bool m_bCharacterFlipped = false;
        Vector2 m_CursorPosition{};
        Vector2 m_CursorWorldPosition{};
        float m_MouseClickTimer = 0.0f;

        // Input handler
        Input m_Input{};

        // Added missing target FPS variable
        int m_TargetFPS = 60;
    };
}
