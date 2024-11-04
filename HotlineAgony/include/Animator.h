#pragma once
#include "raylib.h"
#include "Animation.h"
#include <functional>


using game::drawing::Animation;

class Animator {
public:
	static void Stop(int id);
	static void Add(int id, Animation& animation, int priority);
	static Animation& GetAnimationById(int id);
	static void Update();
	static void Play(int id);
private:
	static std::vector<std::tuple<int, Animation, int>> TaskBuffer;
};