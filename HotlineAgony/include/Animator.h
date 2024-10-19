#pragma once
#include "raylib.h"
#include "Animation.h"
#include <functional>

using game::drawing::Animation;

class Animator {
public:
	static void Stop(Animation animation);
	static void Add(int id, Animation animation, std::function<bool(void)> condition);
	static Animation& GetAnimationById(int id);
	static void Update();
private:
	static std::vector<std::tuple<int, Animation, std::function<bool(void)>>> TaskBuffer;
};