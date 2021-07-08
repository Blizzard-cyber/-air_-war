#include "raylib.h"
#include "hero.h"
#include "sky.h"


int main() {
    const int SCREEN_WIDTH 450;
    const int  SCREEN_HEIGHT 800;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "flighters");
    InitAudioDevice();

    Music music = LoadMusicStream("resources/sound/wind.mp3");
    PlayMusicStream(music);

    Texture2D texture3 = LoadTexture("resources/image/background.png");
    texture1 = LoadTexture("resources/image/shoot.png");
    texture_enemy1 = LoadTexture("resources/image/enemy1.png");
    texture_enemy2 = LoadTexture("resources/image/enemy2.png");

    Hero *hero = create_hero(SCREEN_WIDTH, SCREEN_HEIGHT, &texture);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        UpdateMusicStream(music);

        update_by_keypressed(screen_width, hero);

        draw_sky(&sky);
        draw_hero(hero);

        EndDrawing();
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

