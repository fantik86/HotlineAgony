#pragma once
#include <memory>
#include <box2d/box2d.h>

class PhysicsWorld {
public:
	static b2World& GetWorld();
private:
	static std::unique_ptr<b2World> m_world;
};

struct PhysicsData {
	const char* name;
	void* owner;
};