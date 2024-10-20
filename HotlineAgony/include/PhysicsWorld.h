#pragma once
#include <memory>
#include <string>
#include <raylib.h>
#include <box2d/box2d.h>

struct PhysicsData {
	const char* name;
	void* owner;
};

class PhysicsWorld {
public:
	static b2World& GetWorld();
private:
	static std::unique_ptr<b2World> m_world;
};

