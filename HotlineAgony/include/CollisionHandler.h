#include <box2d/box2d.h>
#include "Character.h"
#include <algorithm>

using game::living::Character;

// That class handles physics collisions 
class ContactListener : public b2ContactListener {
	void BeginContact(b2Contact* contact) override {
		b2Fixture* a = contact->GetFixtureA();
		b2Fixture* b = contact->GetFixtureB();

		// If user data is not set for any of objects
		if (a->GetUserData().pointer == 0 || a->GetUserData().pointer == 0) {
			return;
		}

		std::string nameA = reinterpret_cast<PhysicsData*>(a->GetUserData().pointer)->name;
        std::string nameB = reinterpret_cast<PhysicsData*>(b->GetUserData().pointer)->name;

		PhysicsData* dataA = reinterpret_cast<PhysicsData*>(a->GetUserData().pointer);
		PhysicsData* dataB = reinterpret_cast<PhysicsData*>(b->GetUserData().pointer);

		if (nameA == "playerCollision" && nameB == "Weapon") {
			reinterpret_cast<Character*>(dataA->owner)->m_collidingWeapons.push_back(reinterpret_cast<MeleeWeapon*>(dataB->owner));
		}
		else if (nameA == "Weapon" && nameB == "playerCollision") {
    			reinterpret_cast<Character*>(dataB->owner)->m_collidingWeapons.push_back(reinterpret_cast<MeleeWeapon*>(dataA->owner));
		}
	}

	void EndContact(b2Contact* contact) override {
		b2Fixture* a = contact->GetFixtureA();
		b2Fixture* b = contact->GetFixtureB();

		if (a->GetUserData().pointer == 0 || a->GetUserData().pointer == 0) {
			return;
		}

		PhysicsData* dataA = reinterpret_cast<PhysicsData*>(a->GetUserData().pointer);
		PhysicsData* dataB = reinterpret_cast<PhysicsData*>(b->GetUserData().pointer);

        std::string nameA = dataA->name;
        std::string nameB = dataB->name;

		b2Vec2 posA = a->GetBody()->GetPosition();
		b2Vec2 posB = b->GetBody()->GetPosition();
		float degrees = atan2(posA.y - posB.y, posA.x - posB.x) * RAD2DEG;
		b2Vec2 Velocity = b2Vec2(cos(degrees), sin(degrees));
		if (nameA == "playerCollision" && nameB == "Weapon") {
			Character* character = reinterpret_cast<Character*>(dataA->owner);
			MeleeWeapon* weapon = reinterpret_cast<MeleeWeapon*>(dataB->owner);
			Velocity *= 15;
			//b->GetBody()->SetLinearVelocity(Velocity);
            auto it = std::find(character->m_collidingWeapons.begin(), character->m_collidingWeapons.end(), weapon);
            character->m_collidingWeapons.erase(it);
		}
		else if (nameA == "Weapon" && nameB == "playerCollision") {
			Character* character = reinterpret_cast<Character*>(dataB->owner);
			MeleeWeapon* weapon = reinterpret_cast<MeleeWeapon*>(dataA->owner);
			Velocity *= 15;

			//a->GetBody()->SetLinearVelocity(Velocity);
            auto it = std::find(character->m_collidingWeapons.begin(), character->m_collidingWeapons.end(), weapon);
            character->m_collidingWeapons.erase(it);
		}
        if (nameA == "Player" && nameB == "Weapon") {
            b->GetBody()->SetAngularVelocity(rand() % 2 == 0 ? rand() % 2 + 2 : -(rand() % 2 + 2));
        }
        else if (nameA == "Weapon" && nameB == "Player") {
            a->GetBody()->SetAngularVelocity(rand() % 2 == 0 ? rand() % 2 + 2 : -(rand() % 2 + 2));
        }
	}
};