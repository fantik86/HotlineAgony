#pragma once
#include <raylib.h>


class MeleeWeapon {
public:
	MeleeWeapon(Vector2 position, Texture2D& lyingSprite) : m_position(position), m_lyingSprite(lyingSprite) {}
	virtual ~MeleeWeapon() = default;

	virtual void Attack() = 0;
	virtual void Drop() = 0;
	virtual void Throw() = 0;
protected:
	Vector2 m_position;
	bool m_onGround;
	bool m_canDrop;
	Texture2D& m_lyingSprite;

	float m_attackCooldown;
	bool m_canAttack;
};

class wp_Knife : MeleeWeapon {
public:
	wp_Knife(Vector2 position, Texture2D& lyingSprite) : MeleeWeapon(position, lyingSprite) {
		m_onGround = true;
		m_canDrop = true;
		m_attackCooldown = 1.f;
		m_canAttack = true;
	}
	void Attack() override;
protected:
};