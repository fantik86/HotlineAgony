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
                Animation anim_knife(packAnimationSequence(std::string(GetApplicationDirectory()) + "/assets/character_appearance/0/knife"), playerBodyTexture, false, 0.02);



                Animator::Add(0, anim_idle, [this]() -> bool {return getState() == CharacterState::Idle;});
                Animator::Add(1, anim_moving, [this]() -> bool {return getState() == CharacterState::Walking;});
                Animator::Add(2, anim_legs, [this]() -> bool {return moving;});
                Animator::Add(3, anim_punching, [this]() -> bool {
                    if ((*holdingWeapon).m_weapon_name == "wp_Fists") {
                        if (Animator::GetAnimationById(3).getAnimationState() == AnimationState::Ended) {
                            setState(CharacterState::Idle);
                            Animator::GetAnimationById(3).setAnimationState(AnimationState::Playing);
                        }
                        if (getState() == CharacterState::Attacking) {
                            return true;
                        }
                        else {
                            return false;
                        }
                    }
                    return false;
                });
                Animator::Add(4, anim_knife, [this]() -> bool {
                    if ((*holdingWeapon).m_weapon_name == "wp_Knife") {
                        if (Animator::GetAnimationById(4).getAnimationState() == AnimationState::Ended) {
                            setState(CharacterState::Idle);
                            Animator::GetAnimationById(4).setAnimationState(AnimationState::Playing);
                        }
                        if (getState() == CharacterState::Attacking) {
                            return true;
                        }
                        else {
                            return false;
                        }
                    }
                    return false;
                });


                b2BodyDef bodyPlayerDef;
                bodyPlayerDef.type = b2_dynamicBody;
                physics_body = PhysicsWorld::GetWorld().CreateBody(&bodyPlayerDef);

                b2CircleShape player_shape;
                player_shape.m_radius = 8.f;
                
                b2FixtureDef plrFixture;
                plrFixture.shape = &player_shape;
                plrFixture.density = 1.f;
                plrFixture.friction = 0.5f;
                plrFixture.filter.categoryBits = COLLISION_PLR_HITBOX;
                plrFixture.filter.maskBits = COLLISION_WORLD;

                b2FixtureUserData userData;
                PhysicsData* metadata = new PhysicsData();
                metadata->name = "Player";
                metadata->owner = this;
                userData.pointer = reinterpret_cast<uintptr_t>(metadata);

                plrFixture.userData = userData;

                physics_body->CreateFixture(&plrFixture);

                b2BodyDef bodyCollisionDef;
                bodyCollisionDef.type = b2_dynamicBody;

                b2CircleShape collision_shape;
                collision_shape.m_radius = 26.f;

                b2FixtureDef collisionFixture;
                collisionFixture.shape = &collision_shape;
                collisionFixture.density = 1.f;
                collisionFixture.friction = 0.f;
                collisionFixture.isSensor = true;
                collisionFixture.filter.categoryBits = COLLISION_PLR_OUTER_HITBOX;
                collisionFixture.filter.maskBits = COLLISION_WEAPON;

                b2FixtureUserData collisionUserData;
                PhysicsData* collisionMetadata = new PhysicsData();
                collisionMetadata->name = "playerCollision";
                collisionMetadata->owner = this;
                collisionUserData.pointer = reinterpret_cast<uintptr_t>(collisionMetadata);

                collisionFixture.userData = collisionUserData;

                collision_body = PhysicsWorld::GetWorld().CreateBody(&bodyCollisionDef);
                collision_body->CreateFixture(&collisionFixture);
                collision_body->SetSleepingAllowed(false);
            }

            void Draw() override;
            void updateKeyPress(); // TODO: move "Move()" to Character class and make it virtual
            void updateState();
            void updatePlayer();
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

