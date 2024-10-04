#include "Animator.h"
#include "utils.h"

using game::drawing::Animator;

Animator::Animator() : m_current_animation(CharAnimationType::Unknown), m_previous_animation(CharAnimationType::Unknown) {}

void Animator::LoadAnimation(CharAnimationType animation_type, Animation animation) {
	m_AnimationBuffer[animation_type] = animation;
}

/**
@brief Gets Animation by ANIMATION_TYPE
@param animation_type Animation type.
@return Animation from m_AnimationBuffer, using animation_type as index.
*/
Animation& Animator::getAnimationByType(CharAnimationType animation_type) {
	return m_AnimationBuffer.at(animation_type);
}

Animation Animator::getCurrentAnimation() const {
	return m_AnimationBuffer.at(m_current_animation);
}

CharAnimationType Animator::getCurrentAnimationType() const {
	return m_current_animation;
}

/**
@brief Loads all player animations to the Animator::m_AnimationBuffer.
@param appearance Character appearance.
*/
void Animator::LoadAnimations(CharacterAppearance appearance) {
	std::string appearance_path = "sprites/character_appearance/";
	appearance_path.append(std::to_string(static_cast<int>(appearance))).append("/");
	Animation idle_anim(packAnimationSequence((appearance_path + "idle")), 0.1, true);
	LoadAnimation(CharAnimationType::Idle, idle_anim);
	Animation walking_anim(packAnimationSequence(appearance_path + "walking"), 0.1, true);
	LoadAnimation(CharAnimationType::Walking, walking_anim);
	Animation punching_anim(packAnimationSequence(appearance_path + "punching"), 0.1, false);
	LoadAnimation(CharAnimationType::Punching, punching_anim);
	Animation legs_anim(packAnimationSequence(appearance_path + "legs"), 0.05, true);
	LoadAnimation(CharAnimationType::Legs, legs_anim);
}

/**
@brief Assigns sprite with frame of given animation.
@param[in] animation Animation type
@param[out] sprite Sprite to assign with new value
@retval 0 On successful sprite assigning
@retval 1 If animation isn't playable but function tried to use
*/
int Animator::Update(CharAnimationType animation, Texture2D& sprite) {
	
	Animation& anim_ref = getAnimationByType(animation);
	const animation_sequence_t& sequence = anim_ref.getAnimationSequence();

	if (anim_ref.getAnimationState() != AnimationState::Playing && 
		anim_ref.getAnimationState() != AnimationState::Playable) {
		return 1;
	}

	m_current_animation = animation;
	if (m_previous_animation != m_current_animation) {
		m_previous_animation = m_current_animation;
	}


	if (anim_ref.frame_counter < sequence.size()) { // if frame counter can be incremented
		sprite = sequence.at(anim_ref.frame_counter);
	}
	else { // if frame counter reached last frame of animation
		anim_ref.frame_counter = 0;
		if (anim_ref.isAnimationLooped()) {
			sprite = sequence.at(0);
		}
		else {
			anim_ref.setAnimationState(AnimationState::Ended);
			return 0;
		}
	}
	if (GetTime() > anim_ref.next_frame_time) {
		if (anim_ref.next_frame_time == 0) {
			anim_ref.next_frame_time = GetTime() + anim_ref.getDelay();
			return 0;
		}
		anim_ref.next_frame_time = GetTime() + anim_ref.getDelay();
		anim_ref.frame_counter++;
	}
	return 0;
}

/*!
\brief Sets animator state to ANIM_STATE_STOPPED.
*/
int Animator::Stop(CharAnimationType animation) {
	getAnimationByType(animation).setAnimationState(AnimationState::Stopped);
	return 0;
}

int Animator::Resume(CharAnimationType animation) {
	getAnimationByType(animation).setAnimationState(AnimationState::Playing);
	return 0;
}