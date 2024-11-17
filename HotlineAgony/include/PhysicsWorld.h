#pragma once
#include <memory>
#include <string>
#include <raylib.h>
#include <box2d/box2d.h>


class PhysicsWorld {
public:
	static b2World& GetWorld();
	static void CreateBullet(Vector2 startPos, float direction, float speed);
	static b2Body* CreateBody(b2BodyType type, b2Vec2 position, float rotation, float linearDamping, float angularDamping, b2FixtureDef fixture);
private:
	static std::unique_ptr<b2World> m_world;
};
