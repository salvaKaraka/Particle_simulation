#ifndef COLLISIONHANDLER_H
#define  COLLISIONHANDLER_H

#include "Particle.h"
#include <vector>
#include <array>
#include "../Settings.h"
#include <cmath>

class CollisionHandler
{
private:

	

	unsigned int width;
	unsigned int height;
	std::array<float, 2> gravity = {0.0f, GRAVITY};

	float calculateDistance(Particle& p1, Particle& p2);
	bool collide(Particle& p1, Particle& p2);
	void handleParticleCollisions(Particle& p1, Particle& p2);
	void handleBorderCollisions(Particle& p) const;


public:

	std::vector<Particle> particles;

	CollisionHandler(std::vector<Particle> p, unsigned int w, unsigned int h);
	void updatePositions(float dt, unsigned int sub_steps);
	void changeGravity(char direction);
	void applyAttraction(std::array<float, 2>& cursor_pos, float attraction_radius, float attraction_strength);
};

#endif

