#pragma once

#include "box2d/box2d.h"

enum class PhysicsBodyType {
	Wall, Weapon, Player, Character, Bullet, CollisionBox, Unknown
};

enum CollisionCategory : uint16 {
	COLLISION_WORLD = 0x0001,
	COLLISION_WEAPON = 0x0002,
	COLLISION_PLR_HITBOX = 0x0004,
	COLLISION_PLR_OUTER_HITBOX = 0x0008
};

struct PhysicsData {
	PhysicsData(PhysicsBodyType type, void* data_owner) : body_type(type), owner(data_owner) {}
	PhysicsBodyType body_type = PhysicsBodyType::Unknown;
	void* owner = nullptr;
	bool isFlaggedToDelete = false;
};

