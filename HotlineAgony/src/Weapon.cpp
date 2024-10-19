#include "Weapon.h"
#include "Environment.h"

std::vector<MeleeWeapon*> WeaponHandler::m_Weapons = { };

using game::global::Environment;

void initWeapon(MeleeWeapon* weapon) {
	b2BodyDef bodyDef;
	bodyDef.linearDamping = 5.f;
	bodyDef.angularDamping = 5.f;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(
		weapon->GetPosition().x,
		weapon->GetPosition().y);
	b2Body* body = Environment::GetPhysicsWorld().CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(weapon->m_physics_body_size.x, weapon->m_physics_body_size.y);


	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 50.f;
	fixture.friction = 1.0f;
	fixture.restitution = 0.1f; // Force of bouncing item off the other hitboxes
	fixture.isSensor = true;

	b2FixtureUserData userData;
	PhysicsData* data = new PhysicsData();
	data->name = "Weapon";
	data->owner = weapon;
	userData.pointer = reinterpret_cast<uintptr_t>(data);

	fixture.userData = userData;

	body->CreateFixture(&fixture);
	weapon->SetPhysicsBody(body);
}

void WeaponHandler::DrawWeapons() {
	for (auto& weapon : m_Weapons) {
		if (weapon->GetPhysicsBody() == nullptr) {
			initWeapon(weapon);
		}
		if (weapon->isOnGround()) {
			Vector2 align_center = weapon->m_physics_body_align_center;
			float angle = weapon->GetPhysicsBody()->GetAngle();
			float offsetX = align_center.x * cos(angle) - align_center.y * sin(angle);
			float offsetY = align_center.x * sin(angle) + align_center.y * cos(angle);
			int sprite_width = weapon->GetLyingSprite().width;
			int sprite_height = weapon->GetLyingSprite().height;
			b2Vec2 physics_body_pos = weapon->GetPhysicsBody()->GetPosition();
			Vector2 weapon_pos = weapon->GetPosition();
			
			weapon->SetPosition(Vector2{
				physics_body_pos.x - offsetX,
				physics_body_pos.y - offsetY
				});

			Environment::DrawTexture(weapon->GetLyingSprite(), Vector2{ weapon_pos.x, weapon_pos.y },
				Vector2{ static_cast<float>(sprite_width / 2),
				static_cast<float>(sprite_height / 2) },
				sprite_width, sprite_height,
				1.3f, angle * RAD2DEG);
		}
	}
}

void WeaponHandler::AddWeapon(MeleeWeapon* weapon) {
	m_Weapons.push_back(weapon);
}

void wp_Fists::Attack() {
	TraceLog(8, "Attack activated");
}

void wp_Fists::Drop() {
	TraceLog(8, "Drop activated");
}

void wp_Fists::Throw() {
	TraceLog(8, "Throw activated");
}


void wp_Knife::Attack() {
	TraceLog(8, "Attack activated");
}

void wp_Knife::Drop() {
	TraceLog(8, "Drop activated");
}

void wp_Knife::Throw() {
	TraceLog(8, "Throw activated");
}