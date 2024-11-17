#include "Player.h"
#include "Chat.h"

using game::living::Player;
using game::global::Environment;
using game::drawing::Gui::Chat;

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

    Environment::DrawTexture(playerLegsTexture, position, Vector2{ legs_width / 2.f, legs_height / 2.f }, legs_width, legs_height, legs_direction, character_size);

    Environment::DrawTexture(playerBodyTexture, position, Vector2{ body_width / 2.f, body_height / 2.f }, body_width, body_height, degree_direction, Normalize(body_width, 1 / body_width, body_width));

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

    Chat::Update(*this);



    // Player movement
    if (canControl && !Chat::isEnabled()) {
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
                if (holdingWeapon->m_weapon_type != WeaponType::wp_Fists) { // Weapon drop
                    int random_rotate = std::rand() % 2;
                    float random_drop_direction = std::rand() % 360;
                    holdingWeapon->SetOnGround(true);
                    holdingWeapon->SetPhysicsBodyTransform(b2Vec2(position.x, position.y));
                    holdingWeapon->GetPhysicsBody()->SetAngularVelocity(random_rotate == 1 ? 2 : -2);

                    float dropLength = 40.f;

                    b2Vec2 dropVelocity(cos(random_drop_direction * DEG2RAD) * dropLength,
                        sin(random_drop_direction * DEG2RAD) * dropLength);

                    holdingWeapon->GetPhysicsBody()->SetLinearVelocity(dropVelocity);
                    holdingWeapon = new wp_Fists();
                    dropTime = -1.0;
                }
            }
        }
        if (IsMouseButtonReleased(std::get<MouseButton>(controls.item_throw))) {
            if (holdingWeapon->m_weapon_type != WeaponType::wp_Fists) {
                if (!justPickedUpWeapon) {
                    justPickedUpWeapon = true;
                    return;
                }
                int random_rotate = std::rand() % 2;
                float random_drop_direction = std::rand() % 360;
                holdingWeapon->SetOnGround(true);
                holdingWeapon->SetPhysicsBodyTransform(b2Vec2(position.x, position.y));
                holdingWeapon->GetPhysicsBody()->SetTransform(b2Vec2(position.x, position.y), degree_direction * DEG2RAD);
                holdingWeapon->GetPhysicsBody()->SetAngularVelocity(random_rotate == 1 ? 2 : -2);

                float dropLength = 1600.f * Clamp(abs(1 - (dropTime - GetTime())), 0.1f, 1.f);

                Vector2 mouse2world = GetScreenToWorld2D(GetMousePosition(), player_camera);


                float character_direction = atan2f(mouse2world.y - position.y,
                    mouse2world.x - position.x); // Dont touch, i dont have idea how this hellish thing works!


                b2Vec2 dropVelocity(cos(character_direction) * dropLength,
                    sin(character_direction) * dropLength);

                holdingWeapon->GetPhysicsBody()->SetLinearVelocity(dropVelocity);
                holdingWeapon = new wp_Fists();
            }
        }
        if (IsMouseButtonUp(std::get<MouseButton>(controls.item_throw))) {
            dropTime = -1.0;
        }

        if (IsMouseButtonPressed(std::get<MouseButton>(controls.item_throw))) {
            if (holdingWeapon->m_weapon_type == WeaponType::wp_Fists) { ///< Weapon take
                if (m_collidingWeapons.size() > 0) {
                    holdingWeapon = m_collidingWeapons.at(0);
                    holdingWeapon->SetOnGround(false);
                    justPickedUpWeapon = false;
                }
            }
        }

        if (IsMouseButtonPressed(std::get<MouseButton>(controls.attack))) {
            switch (holdingWeapon->m_weapon_type) {
            case WeaponType::wp_Fists:
                break;
            case WeaponType::wp_Knife:
                break;
            case WeaponType::wp_Pistol:
                PhysicsWorld::CreateBullet(Vector2Add(position, Vector2{cos(degree_direction * DEG2RAD) * 16, sin(degree_direction * DEG2RAD) * 16}), degree_direction * DEG2RAD, 30000);
                break;
            }
        }

        updateWalkingDirection();

        velocity.x = walking_direction.x * walkspeed;
        velocity.y = -walking_direction.y * walkspeed;
        
        worldcam.rotation = Lerp(worldcam.rotation, -walking_direction.x, 0.05f);

        physics_body->SetLinearVelocity(velocity);
        collision_body->SetTransform(b2Vec2(position.x, position.y), 0.f);
    }
}

void Player::updateWalkingDirection() {
    bool isLeftPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_left));
    bool isRightPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_right));
    bool isUpPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_up));
    bool isDownPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_down));

    if (isUpPressed) {
        walking_direction.y = 1;
    }
    if (isDownPressed) {
        walking_direction.y = -1;
    }
    if (isRightPressed) {
        walking_direction.x = 1;
    }
    if (isLeftPressed) {
        walking_direction.x = -1;
    }

    if (isRightPressed && isLeftPressed)
        walking_direction.x = 0;
    if (isUpPressed && isDownPressed)
        walking_direction.y = 0;

    if (!isRightPressed && !isLeftPressed)
        walking_direction.x = 0;
    if (!isUpPressed && !isDownPressed)
        walking_direction.y = 0;
}

void Player::updateState() {
    bool isLeftPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_left));
    bool isRightPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_right));
    bool isUpPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_up));
    bool isDownPressed = IsKeyDown(std::get<KeyboardKey>(controls.move_down));
    bool isAttackPressed = IsMouseButtonDown(std::get<MouseButton>(controls.attack));

    int anim_idle_id = 0;
    int anim_walk_id = 1;
    int anim_legs_id = 2;
    int anim_attack_id = 3;

    switch (holdingWeapon->m_weapon_type) {
    case WeaponType::wp_Fists:
        anim_attack_id = 3;
        break;
    case WeaponType::wp_Knife:
        anim_idle_id = 5;
        anim_attack_id = 4;
        break;
    case WeaponType::wp_Pistol:
        anim_idle_id = 6;
        anim_walk_id = 6;
        anim_attack_id = 6;
        break;
    }

    if (isAttackPressed) {
        setState(CharacterState::Attacking);
        Animator::Play(anim_attack_id);
        Animator::Stop(anim_idle_id);
        Animator::Stop(1);
    }

    if (Animator::GetAnimationById(3).getAnimationState() == AnimationState::Ended) {
        setState(CharacterState::Idle);
    }

    if (state != CharacterState::Attacking) {
        if (isLeftPressed || isRightPressed || 
            isUpPressed || isDownPressed) {
            if (isLeftPressed && !isRightPressed ||
                isRightPressed && !isLeftPressed) {
                if (isMoving()) {
                    setState(CharacterState::Walking);
                    Animator::Play(anim_walk_id);
                    Animator::Play(anim_legs_id);
                    return;
                }
            }
            
            if (isUpPressed && !isDownPressed ||
                isDownPressed && !isUpPressed) {
                if (isMoving()) {
                    setState(CharacterState::Walking);
                    Animator::Play(anim_walk_id);
                    Animator::Play(anim_legs_id);
                    return;
                }
            }

            if (isLeftPressed && isRightPressed ||
                isUpPressed && isDownPressed) {
                setState(CharacterState::Idle);
                Animator::Stop(anim_legs_id);
                Animator::Play(anim_idle_id);
                return;
            }
        }
        else {
            setState(CharacterState::Idle);
            Animator::Stop(anim_legs_id);
            Animator::Play(anim_idle_id);
            return;
        }
    }
}

void Player::updatePlayer() {
    position = Vector2{ physics_body->GetPosition().x, physics_body->GetPosition().y };
    updateState();
    updateCamera();
    updateKeyPress();
    Draw();
}

void Player::setCamera(Camera2D& camera) {
    player_camera = camera;
}