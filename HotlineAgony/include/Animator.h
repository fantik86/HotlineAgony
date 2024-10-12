#pragma once
#include "raylib.h"
#include "Animation.h"
#include <functional>

class Animator {
public:
	static void Stop(game::drawing::Animation animation);
	static void Add(game::drawing::Animation animation, std::function<bool(void)> condition);
	static void Update();
private:
	static std::vector<std::pair<game::drawing::Animation, std::function<bool(void)>>> TaskBuffer;
};