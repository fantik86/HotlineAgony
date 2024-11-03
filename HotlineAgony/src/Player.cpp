#include "Player.h"


using game::living::Player;
using game::global::Environment;

extern Camera2D worldcam;

void Player::Draw() {
    

    float body_width = static_cast<float>(playerBodyTexture.width);
    float body_height = static_cast<float>(playerBodyTexture.height);
    float legs_width = static_cast<float>(playerLegsTexture.width);
    float legs_height = static_cast<float>(playerLegsTexture.height);

    if (body_width != body_height) {
        throw std::runtime_error("Player's body width and body height should be in a 1:1 ratio. Check player animation sprites.");
    }
    float legs_direction = atan2(-walking_direction.y, walking_direction.x) * RAD2DEG;

    BeginMode2D(player_camera);

    Environment::DrawTexture(playerLegsTexture, position, Vector2{ legs_width / 2.f, legs_height / 2.f }, legs_width, legs_height, character_size, legs_direction);
    Environment::DrawTexture(playerBodyTexture, position, Vector2{ body_width / 2.f, body_height / 2.f }, body_width, body_height, Normalize(body_width, 1 / body_width, body_width), degree_direction);

    EndMode2D();
}

static void switchFullsreen() {
    int currentMonitor = GetCurrentMonitor();
    if (IsWindowFullscreen()) {
        SetWindowSize(800, 600);
    }
    else {
        SetWindowSize(GetMonitorWidth(currentMonitor), GetMonitorHeight(currentMonitor));
    }
    ToggleFullscreen();

}

void Player::updateCamera() {
    Vector2 mouse2world = GetScreenToWorld2D(GetMousePosition(), player_camera);


    degree_direction = atan2f(mouse2world.y - position.y,
        mouse2world.x - position.x) * RAD2DEG; // Dont touch, i dont have idea how this hellish thing works!

    Vector2 newCameraPos = Vector2{
        GetScreenWidth() / 2.0f - camera_info.camera_player_distance * cos(degree_direction * DEG2RAD),
        GetScreenHeight() / 2.0f - camera_info.camera_player_distance * sin(degree_direction * DEG2RAD) };


    bool isCameraPressed = IsKeyDown(std::get<KeyboardKey>(controls.camera));

    Camera2D temp_camera = player_camera;
    temp_camera.zoom = camera_info.fly_length; ///< Zoom defines length of camera flight during hold LShift

    if (isCameraPressed) {
        temp_camera.zoom = 1.f / temp_camera.zoom;
        newCameraPos = Vector2Subtract(newCameraPos, Vector2Subtract(GetScreenToWorld2D(GetMousePosition(), temp_camera), position));
    }

    
    float mouseWheelMove = GetMouseWheelMove();
    float zoomStep = 1.f / 3.f;
    float minZoom = 1.f;
    float maxZoom = 2.f;
    float zoomChangeSpeed = 0.4f;

    if (mouseWheelMove > 0) {
        mouseWheelZoom += zoomStep;
    }
    else if (mouseWheelMove < 0) {
        mouseWheelZoom -= zoomStep;
    }

    mouseWheelZoom = Clamp(mouseWheelZoom, minZoom, maxZoom);

    player_camera.target = position;
    player_camera.offset = Vector2Lerp(player_camera.offset, newCameraPos, camera_info.speed);
    player_camera.zoom = Lerp(player_camera.zoom, mouseWheelZoom * getWindowSizeRatio() * CAMERA_ZOOM_MULTIPLIER, zoomChangeSpeed);
}

void Player::updateKeyPress() {

    if (IsKeyPressed(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER))) {
        switchFullsreen();

        /*
        * Line below is need to prevent camera easing to player
        * from down-right corner upon changing fullscreen
        */
        player_camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    }

    if (IsKeyPressed(KEY_F2)) {
        Environment::switchShowHitboxes();
    }


    
    // Player movement
    if (canControl) {
        bool isLeftPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_left));
        bool isRightPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_right));
        bool isUpPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_up));
        bool isDownPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_down));


        b2Vec2 velocity(0.f, 0.f);
        if (IsMouseButtonDown(std::get<MouseButton>(controls.item_throw))) {
            if (dropTime == -1.0) {
                dropTime = GetTime() + 1.0;
            }
            if (GetTime() > dropTime) {
                if (holdingWeapon->m_weapon_name != "wp_Fists") {
                    int random_rotate = std::rand() % 2;
                    float random_drop_direction = std::rand() % 360;
                    holdingWeapon->SetOnGround(true);
                    holdingWeapon->SetPhysicsBodyPosition(b2Vec2(position.x, position.y));
                    holdingWeapon->GetPhysicsBody()->SetAngularVelocity(random_rotate == 1 ? 2 : -2);

                    float dropLength = 40.f;

                    b2Vec2 dropVelocity(cos(random_drop_direction * DEG2RAD) * dropLength,
                        sin(random_drop_direction * DEG2RAD) * dropLength);

                    holdingWeapon->GetPhysicsBody()->SetLinearVelocity(dropVelocity);
                    holdingWeapon = new wp_Fists();
                }
            }
        }
        else {
            dropTime = -1.0;

        }
        
        if (IsMouseButtonReleased(std::get<MouseButton>(controls.item_throw))) {
            if (holdingWeapon->m_weapon_name != "wp_Fists") {
                if (!justPickedUpWeapon) {
                    justPickedUpWeapon = true;
                    return;
                }
                int random_rotate = std::rand() % 2;
                float random_drop_direction = std::rand() % 360;
                holdingWeapon->SetOnGround(true);
                holdingWeapon->SetPhysicsBodyPosition(b2Vec2(position.x, position.y));
                holdingWeapon->GetPhysicsBody()->SetAngularVelocity(random_rotate == 1 ? 2 : -2);

                float dropLength = 1600.f;

                Vector2 mouse2world = GetScreenToWorld2D(GetMousePosition(), player_camera);


                float character_direction = atan2f(mouse2world.y - position.y,
                    mouse2world.x - position.x); // Dont touch, i dont have idea how this hellish thing works!


                b2Vec2 dropVelocity(cos(character_direction) * dropLength,
                    sin(character_direction) * dropLength);

                holdingWeapon->GetPhysicsBody()->SetLinearVelocity(dropVelocity);
                holdingWeapon->GetPhysicsBody()->ApplyAngularImpulse(100000, true);
                holdingWeapon = new wp_Fists();
            }
        }

        if (IsMouseButtonPressed(std::get<MouseButton>(controls.item_throw))) {
            if (holdingWeapon->m_weapon_name == "wp_Fists") { ///< Weapon take
                if (m_collidingWeapons.size() > 0) {
                    holdingWeapon = m_collidingWeapons.at(0);
                    holdingWeapon->SetOnGround(false);
                    justPickedUpWeapon = false;
                }
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
        collision_body->SetTransform(physics_body->GetPosition(), 0.f);
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
        if (isLeftPressed || isRightPressed || 
            isUpPressed || isDownPressed) {
            if (isLeftPressed && !isRightPressed ||
                isRightPressed && !isLeftPressed)
                setState(CharacterState::Walking);
            
            if (isUpPressed && !isDownPressed ||
                isDownPressed && !isUpPressed)
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
    updateCamera();
    updateKeyPress();
    Draw();
}

void Player::setCamera(Camera2D& camera) {
    player_camera = camera;
}