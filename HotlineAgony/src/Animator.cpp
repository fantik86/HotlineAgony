#include <Animator.h>
#include <raylib.h>
#include <algorithm>


std::vector<std::pair<int, game::drawing::Animation>> Animator::TaskBuffer = { };

///< Deleting animation from TaskBuffer
void Animator::Stop(int id) {
	for (auto& element : TaskBuffer) {
		if (element.first == id) {
			if (element.second.getAnimationState() != AnimationState::Ended) {
				element.second.setAnimationState(AnimationState::Ended);
				return;
			}
		}
	}
}

void Animator::Add(int id, game::drawing::Animation& animation) {
	auto it = std::find_if(TaskBuffer.begin(), TaskBuffer.end(), [&animation](const std::pair<int, game::drawing::Animation>& value) {
		return &animation == &(value.second);
		});

	if (it != TaskBuffer.end()) { // If animation is found
		throw std::runtime_error("Can't add existing animation to the Animator.");
	}

	TaskBuffer.push_back(std::make_pair(id, std::move(animation)));
}


void Animator::Update() {
	std::vector<std::reference_wrapper<std::pair<int, game::drawing::Animation>>> filteredAnims;

	std::copy_if(TaskBuffer.begin(), TaskBuffer.end(), std::back_inserter(filteredAnims), [](const std::reference_wrapper<std::pair<int, game::drawing::Animation>>& value) {
		return value.get().second.getAnimationState() == AnimationState::Playing;
		}); // copying only playing anims

	for (auto& anim : filteredAnims) {
		std::get<1>(anim.get()).UpdateFrame();
	}
}

void Animator::Play(int id) {

	Animation& anim = GetAnimationById(id);
	
	// This vector stores animations with the same texture ref as the anim variable
	std::vector<std::reference_wrapper<std::pair<int, game::drawing::Animation>>> playingAnims;

	std::copy_if(TaskBuffer.begin(), TaskBuffer.end(), std::back_inserter(playingAnims), [&anim](const std::pair<int, game::drawing::Animation>& value) {
		if (value.second != anim) {
			return &(value.second).getAnimatingTextureRef() == &anim.getAnimatingTextureRef();
		}
		});

	for (auto& animation : playingAnims) {
		std::get<1>(animation.get()).setAnimationState(AnimationState::Ended);
	}

	anim.setAnimationState(AnimationState::Playing);
}

void Animator::Pause(int id) {
	auto it = std::find_if(TaskBuffer.begin(), TaskBuffer.end(), [id](const std::pair<int, game::drawing::Animation>& value) {
		return value.first == id;
		});

	if (it != TaskBuffer.end())
		(*it).second.setAnimationState(AnimationState::Pause);
}

Animation& Animator::GetAnimationById(int id) {
	auto it = std::find_if(TaskBuffer.begin(), TaskBuffer.end(), [id](const std::pair<int, game::drawing::Animation>& value) {
		return value.first == id;
		});

	if (it != TaskBuffer.end())
		return (*it).second;
}