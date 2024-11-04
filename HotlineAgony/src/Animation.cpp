#include "Animation.h"

using game::drawing::Animation;

Animation::Animation(animation_sequence_t frame_sequence, Texture2D& texture, bool isLooped, double frame_delay) :
	m_frame_sequence(frame_sequence), 
	m_current_texture(texture), 
	m_frame_delay(frame_delay), 
	m_next_frame_time(GetTime() + frame_delay), 
	m_current_frame(0), 
	m_looped(isLooped), 
	m_animation_state(AnimationState::Ended) {}


/// Updates texture of the Animation.
void Animation::UpdateFrame() {
	
	if (m_animation_state == AnimationState::Playing) {
		if (GetTime() >= m_next_frame_time) {
			if (m_current_frame == m_frame_sequence.size()) { // if current frame is the last frame
				if (m_looped) {
					m_current_frame = 0;
				}
				else {
					m_current_frame = 0;
					m_current_texture = m_frame_sequence.at(m_frame_sequence.size() - 1);
					m_animation_state = AnimationState::Ended;
					return;	// Animation is ended
				}
			}
			m_current_texture = m_frame_sequence.at(m_current_frame);
			m_current_frame++;
			m_next_frame_time = GetTime() + m_frame_delay;
			m_animation_state = AnimationState::Playing;
			return;
		}
	}
}

AnimationState Animation::getAnimationState() const {
	return m_animation_state;
}

const Texture2D& Animation::getAnimatingTextureRef() const {
	return m_current_texture;
}

void Animation::setAnimationState(AnimationState new_state) {
	m_animation_state = new_state;
}