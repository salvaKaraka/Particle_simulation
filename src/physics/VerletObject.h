#pragma once

#include "Settings.h"

#include <utils/SKMath.h>

struct VerletObject {

    VerletObject(Vec2 p, float r) : current_pos{ p }, prev_pos{ 0.0f, 0.0f }, acc{ 0.0f, 0.0f }, radius{ r } {}

	void updatePosition(float dt);
    void accelerate(Vec2 a) { acc = acc + a; };
	void setVelocity(Vec2 v, float dt) { prev_pos = current_pos + v * dt; };
	void addVelocity(Vec2 v, float dt) { prev_pos = prev_pos - v * dt; };

	Vec2 getVelocity(float dt) const { return (current_pos - prev_pos)/dt; };

    Vec2 prev_pos;
    Vec2 current_pos;
    Vec2 acc;
    float radius = RADIUS;
};
