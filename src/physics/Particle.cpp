#include "Particle.h"
#include <math.h>
#include <glad/glad.h>
#include <vector>

Particle::Particle(std::array<float, 2> p, float m, std::array<float, 3> c, std::array<float, 2> v, std::array<float, 2> a) : pos(p), mass(m), color(c), vel(v), acc(a) {
	radius = mass;
	nextPos = pos;
}

void Particle::update() {
	pos = nextPos;
}