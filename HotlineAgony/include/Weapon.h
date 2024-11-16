#pragma once
#include <raylib.h>
#include <box2d/box2d.h>
#include <string>
#include <vector>
#include "utils.h"


class MeleeWeapon {
protected:
	MeleeWeapon() :
		m_lyingSprite(LoadTexture(std::string(GetApplicationDirectory()).append("/assets/weapons/wp_Unknown.png").c_str())),
		m_weapon_type(WeaponType::wp_Unknown) {}

	MeleeWeapon(WeaponType weapon_type) :
		m_lyingSprite(LoadTexture(std::string(GetApplicationDirectory()).append("/assets/weapons/wp_Unknown.png").c_str())),
		m_weapon_type(weapon_type) {}

	MeleeWeapon(WeaponType weapon_type, Texture2D lyingSprite) :
		m_lyingSprite(lyingSprite), 
		m_weapon_type(weapon_type) {}


	Vector2 m_position = Vector2{ 0, 0 };
	b2Body* m_physics_body = nullptr;
	Texture2D m_lyingSprite;

	bool m_onGround = true;
	bool m_canDrop = true;
	bool m_canAttack = true;
	float m_attackCooldown = 0.f;
public:
	virtual ~MeleeWeapon() = default;

	virtual void Attack() = 0;
	virtual void Drop() = 0;
	virtual void Throw() = 0;

	inline Vector2 GetPosition() const {
		return m_position;
	}
	inline void SetPosition(Vector2 position) {
		m_position = position;
	}
	inline void SetPhysicsBodyTransform(b2Vec2 position, float angle) {
		m_physics_body->SetTransform(position, angle);
	}
	inline void SetPhysicsBodyTransform(b2Vec2 position) {
		m_physics_body->SetTransform(position, m_physics_body->GetAngle());
	}
	inline Texture2D& GetLyingSprite() {
		return m_lyingSprite;
	}
	inline b2Body* GetPhysicsBody() {
		return m_physics_body;
	}
	inline void SetPhysicsBody(b2Body* physics_body) {
		m_physics_body = physics_body;
	}
	inline bool isOnGround() const {
		return m_onGround;
	}
	inline void SetOnGround(bool on_ground) {
		m_onGround = on_ground;
	}

	WeaponType m_weapon_type;
	Vector2 m_physics_body_size = Vector2{ 12.f, 5.f };
	Vector2 m_physics_body_align_center = Vector2{ 6.f, 5.f };
};

class RangeWeapon {
protected:
	RangeWeapon() :
		m_lyingSprite(LoadTexture(std::string(GetApplicationDirectory()).append("/assets/weapons/wp_Unknown.png").c_str())),
		m_weapon_type(WeaponType::wp_Unknown) {}

	RangeWeapon(WeaponType weapon_type) :
		m_lyingSprite(LoadTexture(std::string(GetApplicationDirectory()).append("/assets/weapons/wp_Unknown.png").c_str())),
		m_weapon_type(weapon_type) {}

	RangeWeapon(WeaponType weapon_type, Texture2D lyingSprite) :
		m_lyingSprite(lyingSprite),
		m_weapon_type(weapon_type) {}


	Vector2 m_position = Vector2{ 0, 0 };
	b2Body* m_physics_body = nullptr;
	Texture2D m_lyingSprite;

	bool m_onGround = true;
	bool m_canDrop = true;
	bool m_canAttack = true;
	float m_attackCooldown = 0.f;
public:
	virtual ~RangeWeapon() = default;

	virtual void Shoot() = 0;
	virtual void Reload() = 0;
	virtual void Drop() = 0;
	virtual void Throw() = 0;

	inline Vector2 GetPosition() const {
		return m_position;
	}
	inline void SetPosition(Vector2 position) {
		m_position = position;
	}
	inline void SetPhysicsBodyPosition(b2Vec2 position) {
		m_physics_body->SetTransform(position, m_physics_body->GetAngle());
	}
	inline Texture2D& GetLyingSprite() {
		return m_lyingSprite;
	}
	inline b2Body* GetPhysicsBody() {
		return m_physics_body;
	}
	inline void SetPhysicsBody(b2Body* physics_body) {
		m_physics_body = physics_body;
	}
	inline bool isOnGround() const {
		return m_onGround;
	}
	inline void SetOnGround(bool on_ground) {
		m_onGround = on_ground;
	}

	WeaponType m_weapon_type;
	int ammoCount = 8;
	Vector2 m_physics_body_size = Vector2{ 12.f, 5.f };
	Vector2 m_physics_body_align_center = Vector2{ 6.f, 5.f };
};

class wp_Fists : public MeleeWeapon {
public:
	wp_Fists() : MeleeWeapon(WeaponType::wp_Fists) {
		m_onGround = false;
		m_canDrop = false;
		m_attackCooldown = 1.f;
		m_canAttack = true;
	}
	void Attack() override;
	void Drop() override;
	void Throw() override;
};

class wp_Knife : public MeleeWeapon {
public:
	wp_Knife() : MeleeWeapon(WeaponType::wp_Knife, LoadTexture(std::string(GetApplicationDirectory()).append("/assets/weapons/wp_Knife.png").c_str())) {
		m_position = Vector2{ 0, 0 };
		m_onGround = true;
		m_canDrop = true;
		m_attackCooldown = 1.f;
		m_canAttack = true;
		m_physics_body_size = Vector2{ 7.8f, 2.f };
	}
	wp_Knife(Vector2 position) : MeleeWeapon(WeaponType::wp_Knife, LoadTexture(std::string(GetApplicationDirectory()).append("/assets/weapons/wp_Knife.png").c_str())) {
		m_position = position;
		m_onGround = true;
		m_canDrop = true;
		m_attackCooldown = 1.f;
		m_canAttack = true;
		m_physics_body_size = Vector2{ 7.8f, 2.f };
	}
	void Attack() override;
	void Drop() override;
	void Throw() override;
};

class wp_Pistol : public RangeWeapon {
public:
	wp_Pistol() : RangeWeapon(WeaponType::wp_Pistol, LoadTexture(std::string(GetApplicationDirectory()).append("/assets/weapons/wp_Pistol.png").c_str())) {
		m_position = Vector2{ 0, 0 };
		m_onGround = true;
		m_canDrop = true;
		m_attackCooldown = 1.f;
		m_canAttack = true;
		m_physics_body_size = Vector2{ 7.f, 3.f };
	}
	wp_Pistol(Vector2 position) : RangeWeapon(WeaponType::wp_Pistol, LoadTexture(std::string(GetApplicationDirectory()).append("/assets/weapons/wp_Pistol.png").c_str())) {
		m_position = position;
		m_onGround = true;
		m_canDrop = true;
		m_attackCooldown = 1.f;
		m_canAttack = true;
		m_physics_body_size = Vector2{ 7.f, 3.f };
	}
	void Shoot() override;
	void Reload() override;
	void Drop() override;
	void Throw() override;
};