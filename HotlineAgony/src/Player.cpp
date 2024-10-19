#include "Player.h"
#include "options.h"
#include "raylib.h"
#include "raymath.h"
#include "Environment.h"
#include <iostream>

using game::living::Player;
using game::global::Environment;

extern Camera2D worldcam;

void Player::Draw() {
	Vector2 mouse2world = GetScreenToWorld2D(GetMousePosition(), player_camera);
    
    
    float character_direction = atan2f(mouse2world.y - position.y,
        mouse2world.x - position.x); // Dont touch, i dont have idea how this hellish thing works!

    player_camera.target = position;
    
    float body_width = static_cast<float>(playerBodyTexture.width);
    float body_height = static_cast<float>(playerBodyTexture.height);
    float legs_width = static_cast<float>(playerLegsTexture.width);
    float legs_height = static_cast<float>(playerLegsTexture.height);
    

    float legs_direction = atan2(-walking_direction.y, walking_direction.x) * RAD2DEG;

    BeginMode2D(player_camera);

    Environment::DrawTexture(playerLegsTexture, position, Vector2{ 16, 16 }, legs_width, legs_height, character_size, legs_direction);
    Environment::DrawTexture(playerBodyTexture, position, Vector2{ 16, 16 }, body_width, body_height, character_size, character_direction * RAD2DEG);
    
    EndMode2D();
    bool isCameraPressed = IsKeyDown(std::get<KeyboardKey>(controls.camera));

    Vector2 newCameraPos = Vector2{
        GetScreenWidth() / 2.0f - camera_info.camera_player_distance * cos(character_direction),
        GetScreenHeight() / 2.0f - camera_info.camera_player_distance * sin(character_direction) };

    Camera2D temp_camera = player_camera;
    temp_camera.zoom = camera_info.fly_length; ///< Zoom defines length of camera flight during hold LShift

    if (isCameraPressed) {
        temp_camera.zoom = 1.f / temp_camera.zoom;
        newCameraPos = Vector2Subtract(newCameraPos, Vector2Subtract(GetScreenToWorld2D(GetMousePosition(), temp_camera), position));
    }
    player_camera.target = position;
    player_camera.offset = Vector2Lerp(player_camera.offset, newCameraPos, camera_info.speed);
}

void Player::updateKeyPress() {
    // Fullscreen switching
    if (IsKeyPressed(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER))) {
        int currentMonitor = GetCurrentMonitor();
        if (IsWindowFullscreen()) {
            SetWindowSize(800, 600);
        }
        else {
            SetWindowSize(GetMonitorWidth(currentMonitor), GetMonitorHeight(currentMonitor));
        }
        ToggleFullscreen();
        
        // This is need to prevent camera easing to player from down-right corner upon changing fullscreen
        player_camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    }
    if (IsKeyPressed(KEY_F2)) {
        Environment::debug_draw_edges = !Environment::debug_draw_edges;
    }
    

    float mouseWheelMove = GetMouseWheelMove();
    float zoomStep = 1.f / 3.f;
    if (mouseWheelMove > 0) {
        mouseWheelZoom += zoomStep;
    }
    else if (mouseWheelMove < 0) {
        mouseWheelZoom -= zoomStep;
    }

    float defaultCameraZoom = (float)GetScreenWidth() / (float)GetScreenHeight() * 2.f * 0.7f;

    mouseWheelZoom = Clamp(mouseWheelZoom, 1, 2);
    //player_camera.zoom = Clamp(mouseWheelZoom * defaultCameraZoom, defaultCameraZoom, 10);
    player_camera.zoom = Lerp(player_camera.zoom, mouseWheelZoom * defaultCameraZoom, 0.4);

    // Player movement
    if (canControl) {
        bool isLeftPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_left));
        bool isRightPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_right));
        bool isUpPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_up));
        bool isDownPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_down));

        float move_step = getWalkspeed() * GetFrameTime();

        b2Vec2 velocity(0.f, 0.f);

        if (IsMouseButtonPressed(std::get<MouseButton>(controls.item_throw))) {
            if (holdingWeapon->m_weapon_name == "wp_Fists") { ///< Weapon take
                // TODO: add something here
            }
            else { ///< Weapon throw/drop
                int random_rotate = std::rand() % 2;
                holdingWeapon->SetOnGround(true);
                holdingWeapon->SetPhysicsBodyPosition(b2Vec2(position.x, position.y));
                holdingWeapon->GetPhysicsBody()->SetTransform(holdingWeapon->GetPhysicsBody()->GetPosition(), holdingWeapon->GetPhysicsBody()->GetAngle());
                holdingWeapon->GetPhysicsBody()->SetAngularVelocity(random_rotate == 1 ? 2 : -2);
                
                holdingWeapon = new wp_Fists();

            }
        }

        if (isLeftPressed && !isRightPressed) {
            player_camera.offset.x = Lerp(player_camera.offset.x, player_camera.offset.x - 50, 0.05f);
            worldcam.rotation = Lerp(worldcam.rotation, 1, 0.05f);
            velocity.x -= walkspeed;
            setWalkingDirectionX(-1.0f);
            moving = true;
        }
        if (isRightPressed && !isLeftPressed) {
            player_camera.offset.x = Lerp(player_camera.offset.x, player_camera.offset.x + 50, 0.05f);
            worldcam.rotation = Lerp(worldcam.rotation, -1, 0.05f);
            velocity.x += walkspeed;
            setWalkingDirectionX(1.0f);
            moving = true;
        }
        if (isUpPressed && !isDownPressed) {
            player_camera.offset.y = Lerp(player_camera.offset.y, player_camera.offset.y + 50, 0.05f);
            velocity.y -= walkspeed;
            setWalkingDirectionY(1.0f);
            moving = true;
        }
        if (isDownPressed && !isUpPressed) {
            player_camera.offset.y = Lerp(player_camera.offset.y, player_camera.offset.y - 50, 0.05f);
            velocity.y += walkspeed;
            setWalkingDirectionY(-1.0f);
            moving = true;
        }

        if (!isLeftPressed && !isRightPressed && !isUpPressed && !isDownPressed) {
            worldcam.rotation = Lerp(worldcam.rotation, 0, 0.05f);
            moving = false;
        }

        if (!isUpPressed && !isDownPressed && (isRightPressed || isLeftPressed)) {
            setWalkingDirectionY(0.0f);
        }

        if (isUpPressed && isDownPressed) {
            setWalkingDirectionY(0.0f);
        }

        if (!isLeftPressed && !isRightPressed && (isUpPressed || isDownPressed)) {
            worldcam.rotation = Lerp(worldcam.rotation, 0, 0.05f);
            setWalkingDirectionX(0.0f);
        }

        if (isLeftPressed && isRightPressed) {
            setWalkingDirectionX(0.0f);
        }
        physics_body->SetLinearVelocity(velocity);
    }
}

void Player::updateState() {
    bool isLeftPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_left));
    bool isRightPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_right));
    bool isUpPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_up));
    bool isDownPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_down));
    bool isAttackPressed = IsMouseButtonDown(std::get<MouseButton>(controls.attack));


    if (isAttackPressed) {
        setState(CharacterState::Attacking);
    }




    if (state != CharacterState::Attacking) {
        if (isLeftPressed && isRightPressed) {
            if (isUpPressed || isDownPressed) {
                setState(CharacterState::Walking);
            }
            else {
                setState(CharacterState::Idle);
            }
        }
        else if (isUpPressed && isDownPressed) {
            if (isLeftPressed || isRightPressed) {
                setState(CharacterState::Walking);
            }
            else {
                setState(CharacterState::Idle);
            }
        }
        else if (isLeftPressed || isRightPressed || isUpPressed || isDownPressed) {
            setState(CharacterState::Walking);
        }
        else {
            setState(CharacterState::Idle);
        }
    }
}

void Player::updatePlayer() {
    position.x = physics_body->GetPosition().x;
    position.y = physics_body->GetPosition().y;
    updateState();
    updateKeyPress();
    Draw();
}

void Player::setCamera(Camera2D& camera) {
	player_camera = camera;
}