#pragma once
#include "Character.h"
#include "raylib.h"
#include "Environment.h"

using game::living::Character;
extern b2World world;

namespace game {
    namespace living {
        class Player : public Character {
        public:
            using Character::Character;
            Player(Camera2D& camera, b2BodyDef* player_body) : player_camera(camera) {
                animator.LoadAnimations(appearance);
                physics_body = game::global::Environment::GetPhysicsWorld().CreateBody(player_body);

                b2CircleShape player_shape;
                player_shape.m_radius = 12.f;

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
        private:
            bool canControl = true;
        };
    }
}
