#pragma once
#include "raylib.h"
#include "Animation.h"
#include <functional>

using game::drawing::Animation;

class Animator {
public:
	static void Stop(game::drawing::Animation animation);
	static void Add(int id, game::drawing::Animation animation, std::function<bool(void)> condition);
	static Animation& GetAnimationById(int id);
	static void Update();
private:
	static std::vector<std::tuple<int, game::drawing::Animation, std::function<bool(void)>>> TaskBuffer;
};