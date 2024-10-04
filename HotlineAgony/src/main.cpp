#include "raylib.h"
#include "Player.h"
#include "options.h"
#include "Environment.h"
#include "TexturePool.h"
#include "box2d/box2d.h"
#include "GUI.h"
#include "b2DrawRayLib.hpp"

void testfunc(int logLevel, const char* text, va_list args) {
    printf("GAMELOG: ");
    vprintf(text, args);
    printf("\n");
}

using game::drawing::Tilemap;
using game::global::Environment;
using game::global::TexturePool;
using game::living::Player;

Camera2D worldcam;

int main(int argc, char** argv)
{

    SetTraceLogLevel(8);
    SetTraceLogCallback(testfunc);
    
    InitWindow((int)DEFAULT_WINDOW_WIDTH, (int)DEFAULT_WINDOW_HEIGHT, WINDOW_NAME);
    
    SetWindowState(FLAG_VSYNC_HINT);

    SetTargetFPS(60);

    std::string game_path = GetApplicationDirectory();

    TexturePool::AddTexture(TexturePoolTextureType::Building, static_cast<int>(TileType::UNKNOWN), std::move(LoadTexture((game_path + "/sprites/buildings/unknown.png").c_str())));
    TexturePool::AddTexture(TexturePoolTextureType::Building, static_cast<int>(TileType::WALL), std::move(LoadTexture((game_path + "/sprites/buildings/wall.png").c_str())));

    Tilemap tilemap(Vector2{0, 0}, 1, 32, 32);

    Environment::SetTilemap(tilemap);
    Environment::FillTilemap(TileType::UNKNOWN, TILEMAP_LAYER_FLOOR);

    Environment::setTilemapTileTexture(1, 0, TILEMAP_LAYER_COLLISION, TileType::WALL, TileRotation::LEFT);

    Camera2D default_camera = { 0 };
    default_camera.target = Vector2{ 0, 0 };
    default_camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    default_camera.zoom = 1.35f;
    default_camera.rotation = 0;

    Environment::InitTilemapHitboxes();


    std::string font_path = game_path + "sprites/fonts/Courier Prime.ttf";
    Font textFont = LoadFontEx(font_path.c_str(), 64, 0, 255);
    b2BodyDef bodyPlayerDef;
    bodyPlayerDef.type = b2_dynamicBody;

    Player plr(default_camera, &bodyPlayerDef);

    if (FULLSCREEN_ON_START) {
        ToggleBorderlessWindowed();
        plr.player_camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    }


    b2DrawRayLib drawer;
    drawer.SetFlags(
        b2Draw::e_shapeBit |
        b2Draw::e_jointBit |
        b2Draw::e_aabbBit |
        b2Draw::e_pairBit |
        b2Draw::e_centerOfMassBit
    );
    
    Environment::GetPhysicsWorld().SetDebugDraw(&drawer);


    while (!WindowShouldClose())
    {
        float defaultCameraZoom = (float)GetScreenWidth() / (float)GetScreenHeight();

        Environment::GetPhysicsWorld().Step(GetFrameTime(), 6, 2);

        if (IsWindowState(FLAG_BORDERLESS_WINDOWED_MODE)) // Can ruin any attempts to change zoom in other place
            plr.player_camera.zoom = defaultCameraZoom;
        else
            plr.player_camera.zoom = defaultCameraZoom * 0.7f;

        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        worldcam.offset = plr.player_camera.offset;
        worldcam.target = plr.player_camera.target; // May be buggy
        worldcam.zoom = default_camera.zoom;

        

        BeginMode2D(worldcam);
        
        Environment::DrawTilemap();

        EndMode2D();
        
        plr.updatePlayer(); // Player is updating at the end of tick to draw him over tilemap
        game::drawing::GUI::drawGui();

        BeginMode2D(worldcam);
        
        if (Environment::debug_draw_edges) {
            Environment::GetPhysicsWorld().DebugDraw();
        }

        EndMode2D();

        EndDrawing();
    }

    CloseWindow();
}