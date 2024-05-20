#ifndef COLLISIONHANDLER_H
#define  COLLISIONHANDLER_H

#include "Particle.h"
#include <vector>
#include <array>

class CollisionHandler
{
private:
	std::vector<Particle> particles;
	int width;
	int height;
	float dt;
	float gravity;
	float dotProduct(const std::array<float, 2>& v1, const std::array<float, 2>& v2);
	float calculateDistance(Particle p1, Particle p2);
	bool collide(Particle p1, Particle p2);
	int sign(float i);
	std::array<float, 2> normalize(const std::array<float, 2>& vec);
public:
	CollisionHandler(std::vector<Particle> p, int w, int h, float t, float g);
	void updatePositions();
	void handleParticleCollisions(Particle& p1, Particle& p2);
	void handleBorderCollisions(Particle& p);
	void setParticles(std::vector<Particle> p);
	std::vector<Particle> getParticles() const;
	void changeGravity();
};

#endif

