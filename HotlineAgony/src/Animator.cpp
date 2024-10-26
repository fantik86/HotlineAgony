#include <Animator.h>
#include <raylib.h>
#include <algorithm>


std::vector<std::tuple<int, game::drawing::Animation, std::function<bool(void)>>> Animator::TaskBuffer = { };

///< Deleting animation from TaskBuffer
void Animator::Stop(game::drawing::Animation animation) {
    std::remove_if(TaskBuffer.begin(), TaskBuffer.end(),
                   [animation](const std::tuple<int, game::drawing::Animation, std::function<bool()>>& v){
        return std::get<1>(v) == animation;
    });
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