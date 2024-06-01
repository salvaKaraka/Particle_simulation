#include "PhysicsEngine.h"


void PhysicsEngine::udateSubSteps() {
	time += frame_dt;
	for (unsigned int i = 0; i < sub_steps; i++) {
		update(getStepDt());
	}
}

void PhysicsEngine::update(float dt) {
	applyGravity();
	updatePositions(dt);
	Solver::solve(VerletObjects,Links, container);
}

void PhysicsEngine::updatePositions(float dt) {
	for (VerletObject& object : VerletObjects) {
		object.updatePosition(dt);
	}
}

VerletObject& PhysicsEngine::addVerletObject(Vec2 pos, float rad) {
	VerletObject object(pos, rad);
	VerletObjects.emplace_back(VerletObject(pos, rad));
	return VerletObjects.back();
}

void PhysicsEngine::removeVerletObject() {
	if (VerletObjects.size() > 0) {
		VerletObjects.pop_back();
	}
}

void PhysicsEngine::setContainer(const char* strategyType, float w, float h, float r) {
	if (strcmp(strategyType, "box") == 0) {
		container = std::make_unique<BoxContainer>(w, h);
	}
	else if (strcmp(strategyType, "circle") == 0) {
		container = std::make_unique<CircleContainer>(r);
	}
	else
	{
		container = std::make_unique<BoxContainer>(w, h);
	}
}


void PhysicsEngine::applyGravity() {
	for (VerletObject& object : VerletObjects) {
		object.accelerate(gravity);
	}
}

void PhysicsEngine::setObjectVelocity(VerletObject& object, Vec2 vel) const {
	object.setVelocity(vel, getStepDt());
}


void PhysicsEngine::applyAttraction(Vec2& cursor_pos, float attraction_radius, float attraction_strength) {
	for (VerletObject& object : VerletObjects) {
		// Calculate the direction vector from the particle to the cursor
		Vec2 direction = cursor_pos - object.current_pos;

		// Calculate the distance
		float distance = direction.magnitude();

		// Apply force only if the particle is within the attraction radius
		if (distance > 0 && distance <= attraction_radius) {
			direction = direction.normalize();

			// Proportional attraction strength based on distance
			float strength = attraction_strength * (attraction_radius - distance) / attraction_radius;

			// Apply an attraction force towards the cursor
			object.acc = direction * strength;
		}
		else {
			// If outside the attraction radius, no acceleration
			object.acc = { 0.0f, 0.0f };
		}
	}
}
