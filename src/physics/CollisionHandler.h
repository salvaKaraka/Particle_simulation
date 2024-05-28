#ifndef COLLISIONHANDLER_H
#define  COLLISIONHANDLER_H

#include "Particle.h"
#include "Container.h"
#include "../Settings.h"

#include <vector>
#include <array>
#include <cmath>
#include <memory>



class CollisionHandler
{
private:
	std::unique_ptr<Container> container = std::make_unique<BoxContainer>(1, 1);
	std::array<float, 2> gravity = {0.0f, GRAVITY};

	float calculateDistance(Particle& p1, Particle& p2);
	bool collide(Particle& p1, Particle& p2);
	void handleWorldCollisions();
	void handleParticleCollisions(Particle& p1, Particle& p2);

public:

	std::vector<Particle> particles;

	CollisionHandler() {};

	void addParticle(Particle p);
	void removeParticle();

	void setContainer(const char* strategyType, float w, float h, float r);


	void updatePositions(float dt, unsigned int sub_steps);

	void changeGravity(char direction);
	void applyAttraction(std::array<float, 2>& cursor_pos, float attraction_radius, float attraction_strength);
};

#endif

