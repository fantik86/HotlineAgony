#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "PhysicsWorld.h"
#include "Environment.h"
#include "Animation.h"
#include "Animator.h"

using game::living::Character;
using game::drawing::Animation;
extern b2World world;

struct CameraInfo {
    float camera_player_distance = 35.f;
    float speed = 0.3f;
    float fly_length = 0.55f;
};

namespace game {
    namespace living {
        class Player : public Character {
        public:
            Player(Camera2D& camera) : Character(100.f), player_camera(camera) {
                
                Animation anim_idle(packAnimationSequence(std::string(GetApplicationDirectory()) + "/assets/character_appearance/0/idle"), playerBodyTexture, true, 0.01);
                Animation anim_moving(packAnimationSequence(std::string(GetApplicationDirectory()) + "/assets/character_appearance/0/walking"), playerBodyTexture, true, 0.1);
                Animation anim_legs(packAnimationSequence(std::string(GetApplicationDirectory()) + "/assets/character_appearance/0/legs"), playerLegsTexture, true, 0.05);
                Animation anim_punching(packAnimationSequence(std::string(GetApplicationDirectory()) + "/assets/character_appearance/0/punching"), playerBodyTexture, false, 0.04);
                Animation anim_knife_attack(packAnimationSequence(std::string(GetApplicationDirectory()) + "/assets/character_appearance/0/knife_attack"), playerBodyTexture, false, 0.02);
                Animation anim_knife_idle(packAnimationSequence(std::string(GetApplicationDirectory()) + "/assets/character_appearance/0/knife_idle"), playerBodyTexture, true, 0.01);
                Animation anim_pistol_idle(packAnimationSequence(std::string(GetApplicationDirectory()) + "/assets/character_appearance/0/pistol_idle"), playerBodyTexture, true, 0.01);

                Animator::Add(0, anim_idle);
                Animator::Add(1, anim_moving);
                Animator::Add(2, anim_legs);
                Animator::Add(3, anim_punching);
                Animator::Add(4, anim_knife_attack);
                Animator::Add(5, anim_knife_idle);
                Animator::Add(6, anim_pistol_idle);
                Animator::Play(0);
                Animator::Play(2);

                b2CircleShape player_shape;
                player_shape.m_radius = 8.f;
                
                b2FixtureDef plrFixture = PhysicsWorld::CreateFixture(&player_shape, 1.f, 0.5f, 0, 
                    false, COLLISION_PLR_HITBOX, COLLISION_WORLD, 
                    new PhysicsData(PhysicsBodyType::Player, this));

                physics_body = PhysicsWorld::CreateBody(b2_dynamicBody, b2Vec2_zero, 0, 0, 0, plrFixture);


                b2CircleShape collision_shape;
                collision_shape.m_radius = 26.f;

                b2FixtureDef collisionFixture = PhysicsWorld::CreateFixture(&collision_shape, 1.f, 0.f, 0, 
                    true, COLLISION_PLR_OUTER_HITBOX, COLLISION_WEAPON,
                    new PhysicsData(PhysicsBodyType::CollisionBox, this));

                collision_body = PhysicsWorld::CreateBody(b2_dynamicBody, b2Vec2_zero, 0, 0, 0, collisionFixture);
                collision_body->SetSleepingAllowed(false);

            }

            void Draw() override;
            void updateKeyPress(); // TODO: move "Move()" to Character class and make it virtual
            void updateState();
            void updatePlayer();
            void updateCamera();
            void setCamera(Camera2D& camera);
            playercontrols_t controls;
            Camera2D& player_camera;
            CameraInfo camera_info;
            float mouseWheelZoom = 1.f;
        private:
            b2Body* collision_body = { 0 };
            bool canControl = true;
            bool justPickedUpWeapon = true;
            double dropTime = -1.0;
        };
    }
}

