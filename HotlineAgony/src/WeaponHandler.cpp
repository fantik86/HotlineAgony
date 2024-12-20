#include "WeaponHandler.h"
#include "PhysicsWorld.h"
#include "FixtureDefBuilder.h"

std::vector<MeleeWeapon*> WeaponHandler::m_Weapons = { };
std::vector<RangeWeapon*> WeaponHandler::m_RangeWeapons = { };

static void initWeapon(MeleeWeapon* weapon) {

	b2PolygonShape shape;
	shape.SetAsBox(weapon->m_physics_body_size.x, weapon->m_physics_body_size.y);

	game::physics::FixtureDefBuilder weapon_fixture;
	weapon_fixture.setShape(&shape);
	weapon_fixture.setDensity(50.f);
	weapon_fixture.setFriction(1.f);
	weapon_fixture.setRestitution(0.5f);
	weapon_fixture.setSensor(false);
	weapon_fixture.setCategoryBits(COLLISION_WEAPON);
	weapon_fixture.setMaskBits(COLLISION_WORLD | COLLISION_PLR_OUTER_HITBOX);
	weapon_fixture.setPhysicsData(new PhysicsData(PhysicsBodyType::Weapon, weapon));


	b2Body* body = PhysicsWorld::CreateBody(b2_dynamicBody,
		b2Vec2(weapon->GetPosition().x, weapon->GetPosition().y), 0, 5.f, 5.f, weapon_fixture.getResult());


	weapon->SetPhysicsBody(body);
}

static void initWeapon(RangeWeapon* weapon) {

	b2PolygonShape shape;
	shape.SetAsBox(weapon->m_physics_body_size.x, weapon->m_physics_body_size.y);


	game::physics::FixtureDefBuilder weapon_fixture;
	weapon_fixture.setShape(&shape);
	weapon_fixture.setDensity(50.f);
	weapon_fixture.setFriction(1.f);
	weapon_fixture.setRestitution(0.5f);
	weapon_fixture.setSensor(false);
	weapon_fixture.setCategoryBits(COLLISION_WEAPON);
	weapon_fixture.setMaskBits(COLLISION_WORLD | COLLISION_PLR_OUTER_HITBOX);
	weapon_fixture.setPhysicsData(new PhysicsData(PhysicsBodyType::Weapon, weapon));


	b2Body* body = PhysicsWorld::CreateBody(b2_dynamicBody, 
		b2Vec2(weapon->GetPosition().x, weapon->GetPosition().y), 0, 5.f, 5.f, weapon_fixture.getResult());


	weapon->SetPhysicsBody(body);
}

template <class WeaponType>
WeaponType* WeaponHandler::CreateWeapon(Vector2 position) {
	static_assert(std::is_base_of<MeleeWeapon, WeaponType>::value || std::is_base_of<RangeWeapon, WeaponType>::value,
		"Weapon can't be created, if WeaponType contains class that isn't inherited from MeleeWeapon or RangeWeapon.");

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
			weapon->SetPhysicsBodyTransform(b2Vec2{ FLT_MAX, FLT_MAX });
		}
	}
	for (auto& weapon : m_RangeWeapons) {
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
template wp_Pistol* WeaponHandler::CreateWeapon<wp_Pistol>(Vector2 position);

void WeaponHandler::AddWeapon(MeleeWeapon* weapon) {
	m_Weapons.push_back(weapon);
}

void WeaponHandler::AddWeapon(RangeWeapon* weapon) {
	m_RangeWeapons.push_back(weapon);
}