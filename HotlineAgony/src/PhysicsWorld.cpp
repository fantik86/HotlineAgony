#include "PhysicsWorld.h"

std::unique_ptr<b2World> PhysicsWorld::m_world = std::make_unique<b2World>(b2Vec2_zero);

b2World& PhysicsWorld::GetWorld() {
	return *(m_world.get());
}