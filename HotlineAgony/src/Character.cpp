#include "Character.h"
#include "raylib.h"
#include "options.h"
#include "Animation.h"


using game::living::Character;


Character::Character() : m_health(100), m_max_health(100) {}

Character::Character(float health) : m_health(health), m_max_health(health) {}

Character::Character(float health, MeleeWeapon* weapon) : m_health(100), m_max_health(100), holdingWeapon(weapon) {}

/**
* @brief Returns \ref Character "Character's" health
* @return m_health
*/
float Character::getHealth() const {
	return m_health;
}

void Character::setHealth(float value) {
	m_health = value;
}

void Character::incHealth(float value) {
	m_health += value;
}

void Character::decHealth(float value) {
	m_health -= value;
}

Vector2 Character::getWalkingDirection() const {
	return walking_direction;
}

void Character::setWalkingDirection(Vector2 new_walking_direction) {
	walking_direction = new_walking_direction;
}

void Character::setWalkingDirectionX(float new_X) {
	walking_direction.x = new_X;
}

void Character::setWalkingDirectionY(float new_Y) {
	walking_direction.y = new_Y;
}

int Character::kill() {
	m_health = 0;
	return 0;
}

int Character::revive() {
	m_health = m_max_health;
	return 0;
}

CharacterState Character::getState() const {
	return state;
}

void Character::setState(CharacterState new_state) {
	if (state == new_state) {
		return;
	}
	
	state = new_state;
}

int Character::getWalkspeed() const {
	return walkspeed;
}

void Character::setWalkspeed(int value) {
	walkspeed = value;
}