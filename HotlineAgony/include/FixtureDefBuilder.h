#pragma once
#include "box2d/box2d.h"
#include "PhysicsDefs.h"

namespace game::physics {
	class FixtureDefBuilder {
	public:
		void setShape(b2Shape* shape) {
			fixtureDef.shape = shape;
		}
		void setDensity(float density) {
			fixtureDef.density = density;
		}
		void setFriction(float friction) {
			fixtureDef.friction = friction;
		}
		void setRestitution(float restitution) {
			fixtureDef.restitution = restitution;
		}
		void setSensor(bool sensor) {
			fixtureDef.isSensor = sensor;
		}
		void setCategoryBits(uint16 category_bits) {
			fixtureDef.filter.categoryBits = category_bits;
		}
		void setMaskBits(uint16 mask_bits) {
			fixtureDef.filter.maskBits = mask_bits;
		}
		void setPhysicsData(PhysicsData* data) {
			b2FixtureUserData userData;
			userData.pointer = reinterpret_cast<uintptr_t>(data);
			fixtureDef.userData = userData;
		}
		b2FixtureDef getResult() {
			return fixtureDef;
		}
	private:
		b2FixtureDef fixtureDef;
	};
}