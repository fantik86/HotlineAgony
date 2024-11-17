#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "PhysicsWorld.h"
#include "Environment.h"
#include "Animation.h"
#include "Animator.h"
#include "FixtureDefBuilder.h"

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
                
                game::physics::FixtureDefBuilder player_fixture;
                player_fixture.setShape(&player_shape);
                player_fixture.setDensity(1.f);
                player_fixture.setFriction(0.5f);
                player_fixture.setSensor(false);
                player_fixture.setCategoryBits(COLLISION_PLR_HITBOX);
                player_fixture.setMaskBits(COLLISION_WORLD);
                player_fixture.setPhysicsData(new PhysicsData(PhysicsBodyType::Player, this));

                physics_body = PhysicsWorld::CreateBody(b2_dynamicBody, b2Vec2_zero, 0, 0, 0, player_fixture.getResult());


                b2CircleShape collision_shape;
                collision_shape.m_radius = 26.f;


                game::physics::FixtureDefBuilder collision_fixture;
                collision_fixture.setShape(&collision_shape);
                collision_fixture.setDensity(1.f);
                collision_fixture.setSensor(true);
                collision_fixture.setCategoryBits(COLLISION_PLR_OUTER_HITBOX);
                collision_fixture.setMaskBits(COLLISION_WEAPON);
                collision_fixture.setPhysicsData(new PhysicsData(PhysicsBodyType::CollisionBox, this));

                collision_body = PhysicsWorld::CreateBody(b2_dynamicBody, b2Vec2_zero, 0, 0, 0, collision_fixture.getResult());
                collision_body->SetSleepingAllowed(false);

            }

            void Draw() override;
            void updateKeyPress();
            void updateState();
            void updatePlayer();
            void updateCamera();
            void updateWalkingDirection();
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

