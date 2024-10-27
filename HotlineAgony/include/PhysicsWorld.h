#pragma once
#include <memory>
#include <string>
#include <raylib.h>
#include <box2d/box2d.h>

struct PhysicsData {
	std::string name;
	void* owner;
};

enum CollisionCategory : uint16 {
    COLLISION_WORLD = 0x0001,
    COLLISION_WEAPON = 0x0002,
    COLLISION_PLR_HITBOX = 0x0004,
    COLLISION_PLR_OUTER_HITBOX = 0x0008
};

class PhysicsWorld {
public:
	static b2World& GetWorld();
private:
	static std::unique_ptr<b2World> m_world;
};

