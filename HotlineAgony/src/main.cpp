#include "raylib.h"
#include "Player.h"
#include "options.h"
#include "Environment.h"
#include "PhysicsWorld.h"
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
using game::drawing::GUI;

Camera2D worldcam;

int main(int argc, char** argv)
{


    InitWindow((int)DEFAULT_WINDOW_WIDTH, (int)DEFAULT_WINDOW_HEIGHT, WINDOW_NAME);
    
    SetWindowState(FLAG_VSYNC_HINT);

    SetTargetFPS(60);

    std::string game_path = GetApplicationDirectory();

    TexturePool::AddTexture(TexturePoolTextureType::Building, static_cast<int>(TileType::UNKNOWN), std::move(LoadTexture((game_path + "/assets/buildings/unknown.png").c_str())));
    TexturePool::AddTexture(TexturePoolTextureType::Building, static_cast<int>(TileType::WALL), std::move(LoadTexture((game_path + "/assets/buildings/wall.png").c_str())));
    TexturePool::AddTexture(TexturePoolTextureType::Building, static_cast<int>(TileType::FLOOR), std::move(LoadTexture((game_path + "/assets/buildings/floor.png").c_str())));


    Tilemap tilemap(Vector2{0, 0}, 1, 32, 32);

    Environment::SetTilemap(tilemap);
    Environment::FillTilemap(TileType::UNKNOWN, TILEMAP_LAYER_FLOOR);

    
    Environment::FillTilemapSquare(4, 6, -4, -6, TILEMAP_LAYER_FLOOR, TileType::FLOOR, TileRotation::UP);

    Environment::FillTilemapLine(2, 2, 2, -3, TILEMAP_LAYER_WALL, TileType::WALL, TileRotation::LEFT);
    Environment::FillTilemapLine(-3, 2, -3, -3, TILEMAP_LAYER_WALL, TileType::WALL, TileRotation::RIGHT);
    Environment::FillTilemapLine(-2, 3, 1, 3, TILEMAP_LAYER_WALL, TileType::WALL, TileRotation::UP);
    Environment::FillTilemapLine(-2, -4, 1, -4, TILEMAP_LAYER_WALL, TileType::WALL, TileRotation::DOWN);
    Environment::setTilemapTileTexture(-3, 0, TILEMAP_LAYER_WALL, TileType::AIR, TileRotation::UP);
    
    
    Camera2D default_camera = { 0 };
    default_camera.target = Vector2{ 0, 0 };
    default_camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    default_camera.zoom = 1.35f;
    default_camera.rotation = 0;
    
    Environment::InitTilemapHitboxes();


    std::string font_path = game_path + "assets/fonts/Courier Prime.ttf";
    Font textFont = LoadFontEx(font_path.c_str(), 64, 0, 255);
    b2BodyDef bodyPlayerDef;
    bodyPlayerDef.type = b2_dynamicBody;

    Player plr(default_camera);


    if (FULLSCREEN_ON_START) {
        int currentMonitor = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(currentMonitor), GetMonitorHeight(currentMonitor));
        SetWindowState(FLAG_FULLSCREEN_MODE);
        
        plr.player_camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    }

    b2DrawRayLib drawer;
    drawer.SetFlags(
        b2Draw::e_shapeBit |
        b2Draw::e_jointBit |
        b2Draw::e_aabbBit |
        b2Draw::e_centerOfMassBit
    );
    
    PhysicsWorld::GetWorld().SetDebugDraw(&drawer);

    float defaultCameraZoom = (float)GetScreenWidth() / (float)GetScreenHeight() * 2.f;

    if (IsWindowFullscreen()) // Can ruin any attempts to change zoom in other place
        plr.player_camera.zoom = defaultCameraZoom;
    else
        plr.player_camera.zoom = defaultCameraZoom * 0.7f;
    wp_Knife* knifeTest = new wp_Knife(Vector2{0, -20});

    //-------------------------------------------------//
    //                    MAIN LOOP                    //
    //-------------------------------------------------//

    while (!WindowShouldClose())
    {
        PhysicsWorld::GetWorld().Step(GetFrameTime(), 6, 2);
        

        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        
        worldcam.offset = plr.player_camera.offset;
        worldcam.target = plr.player_camera.target; // May be buggy
        worldcam.zoom = plr.player_camera.zoom;
        

        BeginMode2D(worldcam);
        
        Environment::DrawTilemap(plr.player_camera, plr.position);

        WeaponHandler::DrawWeapons();
        EndMode2D();

        Animator::Update();
        plr.updatePlayer(); // Player is updating at the end of tick to draw him over tilemap
        GUI::drawGui();

        BeginMode2D(worldcam);
        
        if (Environment::debug_draw_edges) {
            Environment::DrawHitboxes();
        }

        EndMode2D();

        EndDrawing();
    }

    CloseWindow();
}