#include "Animation.h"

using game::drawing::Animation;

Animation::Animation() : m_animationSequence(std::vector<Texture2D>({ 0 })), m_frameDelay(1) {}
Animation::Animation(animation_sequence_t animationSequence, double animSpeed, bool isLooped) : m_animationSequence(animationSequence), m_frameDelay(animSpeed), m_looped(isLooped) {}

animation_sequence_t Animation::getAnimationSequence() const {
	return m_animationSequence;
}

void Animation::setDelay(float newSpeed) {
	m_frameDelay = newSpeed;
}

double Animation::getDelay() const {
	return m_frameDelay;
}

void Animation::setAnimationState(AnimationState state) {
	m_animation_state = state;
}

AnimationState Animation::getAnimationState() const {
	return m_animation_state;
}

bool Animation::isAnimationLooped() const {
	return m_looped;
}