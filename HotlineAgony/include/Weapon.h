#pragma once
#include <raylib.h>
#include <box2d/box2d.h>
#include <string>
#include <vector>


class MeleeWeapon {
protected:
	MeleeWeapon() :
		m_lyingSprite(LoadTexture(std::string(GetApplicationDirectory()).append("/assets/weapons/wp_Unknown.png").c_str())),
		m_weapon_name("wp_Unknown") {}

	MeleeWeapon(std::string weapon_name) :
		m_lyingSprite(LoadTexture(std::string(GetApplicationDirectory()).append("/assets/weapons/wp_Unknown.png").c_str())),
		m_weapon_name(weapon_name) {}

	MeleeWeapon(std::string weapon_name, Texture2D lyingSprite) :
		m_lyingSprite(lyingSprite), 
		m_weapon_name(weapon_name) {}


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

	std::string m_weapon_name;
	Vector2 m_physics_body_size = Vector2{ 12.f, 5.f };
	Vector2 m_physics_body_align_center = Vector2{ 6.f, 5.f };
};

class wp_Fists : public MeleeWeapon {
public:
	wp_Fists() : MeleeWeapon("wp_Fists") {
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
	wp_Knife() : MeleeWeapon("wp_Knife", LoadTexture(std::string(GetApplicationDirectory()).append("/assets/weapons/wp_Knife.png").c_str())) {
		m_position = Vector2{ 0, 0 };
		m_onGround = true;
		m_canDrop = true;
		m_attackCooldown = 1.f;
		m_canAttack = true;
		m_physics_body_size = Vector2{ 7.8f, 2.f };
	}
	wp_Knife(Vector2 position) : MeleeWeapon("wp_Knife", LoadTexture(std::string(GetApplicationDirectory()).append("/assets/weapons/wp_Knife.png").c_str())) {
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