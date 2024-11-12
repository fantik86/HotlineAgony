#pragma once
#include "raylib.h"
#include "utils.h"
#include "Weapon.h"
#include "WeaponHandler.h"



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
            Character(float health, MeleeWeapon* weapon);
            float getHealth() const;
            int getWalkspeed() const;
            void setWalkspeed(int value);
            void setHealth(float value);
            void incHealth(float value);
            void decHealth(float value);
            virtual void Draw();
            int kill();
            int revive();
            void setState(CharacterState new_state);
            CharacterState getState() const;
            Vector2 getWalkingDirection() const;
            const Tile& getStandingTile(); ///< Get the tile where character stading on
            Vector2 getStandingTilePos();
            Vector2 position = Vector2{ 0, 0 };
            Texture2D playerBodyTexture = { 0 };
            Texture2D playerLegsTexture = { 0 };
            b2Body* physics_body = { 0 };
            std::vector<MeleeWeapon*> m_collidingWeapons;
        protected:
            void setWalkingDirection(Vector2 new_walking_direction);
            void setWalkingDirectionX(float new_X);
            void setWalkingDirectionY(float new_Y);
            float degree_direction = 0;
            Vector2 walking_direction = { 0, 0 };
            float character_size = 0.95f;
            double attackCooldown = 0.0;
            MeleeWeapon* holdingWeapon = WeaponHandler::CreateWeapon<wp_Fists>(Vector2{ FLT_MAX, FLT_MAX }); ///< Fists as default player weapon.
            bool flipAttack = false;
            int walkspeed = 120;                ///< Character speed.
            float m_health;                     ///< Character health. Game will define Character as dead, if health <= 0.
            const float m_max_health;           ///< Max health of the character.
            //const int& low_health;            ///< Maybe TBA in future
            bool moving = false;                ///< Is Character moving right now
            bool isInvincible = false;          ///< Is Character cant be killed
            CharacterState state = CharacterState::Idle; ///< Current character state, animation, death, attack and other things are depends on state.
            CharacterAppearance appearance = CharacterAppearance::russian_mafia; ///< Character \ref Animation set
        };
    }
}