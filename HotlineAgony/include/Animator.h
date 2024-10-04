#pragma once
#include <unordered_map>
#include "raylib.h"
#include "Animation.h"
#include "utils.h"

/**
* @file Animator.h
* @brief \ref Animator class definitions.
* @details This file contains \ref Animator class.
*/

using game::drawing::Animation;

namespace game {
    namespace drawing {
        /**
        * @brief Animator is need to control the given Texture2D's texture and update it with given interval and \ref Animation.
        */
        class Animator {
        public:
            Animator();
            void LoadAnimation(CharAnimationType animation_type, Animation animation);
            void LoadAnimations(CharacterAppearance appearance);
            Animation& getAnimationByType(CharAnimationType animation_type);
            Animation getCurrentAnimation() const;
            CharAnimationType getCurrentAnimationType() const;
            int Update(CharAnimationType animation, Texture2D& sprite);
            int Stop(CharAnimationType animation);
            int Resume(CharAnimationType animation);
        private:
            std::unordered_map<CharAnimationType, Animation> m_AnimationBuffer; ///< The buffer that contains animations as <\ref ANIMATION_TYPE, \ref Animation>.
            CharAnimationType m_current_animation; ///< Current Animation that is playing.
            CharAnimationType m_previous_animation; ///< Previous Animation that was played before \ref Animator::m_current_animation.
        };
    }
}