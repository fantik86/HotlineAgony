#include <Animator.h>
#include <raylib.h>

std::vector<std::tuple<int, game::drawing::Animation, std::function<bool(void)>>> Animator::TaskBuffer = { };

///< Deleting animation from TaskBuffer
void Animator::Stop(game::drawing::Animation animation) {
	for (auto& it = TaskBuffer.begin(); it != TaskBuffer.end(); it++) {
		if (std::get<1>((*it)) == animation) {
			TaskBuffer.erase(it);
		}
	}
}

void Animator::Add(int id, game::drawing::Animation animation, std::function<bool(void)> condition) {
	TaskBuffer.push_back(std::make_tuple(id, std::move(animation), condition));
}

void Animator::Update() {
	for (auto& element : TaskBuffer) {
		if (std::get<2>(element)()) {
			std::get<1>(element).UpdateFrame();
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