#pragma once
#include <vector>
#include "raylib.h"
#include "utils.h"

/**
* \file Animation.h
* @brief Animation class definitions.
* @details This file contains Animation class.
*/

namespace game {
    namespace drawing {
        class Animation {
        public:
            Animation();
            Animation(animation_sequence_t animationSequence, double animSpeed, bool isLooped);
            animation_sequence_t getAnimationSequence() const;
            void setDelay(float newSpeed);
            bool isAnimationLooped() const;
            double getDelay() const;
            int frame_counter = 0; ///< Counter that defines current animation frame.
            double next_frame_time = 0; ///< Point in time, when game reaching this point in time, \ref Animator::sequence_counter increments.
            void setAnimationState(AnimationState state);
            AnimationState getAnimationState() const;
        private:
            animation_sequence_t m_animationSequence;
            double m_frameDelay;
            bool m_looped = true;
            AnimationState m_animation_state = AnimationState::Playable; ///< Current \ref ANIMATION_STATE
        };
    }
}