#include "AnimatedSprite.h"
#include <algorithm>

RayEngine::AnimatedSprite::AnimatedSprite(
    const Texture2D &spriteSheetTexture,
    const Vector2 &origin,
    const Vector2 &frameSize,
    const Vector2 &frameOrigin,
    const int frameColumns,
    const double frameTime,
    const int frameCount,
    const int firstFrame,
    const bool bRepeat
): m_Texture(spriteSheetTexture),
   m_Origin(origin),
   m_FrameSize(frameSize),
   m_FrameOrigin(frameOrigin),
   m_FrameColumns(frameColumns),
   m_FrameTime(frameTime),
   m_FrameCount(frameCount),
   m_FirstFrame(firstFrame),
   m_bRepeat(bRepeat) {
    m_FrameOrigin.x = std::clamp(m_FrameOrigin.x, 0.0f, 1.0f);
    m_FrameOrigin.y = std::clamp(m_FrameOrigin.y, 0.0f, 1.0f);
}

void RayEngine::AnimatedSprite::Update(double deltaTime) {
    if (!m_bPlaying) {
        return;
    }
    m_ElapsedTime += deltaTime;
}

void RayEngine::AnimatedSprite::Render(const Vector2 &position, float rotation, float scale, Color tint, bool flipX, bool flipY) {
    Render(position, rotation, Vector2{scale, scale}, tint, flipX, flipY);
}

void RayEngine::AnimatedSprite::Render(const Vector2 &position, float rotation, const Vector2 &scale, Color tint, bool flipX, bool flipY) {
    int currentFrame = static_cast<int>(m_ElapsedTime / m_FrameTime);
    if (m_bRepeat) {
        currentFrame %= m_FrameCount;
    } else {
        currentFrame = std::min(currentFrame, m_FrameCount - 1);
    }

    currentFrame += m_FirstFrame;

    const int currentColumn = currentFrame % m_FrameColumns;
    const int currentRow = currentFrame / m_FrameColumns;

    Rectangle sourceRect = {
        m_Origin.x + currentColumn * m_FrameSize.x,
        m_Origin.y + currentRow * m_FrameSize.y,
        m_FrameSize.x,
        m_FrameSize.y
    };

    // Apply flipping by manipulating the source rectangle
    if (flipX) {
        sourceRect.width *= -1;  // Negative width flips horizontally
    }

    if (flipY) {
        sourceRect.height *= -1;  // Negative height flips vertically
    }

    const Vector2 spriteSize = {m_FrameSize.x * scale.x, m_FrameSize.y * scale.y};
    const Rectangle destRect = {
        position.x,
        position.y,
        spriteSize.x,
        spriteSize.y
    };

    const Vector2 origin = {spriteSize.x * m_FrameOrigin.x, spriteSize.y * m_FrameOrigin.y};

    DrawTexturePro(m_Texture, sourceRect, destRect, origin, rotation, tint);
}

void RayEngine::AnimatedSprite::Play() {
    m_bPlaying = true;
}

void RayEngine::AnimatedSprite::Stop() {
    m_bPlaying = false;
}

void RayEngine::AnimatedSprite::Reset() {
    m_ElapsedTime = 0.0;
}

void RayEngine::AnimatedSprite::SetFrame(int frame) {
    m_ElapsedTime = frame * m_FrameTime;
}