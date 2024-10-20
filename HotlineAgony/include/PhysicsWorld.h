#pragma once
#include <memory>
#include <string>
#include <raylib.h>
#include <box2d/box2d.h>

struct PhysicsData {
	const char* name;
	void* owner;
};

class PhysicsWorld {
public:
	static b2World& GetWorld();
private:
	static std::unique_ptr<b2World> m_world;
};

// That class handles physics collisions 
class ContactListener : public b2ContactListener {
	void BeginContact(b2Contact* contact) override {
		b2Fixture* a = contact->GetFixtureA();
		b2Fixture* b = contact->GetFixtureB();

		// If user data is not set for any of objects
		if (a->GetUserData().pointer == 0 || a->GetUserData().pointer == 0) {
			return;
		}

		const char* nameA = reinterpret_cast<PhysicsData*>(a->GetUserData().pointer)->name;
		const char* nameB = reinterpret_cast<PhysicsData*>(b->GetUserData().pointer)->name;

		if (nameA == "Player" && nameB == "Weapon") {
			b->SetSensor(true);
		}
		else if (nameA == "Weapon" && nameB == "Player") {
			a->SetSensor(true);
		}
	}
	void EndContact(b2Contact* contact) override {
		b2Fixture* a = contact->GetFixtureA();
		b2Fixture* b = contact->GetFixtureB();

		if (a->GetUserData().pointer == 0 || a->GetUserData().pointer == 0) {
			return;
		}

		const char* nameA = reinterpret_cast<PhysicsData*>(a->GetUserData().pointer)->name;
		const char* nameB = reinterpret_cast<PhysicsData*>(b->GetUserData().pointer)->name;

		b2Vec2 posA = a->GetBody()->GetPosition();
		b2Vec2 posB = b->GetBody()->GetPosition();
		float degrees = atan2(posA.y - posB.y, posA.x - posB.x) * RAD2DEG;
		b2Vec2 Velocity = b2Vec2(cos(degrees), sin(degrees));
		if (nameA == "Player" && nameB == "Weapon") {
			Velocity *= 15;
			b->SetSensor(true);
			b->GetBody()->SetLinearVelocity(Velocity);
			b->GetBody()->SetAngularVelocity(rand() % 2 == 0 ? rand() % 2 + 2 : -(rand() % 2 + 2));
		}
		else if (nameA == "Weapon" && nameB == "Player") {
			Velocity *= 15;
			a->SetSensor(true);
			a->GetBody()->SetLinearVelocity(Velocity);
			a->GetBody()->SetAngularVelocity(rand() % 2 == 0 ? rand() % 2 + 2 : -(rand() % 2 + 2));
		}
		
	}
};
