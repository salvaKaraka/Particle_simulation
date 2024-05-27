#ifndef PARTICLE_H
#define PARTICLE_H

#include <array>

class Particle {

public:
    Particle(std::array<float, 2> p, float m, std::array<float, 3> c, std::array<float, 2> v, std::array<float, 2> a);
	Particle() = default;
	void update();

    std::array<float, 2> pos = { 0 };
    std::array<float, 2> nextPos = { 0 };
    float mass = 0;
    float radius = 0;
    std::array<float, 2> vel = { 0 };
    std::array<float, 2> acc = { 0 };
    std::array<float, 3> color = { 0 };
};

#endif
