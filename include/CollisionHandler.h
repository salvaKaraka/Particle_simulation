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
	float dotProduct(const std::vector<float>& v1, const std::vector<float>& v2);
	float calculateDistance(Particle p1, Particle p2);
	bool collide(Particle p1, Particle p2);
	int sign(float i);
	std::vector<float> normalize(const std::vector<float>& vec);
public:
	CollisionHandler(std::vector<Particle> p, int w, int h, float t, float g);
	void updatePositions();
	void handleParticleCollisions(Particle& p1, Particle& p2);
	void handleBorderCollisions(Particle& p);
	void setParticles(std::vector<Particle> p);
};

#endif

