#pragma once
#include <box2d/box2d.h>
#include "Character.h"
#include <algorithm>
#include "PhysicsDefs.h"

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

		PhysicsData* dataA = reinterpret_cast<PhysicsData*>(a->GetUserData().pointer);
		PhysicsData* dataB = reinterpret_cast<PhysicsData*>(b->GetUserData().pointer);

		PhysicsBodyType typeA = dataA->body_type;
		PhysicsBodyType typeB = dataB->body_type;


		if (typeA == PhysicsBodyType::CollisionBox && typeB == PhysicsBodyType::Weapon) {
			reinterpret_cast<Character*>(dataA->owner)->m_collidingWeapons.push_back(reinterpret_cast<MeleeWeapon*>(dataB->owner));
		}
		else if (typeA == PhysicsBodyType::Weapon && typeB == PhysicsBodyType::CollisionBox) {
    		reinterpret_cast<Character*>(dataB->owner)->m_collidingWeapons.push_back(reinterpret_cast<MeleeWeapon*>(dataA->owner));
		}

		if (typeA == PhysicsBodyType::Bullet && typeB == PhysicsBodyType::Wall) {
			dataA->isFlaggedToDelete = true;
		}
		else if (typeA == PhysicsBodyType::Wall && typeB == PhysicsBodyType::Bullet) {
			dataB->isFlaggedToDelete = true;
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

		PhysicsBodyType typeA = dataA->body_type;
		PhysicsBodyType typeB = dataB->body_type;

		b2Vec2 posA = a->GetBody()->GetPosition();
		b2Vec2 posB = b->GetBody()->GetPosition();
		float degrees = atan2(posA.y - posB.y, posA.x - posB.x) * RAD2DEG;
		b2Vec2 Velocity = b2Vec2(cos(degrees), sin(degrees));
		if (typeA == PhysicsBodyType::CollisionBox && typeB == PhysicsBodyType::Weapon) {
			Character* character = reinterpret_cast<Character*>(dataA->owner);
			MeleeWeapon* weapon = reinterpret_cast<MeleeWeapon*>(dataB->owner);
			Velocity *= 15;
			//b->GetBody()->SetLinearVelocity(Velocity);
            auto it = std::find(character->m_collidingWeapons.begin(), character->m_collidingWeapons.end(), weapon);
            character->m_collidingWeapons.erase(it);
		}
		else if (typeA == PhysicsBodyType::Weapon && typeB == PhysicsBodyType::CollisionBox) {
			Character* character = reinterpret_cast<Character*>(dataB->owner);
			MeleeWeapon* weapon = reinterpret_cast<MeleeWeapon*>(dataA->owner);
			Velocity *= 15;

			//a->GetBody()->SetLinearVelocity(Velocity);
            auto it = std::find(character->m_collidingWeapons.begin(), character->m_collidingWeapons.end(), weapon);
            character->m_collidingWeapons.erase(it);
		}
        if (typeA == PhysicsBodyType::Player && typeB == PhysicsBodyType::Weapon) {
            b->GetBody()->SetAngularVelocity(rand() % 2 == 0 ? rand() % 2 + 2 : -(rand() % 2 + 2));
        }
        else if (typeA == PhysicsBodyType::Weapon && typeB == PhysicsBodyType::Player) {
            a->GetBody()->SetAngularVelocity(rand() % 2 == 0 ? rand() % 2 + 2 : -(rand() % 2 + 2));
        }
	}
};