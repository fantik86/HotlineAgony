#pragma once
#include <memory>
#include <string>
#include <raylib.h>
#include <box2d/box2d.h>

enum class PhysicsBodyType {
	Wall, Weapon, Player, Character, Bullet, CollisionBox, Unknown
};

struct PhysicsData {
	PhysicsData(PhysicsBodyType type, void* data_owner) : body_type(type), owner(data_owner) {}
	PhysicsBodyType body_type = PhysicsBodyType::Unknown;
	void* owner = nullptr;
	bool isFlaggedToDelete = false;
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
	static void CreateBullet(Vector2 startPos, float direction, float speed);
	static b2Body* CreateBody(b2BodyType type, b2Vec2 position, float rotation, float linearDamping, float angularDamping, b2FixtureDef fixture);
	static b2FixtureDef CreateFixture(b2Shape* shape, float density, float friction,
		float restitution, bool isSensor, uint16 categoryBits, 
		uint16 maskBits, PhysicsData* data);
private:
	static std::unique_ptr<b2World> m_world;
};

