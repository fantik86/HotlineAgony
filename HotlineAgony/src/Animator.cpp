#include <Animator.h>
#include <raylib.h>
#include <algorithm>


std::vector<std::tuple<int, game::drawing::Animation, int>> Animator::TaskBuffer = { };

///< Deleting animation from TaskBuffer
void Animator::Stop(int id) {
	for (auto& element : TaskBuffer) {
		if (std::get<0>(element) == id) {
			if (std::get<1>(element).getAnimationState() != AnimationState::Ended) {
				std::get<1>(element).setAnimationState(AnimationState::Ended);
			}
		}
	}
}

void Animator::Add(int id, game::drawing::Animation& animation, int priority) {
	TaskBuffer.push_back(std::make_tuple(id, std::move(animation), priority));
	std::sort(TaskBuffer.begin(), TaskBuffer.end(),
		[](const std::tuple<int, game::drawing::Animation, int>& lhs, const std::tuple<int, game::drawing::Animation, int>& rhs) {
			return std::get<2>(lhs) < std::get<2>(rhs);
		});
}

void Animator::Update() {
	for (auto& element : TaskBuffer) {
		const Texture2D& current_texture = std::get<1>(element).getAnimatingTextureRef();

		std::vector<std::tuple<int, game::drawing::Animation, int>> tempBuf; // buffer for animations with same textures but different priorities

		std::copy_if(TaskBuffer.begin(), TaskBuffer.end(), std::back_inserter(tempBuf), [&current_texture](std::tuple<int, game::drawing::Animation, int> value) {
			return &std::get<1>(value).getAnimatingTextureRef() == &current_texture;
			});
		auto it = std::max_element(tempBuf.begin(), tempBuf.end(), [](const std::tuple<int, game::drawing::Animation, int>& a, const std::tuple<int, game::drawing::Animation, int>& b) {
			return std::get<2>(a) < std::get<2>(b);
			});
		
		std::get<1>(*it).UpdateFrame();
	}
	
}

void Animator::Play(int id) {
	for (auto& element : TaskBuffer) {
		if (std::get<0>(element) == id) {
			std::get<1>(element).setAnimationState(AnimationState::Playing);
		}
	}
}

Animation& Animator::GetAnimationById(int id) {
	for (auto& element : TaskBuffer) {
		if (std::get<0>(element) == id) {
			return std::get<1>(element);
		}
	}
}