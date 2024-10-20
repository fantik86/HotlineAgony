#pragma once
#include "Weapon.h"

class WeaponHandler {
public:
	static void DrawWeapons();

	template <class WeaponType>
	static WeaponType* CreateWeapon(Vector2 position);

	static void AddWeapon(MeleeWeapon* weapon);
private:
	static std::vector<MeleeWeapon*> m_Weapons;
};