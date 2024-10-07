#pragma once
#include "raylib.h"

class MeleeWeapon {
public:
	virtual void Attack() = 0;
	virtual ~MeleeWeapon() = default;
protected:
	float attackCooldown;
};

class Knife : public MeleeWeapon {
public:
	Knife() : attackCooldown(1.5f) {}
};