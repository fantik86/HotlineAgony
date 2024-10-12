#pragma once
#include "Character.h"
#include "raylib.h"
#include "Environment.h"
#include "Animation.h"
#include "Animator.h"

using game::living::Character;
extern b2World world;

class CameraInfo {
public:
    float camera_player_distance = 35.f;
    float speed = 0.3f;
    float fly_length = 0.55f;
};

namespace game {
    namespace living {
        class Player : public Character {
        public:
            using Character::Character;
            Player(Camera2D& camera) : player_camera(camera) {

                game::drawing::Animation anim_idle(packAnimationSequence(std::string(GetApplicationDirectory()) + "/assets/character_appearance/0/idle"), playerBodyTexture, 0.01);
                game::drawing::Animation anim_moving(packAnimationSequence(std::string(GetApplicationDirectory()) + "/assets/character_appearance/0/walking"), playerBodyTexture, 0.1);
                game::drawing::Animation anim_legs(packAnimationSequence(std::string(GetApplicationDirectory()) + "/assets/character_appearance/0/legs"), playerLegsTexture, 0.05);
                Animator::Add(anim_idle, [this]() -> bool {return getState() == CharacterState::Idle;});
                Animator::Add(anim_moving, [this]() -> bool {return getState() == CharacterState::Walking;});
                Animator::Add(anim_legs, [this]() -> bool {return getState() == CharacterState::Walking;});

                b2BodyDef bodyPlayerDef;
                bodyPlayerDef.type = b2_dynamicBody;
                physics_body = game::global::Environment::GetPhysicsWorld().CreateBody(&bodyPlayerDef);

                b2CircleShape player_shape;
                player_shape.m_radius = 8.f;
                
                b2FixtureDef fixture;
                fixture.shape = &player_shape;
                fixture.density = 1.f;
                fixture.friction = 0.5f;
                
                physics_body->CreateFixture(&fixture);
            }

            void Draw(); // TODO: move "Draw()" to Character class and make it virtual
            void updateKeyPress(); // TODO: move "Move()" to Character class and make it virtual
            void updateState();
            void updatePlayer();
            void setCamera(Camera2D& camera);
            playercontrols_t controls;
            Camera2D& player_camera;
            CameraInfo camera_info;
        private:
            bool canControl = true;
        };
    }
}

