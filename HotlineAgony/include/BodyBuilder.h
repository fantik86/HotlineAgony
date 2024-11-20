#pragma once
#include "box2d/box2d.h"
#include "PhysicsWorld.h"
#include "PhysicsDefs.h"
#include <stdexcept>

namespace game::physics {
	class BodyBuilder {
	public:
		void setType(b2BodyType type) {
			bodyDef.type = type;
		}
		void setTransform(Vector2 position, float angle) {
			bodyDef.position = b2Vec2(position.x, position.y);
			bodyDef.angle = angle;
		}
		void setPosition(Vector2 position) {
			bodyDef.position = b2Vec2(position.x, position.y);
		}
		void setAllowSleep(bool value) {
			bodyDef.allowSleep = value;
		}
		void setAngularDamping(float value) {
			bodyDef.angularDamping = value;
		}
		void setAngularVelocity(float value) {
			bodyDef.angularVelocity = value;
		}
		void setLinearDamping(float value) {
			bodyDef.linearDamping = value;
		}
		void setLinearVelocity(Vector2 velocity) {
			bodyDef.linearVelocity = b2Vec2(velocity.x, velocity.y);
		}
		void setFixedRotation(bool value) {
			bodyDef.fixedRotation = value;
		}
		void initBody() {
			if (!isInited) {
				body = PhysicsWorld::GetWorld().CreateBody(&bodyDef);
				isInited = true;
			}
			else {
				std::runtime_error("Cannot init body twice!");
			}
		}
		b2Body* getResult() {
			if (isInited) {
				return body;
			}
			else {
				std::runtime_error("Cannot return body that isn't inited!");
			}
		}
	private:
		bool isInited;
		b2BodyDef bodyDef;
		b2Body* body;
	};
}