#include "PhysicsWorld.h"
#include "FixtureDefBuilder.h"

std::unique_ptr<b2World> PhysicsWorld::m_world = std::make_unique<b2World>(b2Vec2_zero);

b2World& PhysicsWorld::GetWorld() {
	return *(m_world.get());
}

void PhysicsWorld::CreateBullet(Vector2 startPos, float direction, float speed) {
    b2BodyDef bodyPlayerDef;
    bodyPlayerDef.type = b2_dynamicBody;
    b2Body* bulletBody;
    bulletBody = PhysicsWorld::GetWorld().CreateBody(&bodyPlayerDef);

    b2PolygonShape bullet_shape;
    bullet_shape.SetAsBox(5, 1);

    game::physics::FixtureDefBuilder bullet_fixture;
    bullet_fixture.setShape(&bullet_shape);
    bullet_fixture.setDensity(1.f);
    bullet_fixture.setFriction(0.5f);
    bullet_fixture.setSensor(false);
    bullet_fixture.setCategoryBits(COLLISION_PLR_HITBOX);
    bullet_fixture.setMaskBits(COLLISION_WORLD);
    bullet_fixture.setPhysicsData(new PhysicsData(PhysicsBodyType::Bullet, bulletBody));

    bulletBody->CreateFixture(&bullet_fixture.getResult());

    bulletBody->SetTransform(b2Vec2(startPos.x, startPos.y), direction);
    bulletBody->ApplyLinearImpulseToCenter(b2Vec2(cos(direction) * speed, sin(direction) * speed), true);
}


b2Body* PhysicsWorld::CreateBody(b2BodyType type, b2Vec2 position, float rotation, float linearDamping, float angularDamping, b2FixtureDef fixture) {
    b2BodyDef bodyDef;
    bodyDef.type = type;
    bodyDef.position = position;
    bodyDef.angle = rotation;
    bodyDef.linearDamping = linearDamping;
    bodyDef.angularDamping = angularDamping;
    b2Body* body = GetWorld().CreateBody(&bodyDef);
    body->CreateFixture(&fixture);

    return body;
}