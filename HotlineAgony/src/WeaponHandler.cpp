#include "WeaponHandler.h"
#include "PhysicsWorld.h"


std::vector<MeleeWeapon*> WeaponHandler::m_Weapons = { };

void initWeapon(MeleeWeapon* weapon) {
	b2BodyDef bodyDef;
	bodyDef.linearDamping = 5.f;
	bodyDef.angularDamping = 5.f;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(
		weapon->GetPosition().x,
		weapon->GetPosition().y);
	b2Body* body = PhysicsWorld::GetWorld().CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(weapon->m_physics_body_size.x, weapon->m_physics_body_size.y);
	
	
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 50.f;
	fixture.friction = 1.f;
	fixture.restitution = 0.5f; // Force of bouncing item off the other hitboxes
    fixture.filter.categoryBits = COLLISION_WEAPON;
    fixture.filter.maskBits = COLLISION_WORLD | COLLISION_PLR_OUTER_HITBOX;


	b2FixtureUserData userData;
	PhysicsData* metadata = new PhysicsData();
	metadata->name = "Weapon";
	metadata->owner = weapon;
	userData.pointer = reinterpret_cast<uintptr_t>(metadata);

	fixture.userData = userData;

	body->CreateFixture(&fixture);

	weapon->SetPhysicsBody(body);
}

template <class WeaponType>
WeaponType* WeaponHandler::CreateWeapon(Vector2 position) {
	static_assert(std::is_base_of<MeleeWeapon, WeaponType>::value,
		"Weapon can't be created, if WeaponType contains class that isn't inherited from MeleeWeapon.");

	WeaponType* weapon = new WeaponType();
	weapon->SetPosition(position);
	initWeapon(weapon);
	AddWeapon(weapon);

	return weapon;

}

void WeaponHandler::DrawWeapons() {
	for (auto& weapon : m_Weapons) {
		if (weapon->isOnGround()) {
			Vector2 align_center = weapon->m_physics_body_align_center;
			float angle = weapon->GetPhysicsBody()->GetAngle();
			float offsetX = align_center.x * cosf(angle) - align_center.y * sinf(angle);
			float offsetY = align_center.x * sinf(angle) + align_center.y * cosf(angle);
			float sprite_width = static_cast<float>(weapon->GetLyingSprite().width);
			float sprite_height = static_cast<float>(weapon->GetLyingSprite().height);
			b2Vec2 physics_body_pos = weapon->GetPhysicsBody()->GetPosition();
			
			weapon->SetPosition(Vector2{
				physics_body_pos.x - offsetX,
				physics_body_pos.y - offsetY
				});

			DrawTexturePro(weapon->GetLyingSprite(), Rectangle{ 0, 0,
				sprite_width,
				sprite_height },
				Rectangle{ weapon->GetPosition().x + offsetX,
				weapon->GetPosition().y + offsetY,
				sprite_width,
				sprite_height }, Vector2{
					sprite_width / 2,
					sprite_height / 2 }, angle * RAD2DEG, WHITE);
		}
		else {
			weapon->SetPhysicsBodyPosition(b2Vec2{ FLT_MAX, FLT_MAX });
		}
	}
}



template wp_Knife* WeaponHandler::CreateWeapon<wp_Knife>(Vector2 position);
template wp_Fists* WeaponHandler::CreateWeapon<wp_Fists>(Vector2 position);

void WeaponHandler::AddWeapon(MeleeWeapon* weapon) {
	m_Weapons.push_back(weapon);
}