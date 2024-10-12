#pragma once
#include "raylib.h"

class BaseWeapon
{
public:
	BaseWeapon() = 0;
	virtual ~BaseWeapon() = default;
	virtual void Drop() = 0;
	virtual void Throw() = 0;
protected:
	bool canDrop;
};


class MeleeWeapon : BaseWeapon {
public:
	MeleeWeapon() = 0;
	virtual ~MeleeWeapon() = default;

	virtual void Attack() = 0;
protected:
	float attackCooldown;
	bool canAttack;
};

class RangeWeapon : BaseWeapon {
public:
	RangeWeapon() = 0;
	virtual ~RangeWeapon() = default;

	virtual void Shoot() = 0;
	virtual void Reload() = 0;
protected:
	float shootCooldown;
	bool canShoot;
};