#include "Game.h"
#include <raymath.h>
#include <cstdio>
#include <cmath>
#include "Visuals/AnimatedSprite.h"
#include "Input/Input.h"

void RayEngine::Game::Init(const GameSpec &gameSpec) {
    m_WindowSize = gameSpec.WindowSize;
    InitWindow(
        static_cast<int>(m_WindowSize.x),
        static_cast<int>(m_WindowSize.y),
        gameSpec.WindowTitle.c_str()
    );

    m_GemTexture = LoadTexture("../../Data/Sprites/gem.png");
    m_FoxTexture = LoadTexture("../../Data/Sprites/FoxAnimations.png");

    const Vector2 screenHalfSize = Vector2Scale(m_WindowSize, 0.5f);
    m_RenderCamera = {
        .offset = screenHalfSize,
        .target = {0.0f, 0.0f},
        .rotation = 0.0f,
        .zoom = 1.0f
    };

    // Initialize fox animations
    m_FoxIdleAnimation = new AnimatedSprite(
        m_FoxTexture,         // texture
        Vector2{0, 0},        // origin on texture
        Vector2{32, 32},      // frame size
        Vector2{0.5f, 0.5f},  // frame origin (center)
        5,                    // frame columns
        0.2f,                 // frame time
        5,                    // frame count
        0,                    // first frame
        true                  // repeat
    );

    m_FoxWalkAnimation = new AnimatedSprite(
        m_FoxTexture,         // texture
        Vector2{0, 32},       // origin on texture (second row)
        Vector2{32, 32},      // frame size
        Vector2{0.5f, 0.5f},  // frame origin (center)
        5,                    // frame columns
        0.2f,                 // frame time
        5,                    // frame count
        0,                    // first frame
        true                  // repeat
    );

    // Start animations
    m_FoxIdleAnimation->Play();
    m_FoxWalkAnimation->Play();

    // Load grayscale shader
    m_GrayscaleShader = LoadShader("../../Data/Shaders/Grayscale.vs", "../../Data/Shaders/Grayscale.fs");
}

void RayEngine::Game::Shutdown() {
    // Clean up resources
    UnloadTexture(m_GemTexture);
    UnloadTexture(m_FoxTexture);

    // Delete animations
    delete m_FoxIdleAnimation;
    delete m_FoxWalkAnimation;

    // Unload shader
    UnloadShader(m_GrayscaleShader);

    CloseWindow();
}

void RayEngine::Game::Run() {
    m_bRunning = true;
    double deltaTime = 1.0 / 30.0;

    SetTargetFPS(120);

    while (m_bRunning && !WindowShouldClose()) {
        const double frameStartTime = GetTime();

        // Handle input
        m_Input.Handle(m_WindowSize);

        // Check for Alt+F4 to close the application
        if (m_Input.GetKey(KeyCode::LeftAlt, InputState::Down) &&
            m_Input.GetKey(KeyCode::F4, InputState::Pressed)) {
            m_bRunning = false;
        }

        Update(deltaTime, m_Input);

        BeginDrawing();
        ClearBackground(GRAY);

        // Apply camera for game world rendering
        const Vector2 screenHalfSize = Vector2Scale(m_WindowSize, 0.5f);
        m_RenderCamera.offset = screenHalfSize;
        BeginMode2D(m_RenderCamera);
        Render();
        EndMode2D();

        // Render UI elements
        RenderUI(m_WindowSize);

        EndDrawing();

        const double frameEndTime = GetTime();
        deltaTime = frameEndTime - frameStartTime;
        deltaTime = std::min(deltaTime, 1.0 / 30.0);
    }
}

void RayEngine::Game::Update(double deltaTime, const Input& input) {
    m_SpriteRotation += 50.0 * deltaTime;
    m_FPS = static_cast<float>(1.0 / deltaTime);

    m_RenderCamera.target = {0.0f, (float) std::sin(GetTime()) * 50.0f};

    m_CursorPosition = input.GetCursorPosition();
    m_CursorWorldPosition = input.GetCursorWorldPosition(m_RenderCamera);

    if (input.GetMouseButton(MouseButton::Left, InputState::Down)) {
        m_MouseClickTimer = 1.0f;  // Set to max when clicking
    } else {
        m_MouseClickTimer = std::max(0.0f, m_MouseClickTimer - static_cast<float>(deltaTime) * 2.0f);  // Decrease over time
    }

    // Calculate movement direction from keyboard input
    Vector2 movementInput = {0.0f, 0.0f};

    if (input.GetKey(KeyCode::A, InputState::Down) || input.GetKey(KeyCode::Left, InputState::Down)) {
        movementInput.x -= 1.0f;
    }
    if (input.GetKey(KeyCode::D, InputState::Down) || input.GetKey(KeyCode::Right, InputState::Down)) {
        movementInput.x += 1.0f;
    }
    if (input.GetKey(KeyCode::W, InputState::Down) || input.GetKey(KeyCode::Up, InputState::Down)) {
        movementInput.y -= 1.0f;
    }
    if (input.GetKey(KeyCode::S, InputState::Down) || input.GetKey(KeyCode::Down, InputState::Down)) {
        movementInput.y += 1.0f;
    }

    // Normalize the movement vector if it's not zero
    float movementLength = Vector2Length(movementInput);
    if (movementLength > 0.0f) {
        // Normalize and apply movement
        movementInput = Vector2Scale(movementInput, 1.0f / movementLength);
        m_CharacterPosition = Vector2Add(m_CharacterPosition, Vector2Scale(movementInput, 150.0f * static_cast<float>(deltaTime)));

        // Set character state
        m_bCharacterMoving = true;
        if (movementInput.x != 0.0f) {
            m_bCharacterFlipped = movementInput.x < 0.0f;
        }
    } else {
        m_bCharacterMoving = false;
    }

    // Reset animations if R key is pressed
    if (input.GetKey(KeyCode::R, InputState::Pressed)) {
        m_FoxIdleAnimation->Reset();
        m_FoxWalkAnimation->Reset();
    }

    // Update animations
    m_FoxIdleAnimation->Update(deltaTime);
    m_FoxWalkAnimation->Update(deltaTime);
}

void RayEngine::Game::Render() {
    // Draw coordinate axes
    DrawLine(-1000.0f, 0.0f, 1000.0f, 0.0f, ColorAlpha(RED, 0.5f));
    DrawLine(0.0f, -1000.0f, 0.0f, 1000.0f, ColorAlpha(GREEN, 0.5f));

    // Draw cursor position in world space
    DrawCircle(
        static_cast<int>(m_CursorWorldPosition.x),
        static_cast<int>(m_CursorWorldPosition.y),
        10.0f,
        ColorAlpha(BLUE, 0.5f)
    );

    // Draw animated fox character
    const float characterScale = 3.0f;

    // Choose animation based on movement state
    if (m_bCharacterMoving) {
        m_FoxWalkAnimation->Render(m_CharacterPosition, 0.0f, characterScale, WHITE, m_bCharacterFlipped, false);
    } else {
        m_FoxIdleAnimation->Render(m_CharacterPosition, 0.0f, characterScale, WHITE, m_bCharacterFlipped, false);
    }
}

Color RayEngine::Game::ColorAlpha(Color color, float alpha) const {
    return {color.r, color.g, color.b, static_cast<unsigned char>(255.0f * alpha)};
}

void RayEngine::Game::RenderUI(const Vector2 &screenSize) const {
    // Draw shapes
    DrawRectangle(25, 25, 50, 50, RED);
    DrawCircle(
        static_cast<int>(screenSize.x * 0.5f),
        static_cast<int>(screenSize.y * 0.5f - 50.0f),
        70.0f,
        ColorAlpha(GREEN, 0.3f)
    );

    // Draw text
    const std::string text = "Hello, Text!";
    const int fontSize = 28;
    const Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, 2.0f);
    DrawText(
        text.c_str(),
        static_cast<int>(screenSize.x * 0.5f - textSize.x * 0.5f),
        10 + static_cast<int>(textSize.y * 0.5f),
        fontSize,
        BLACK
    );

    // Draw cursor position feedback in screen space
    float mouseCircleSize = 20.0f + 20.0f * m_MouseClickTimer;  // Size increases when clicked
    DrawCircle(
        static_cast<int>(m_CursorPosition.x * screenSize.x),
        static_cast<int>(m_CursorPosition.y * screenSize.y),
        mouseCircleSize,
        ColorAlpha(PURPLE, 0.3f + 0.3f * m_MouseClickTimer)  // Color intensity increases when clicked
    );

    // Draw texture with grayscale shader
    const float textureWidth = static_cast<float>(m_GemTexture.width);
    const float textureHeight = static_cast<float>(m_GemTexture.height);
    const Rectangle sourceRect = {0.0f, 0.0f, textureWidth, textureHeight};
    const Rectangle destRect = {
        screenSize.x - 100.0f,  // X position
        10.0f,                  // Y position
        100.0f,                 // Width
        100.0f                  // Height
    };
    const Vector2 origin = {50.0f, 50.0f}; // Rotation center

    // Apply grayscale shader
    BeginShaderMode(m_GrayscaleShader);
    DrawTexturePro(m_GemTexture, sourceRect, destRect, origin, 0.0f, WHITE);
    EndShaderMode();

    // Draw FPS counter
    char fpsBuffer[32];
    snprintf(fpsBuffer, sizeof(fpsBuffer), "FPS: %.2f", m_FPS);
    const int fpsFontSize = 20;
    const Vector2 fpsTextSize = MeasureTextEx(GetFontDefault(), fpsBuffer, fpsFontSize, 1.0f);
    const float padding = 10.0f;
    DrawText(
        fpsBuffer,
        static_cast<int>(screenSize.x - fpsTextSize.x - padding),
        static_cast<int>(padding),
        fpsFontSize,
        BLACK
    );
}

void RayEngine::Game::SetTargetFPS(int fps) {

    m_TargetFPS = fps;
}