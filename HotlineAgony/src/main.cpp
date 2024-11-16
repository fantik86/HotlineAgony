#include "raylib.h"
#include "Player.h"
#include "options.h"
#include "Environment.h"
#include "PhysicsWorld.h"
#include "TexturePool.h"
#include "box2d/box2d.h"
#include "GUI.h"
#include "WeaponHandler.h"
#include "b2DrawRayLib.hpp"


void customlogger(int logLevel, const char* text, va_list args) {
        time_t timestamp = time(&timestamp);
        struct tm datetime = *localtime(&timestamp);
        switch (logLevel) {
        case LOG_INFO: printf("[INFO]: "); break;
        case LOG_ERROR: printf("[ERROR]: "); break;
        case LOG_WARNING: printf("[WARN]: "); break;
        case LOG_DEBUG: printf("[DEBUG]: "); break;
        case 8: printf("GAMELOG: "); break;
        case 9: printf("[%d:%d:%d]: ", datetime.tm_hour, datetime.tm_min, datetime.tm_sec); break;
        }
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
    SetExitKey(NULL);
    SetTargetFPS(60);
    SetTraceLogCallback(customlogger);

    std::string game_path = GetApplicationDirectory();

    TexturePool::AddTexture(TexturePoolTextureType::Building, static_cast<int>(TileType::UNKNOWN), LoadTexture((game_path + "/assets/buildings/unknown.png").c_str()));
    TexturePool::AddTexture(TexturePoolTextureType::Building, static_cast<int>(TileType::WALL), LoadTexture((game_path + "/assets/buildings/wall.png").c_str()));
    TexturePool::AddTexture(TexturePoolTextureType::Building, static_cast<int>(TileType::FLOOR), LoadTexture((game_path + "/assets/buildings/floor.png").c_str()));


    Tilemap tilemap(Vector2{0, 0}, 32, 32);

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
    default_camera.offset = Vector2{
        static_cast<float>(GetScreenWidth()) / 2.0f,
        static_cast<float>(GetScreenHeight()) / 2.0f };
    default_camera.zoom = 1.35f;
    default_camera.rotation = 0;
    
    Environment::InitTilemapHitboxes();


    std::string font_path = game_path + "assets/fonts/Courier Prime.ttf";

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
    
    float defaultCameraZoom = getWindowSizeRatio();

    if (IsWindowFullscreen()) // Can ruin any attempts to change zoom in other place
        plr.player_camera.zoom = defaultCameraZoom;
    else
        plr.player_camera.zoom = defaultCameraZoom;

    WeaponHandler::CreateWeapon<wp_Pistol>(Vector2{0, 50});

    //-------------------------------------------------//
    //                    MAIN LOOP                    //
    //-------------------------------------------------//
    
    while (!WindowShouldClose())
    {

        PhysicsWorld::GetWorld().Step(GetFrameTime() * TIME_SCALE, 6 * TIME_SCALE, 2 * TIME_SCALE);
        

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
        
        if (Environment::isHitboxesShowing()) {
            Environment::DrawHitboxes();
        }

        EndMode2D();

        EndDrawing();
    }

    CloseWindow();
}
