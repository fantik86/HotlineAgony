#include "Animation.h"
#include "Environment.h"
#include "Animator.h"

using game::drawing::Animation;

Animation::Animation(animation_sequence_t frame_sequence, Texture2D& texture, double frame_delay) :
	m_frame_sequence(frame_sequence), m_current_texture(texture), 
	m_frame_delay(frame_delay), m_next_frame_time(GetTime() + frame_delay), m_current_frame(0) {
	
}


/// Updates texture of the Animation.
AnimationState Animation::UpdateFrame() {
	if (GetTime() >= m_next_frame_time) {
		if (m_current_frame == m_frame_sequence.size()) { // if current frame is the last frame
			if (m_looped) {
				m_current_frame = 0;
			}
			else {
				m_current_texture = m_frame_sequence.at(m_current_frame);
				return AnimationState::Ended;	// Animation is ended
			}
		}
		m_current_texture = m_frame_sequence.at(m_current_frame);
		m_current_frame++;
		m_next_frame_time = GetTime() + m_frame_delay;
		return AnimationState::Playing;
	}
}
