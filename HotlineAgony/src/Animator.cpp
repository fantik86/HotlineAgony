#include <Animator.h>
#include <raylib.h>

std::vector<game::drawing::Animation> Animator::TaskBuffer = { };

///< Deleting animation from TaskBuffer
void Animator::Stop(game::drawing::Animation animation) {
	for (auto& it = TaskBuffer.begin(); it != TaskBuffer.end(); it++) {
		if (*it == animation) {
			TaskBuffer.erase(it);
		}
	}
}

void Animator::Add(game::drawing::Animation animation) {
	TaskBuffer.push_back(animation);
}

void Animator::Update() {
	for (auto& element : TaskBuffer) {
		element.UpdateFrame();
	}
}