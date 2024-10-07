#pragma once
#include "raylib.h"
#include "Animation.h"

class Animator {
public:
	static void Stop(game::drawing::Animation animation);
	static void Add(game::drawing::Animation animation);
	
	static void Update();
private:
	static std::vector<game::drawing::Animation> TaskBuffer;
};