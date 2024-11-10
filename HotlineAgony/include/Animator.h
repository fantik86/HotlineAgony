#pragma once
#include "raylib.h"
#include "Animation.h"
#include <functional>


using game::drawing::Animation;

class Animator {
public:
	static void Add(int id, Animation& animation);
	static Animation& GetAnimationById(int id);
	static void Update();
	static void Play(int id);
	static void Stop(int id);
	static void Pause(int id);
private:
	static std::vector<std::pair<int, Animation>> TaskBuffer;
};