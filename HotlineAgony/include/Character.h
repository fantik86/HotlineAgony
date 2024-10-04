#pragma once
#include "raylib.h"
#include <vector>
#include "utils.h"
#include "Animator.h"
#include "box2d/box2d.h"

using game::drawing::Animator;

namespace game {
    namespace living {
        /**
        * @class Character
        * @brief Core class for any living entities.
        */
        class Character {
        public:
            Character();
            Character(float health);
            float getHealth() const;
            int getWalkspeed() const;
            void setWalkspeed(int value);
            void setHealth(float value);
            void incHealth(float value);
            void decHealth(float value);
            int kill();
            int revive();
            CharacterState getState() const;
            void setState(CharacterState new_state);
            Vector2 getWalkingDirection() const;
            const Tile& getStandingTile(); ///< Get the tile where character stading on
            Vector2 getStandingTilePos();
            Vector2 position = Vector2{ 0, 0 };
            Texture2D playerBodyTexture = { 0 };
            Texture2D playerLegsTexture = { 0 };
            b2Body* physics_body = { 0 };
        protected:
            void setWalkingDirection(Vector2 new_walking_direction);
            void setWalkingDirectionX(float new_X);
            void setWalkingDirectionY(float new_Y);
            float character_direction = 0; ///< Character direction **(in radians!)**
            Vector2 walking_direction = { 0, 0 };
            int walkspeed = 300; ///< Character speed.
            float m_health; ///< Character health. Game will define Character as dead, if health <= 0.
            const float m_max_health; ///< Max health of the character.
            //const int& low_health;        TODO: L2
            bool moving = false; ///< Is Character moving right now
            bool isInvincible = false; ///< Is Character can be killed
            CharacterState state = CharacterState::Idle; ///< Character state
            CharacterAppearance appearance = CharacterAppearance::russian_mafia; ///< Character \ref Animation set
            Animator animator; ///< \ref Animator object of the Character.
        };
    }
}