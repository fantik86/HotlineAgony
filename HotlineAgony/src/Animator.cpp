#include <Animator.h>
#include <raylib.h>

std::vector<std::pair<game::drawing::Animation, std::function<bool(void)>>> Animator::TaskBuffer = { };

///< Deleting animation from TaskBuffer
void Animator::Stop(game::drawing::Animation animation) {
	for (auto& it = TaskBuffer.begin(); it != TaskBuffer.end(); it++) {
		if ((*it).first == animation) {
			TaskBuffer.erase(it);
		}
	}
}

void Animator::Add(game::drawing::Animation animation, std::function<bool(void)> condition) {
	TaskBuffer.push_back(std::make_pair(animation, condition));
}

void Animator::Update() {
	for (auto& element : TaskBuffer) {
		if (element.second()) {
			element.first.UpdateFrame();
		}
	}
}