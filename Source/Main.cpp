#include "Game.h"

int main() {
    const RayEngine::GameSpec gameSpec{
        .WindowTitle = "Advanced Rendering Demo",
        .WindowSize = {1280, 720}
    };

    RayEngine::Game game;
    game.Init(gameSpec);
    game.Run();
    game.Shutdown();

    return 0;
}