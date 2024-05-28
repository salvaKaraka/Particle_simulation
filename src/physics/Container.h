#pragma once
#include "../Settings.h"
#include "../utils/SKMath.h"
#include "VerletObject.h"

struct Container {
	virtual ~Container() = default;
	virtual void solveBorderCollisions(VerletObject& object) const = 0;
};

struct BoxContainer : public Container {
	float width = 1;
	float height = 1;
	BoxContainer(float w, float h) : width(w), height(h) {}
	void solveBorderCollisions(VerletObject& object) const override {
			//chack left and right
			if (abs(object.current_pos.x) + object.radius > width / 2.0f) {
				object.current_pos.x = (width / 2.0f - object.radius) * (object.current_pos.x > 0 ? 1 : -1);
			}
			//chack top and bottom
			if (abs(object.current_pos.y) + object.radius > height / 2.0f) {
				object.current_pos.y = (height / 2.0f - object.radius) * (object.current_pos.y > 0 ? 1 : -1);
			}
	};
};

struct CircleContainer : public Container {
	float radius = 1;
	CircleContainer(float r) : radius(r) {}
	void solveBorderCollisions(VerletObject& object) const override {
			float distFromCenter = object.current_pos.magnitude();
			if (distFromCenter + object.radius > radius) {
				Vec2 norm = object.current_pos.normalize();
				object.current_pos = norm * (radius - object.radius);
			};
	};
};
