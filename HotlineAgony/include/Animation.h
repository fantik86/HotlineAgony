#pragma once
#include <vector>
#include "raylib.h"
#include "utils.h"

namespace game {
    namespace drawing {
        class Animation {
        public:
            Animation(animation_sequence_t frame_sequence, Texture2D& texture, bool isLooped, double frame_delay);
            void UpdateFrame();
            AnimationState getAnimationState() const;
            void setAnimationState(AnimationState new_state);
            bool operator==(const Animation& b) const {
                if (m_current_frame == b.m_current_frame && m_frame_sequence.size() == b.m_frame_sequence.size()
                    && m_current_texture.id == b.m_current_texture.id && m_next_frame_time == b.m_next_frame_time
                    && m_frame_delay == b.m_frame_delay && m_looped == b.m_looped) {
                    return true;
                }
                return false;
            }
            void operator=(const Animation& b) {
                m_current_frame = b.m_current_frame;
                m_current_texture = b.m_current_texture;
                m_frame_sequence = b.m_frame_sequence;
                m_next_frame_time = b.m_next_frame_time;
                m_frame_delay = b.m_frame_delay;
                m_looped = b.m_looped;
            }
        private:
            int m_current_frame;
            animation_sequence_t m_frame_sequence;
            AnimationState m_animation_state;
            Texture2D& m_current_texture;
            double m_next_frame_time;
            double m_frame_delay;
            bool m_looped = true;
        };
    }
}