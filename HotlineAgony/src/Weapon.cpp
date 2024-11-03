#include "Weapon.h"
#include "PhysicsWorld.h"
#include <type_traits>

void wp_Fists::Attack() {
	TraceLog(8, "Attack activated");
}

void wp_Fists::Drop() {
	TraceLog(8, "Drop activated");
}

void wp_Fists::Throw() {
	TraceLog(8, "Throw activated");
}

void wp_Knife::Attack() {
	TraceLog(8, "Attack activated");
}

void wp_Knife::Drop() {
	TraceLog(8, "Drop activated");
}

void wp_Knife::Throw() {
	TraceLog(8, "Throw activated");
}

void wp_Pistol::Shoot() {
	TraceLog(8, "Attack activated");
}

void wp_Pistol::Reload() {
	TraceLog(8, "Attack activated");
}

void wp_Pistol::Drop() {
	TraceLog(8, "Drop activated");
}

void wp_Pistol::Throw() {
	TraceLog(8, "Throw activated");
}