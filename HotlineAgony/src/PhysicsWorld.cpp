#include "PhysicsWorld.h"

std::unique_ptr<b2World> PhysicsWorld::m_world = std::make_unique<b2World>(b2Vec2_zero);

b2World& PhysicsWorld::GetWorld() {
	return *(m_world.get());
}

void PhysicsWorld::CreateBullet(Vector2 startPos, float direction, float speed) {
    b2BodyDef bodyPlayerDef;
    bodyPlayerDef.type = b2_dynamicBody;
    b2Body* bulletBody;
    bulletBody = PhysicsWorld::GetWorld().CreateBody(&bodyPlayerDef);

    b2PolygonShape player_shape;
    player_shape.SetAsBox(5, 1);

    b2FixtureDef plrFixture;
    plrFixture.shape = &player_shape;
    plrFixture.density = 1.f;
    plrFixture.friction = 0.5f;
    plrFixture.filter.categoryBits = COLLISION_PLR_HITBOX;
    plrFixture.filter.maskBits = COLLISION_WORLD;

    b2FixtureUserData userData;
    PhysicsData* metadata = new PhysicsData(PhysicsBodyType::Bullet, bulletBody);
    userData.pointer = reinterpret_cast<uintptr_t>(metadata);

    plrFixture.userData = userData;

    bulletBody->CreateFixture(&plrFixture);

    bulletBody->SetTransform(bulletBody->GetPosition(), direction);
    bulletBody->ApplyLinearImpulseToCenter(b2Vec2(cos(direction) * speed, sin(direction) * speed), true);
}

b2FixtureDef PhysicsWorld::CreateFixture(b2Shape* shape, float density, float friction,
    float restitution, bool isSensor, uint16 categoryBits, 
    uint16 maskBits, PhysicsData* data) {
    b2FixtureDef fixture;
    fixture.shape = shape;
    fixture.density = density;
    fixture.friction = friction;
    fixture.restitution = restitution;
    fixture.isSensor = isSensor;
    fixture.filter.categoryBits = categoryBits;
    fixture.filter.maskBits = maskBits;

    b2FixtureUserData userData;
    userData.pointer = reinterpret_cast<uintptr_t>(data);
    fixture.userData = userData;

    return fixture;
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