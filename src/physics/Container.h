#pragma once
#include "../Settings.h"

struct Container {
	virtual ~Container() = default;
	virtual void handleBorderCollisions(Particle& particle) const = 0;
};

struct BoxContainer : public Container {
	float width = 1;
	float height = 1;
	BoxContainer(float w, float h) : width(w), height(h) {}
	void handleBorderCollisions(Particle& p) const override {
			//chack left and right
			if (abs(p.pos[0]) + p.radius > width / 2.0f) {
				p.pos[0] = (width / 2.0f - p.radius) * (p.pos[0] > 0 ? 1 : -1);
				p.vel[0] *= -1 * BORDER_COLLISION_DAMPING; // Reverse velocity and slow down
			}
			//chack top and bottom
			if (abs(p.pos[1]) + p.radius > height / 2.0f) {
				p.pos[1] = (height / 2.0f - p.radius) * (p.pos[1] > 0 ? 1 : -1);
				p.vel[1] *= -1 * BORDER_COLLISION_DAMPING;
			}
	};
};

struct CircleContainer : public Container {
	float radius = 1;
	CircleContainer(float r) : radius(r) {}
	void handleBorderCollisions(Particle& p) const override {
			float distFromCenter = sqrt(p.pos[0] * p.pos[0] + p.pos[1] * p.pos[1]);
			if (distFromCenter + p.radius > radius) {
				float normX = p.pos[0] / distFromCenter;
				float normY = p.pos[1] / distFromCenter;

				p.pos[0] = normX * (radius - p.radius);
				p.pos[1] = normY * (radius - p.radius);

				float dotProduct = p.vel[0] * normX + p.vel[1] * normY;
				p.vel[0] -= 2 * dotProduct * normX;
				p.vel[1] -= 2 * dotProduct * normY;

				p.vel[0] *= BORDER_COLLISION_DAMPING;
				p.vel[1] *= BORDER_COLLISION_DAMPING;
			}
	};
};
