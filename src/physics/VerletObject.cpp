#include "VerletObject.h"


void VerletObject::updatePosition(float dt) {
	const Vec2 vel = current_pos - prev_pos;
	prev_pos = current_pos;
	current_pos = current_pos + vel + (acc * dt* dt);
	acc = { 0.0f , 0.0f};
}