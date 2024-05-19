#ifndef PARTICLE_H
#define PARTICLE_H

#include <array>

class Particle {

public:
    Particle(std::array<float, 2> p, float m, std::array<float, 3> c, std::array<float, 2> v, std::array<float, 2> a);

    std::array<float, 2> pos;
    std::array<float, 2> nextPos;
    float mass;
    float radius;
    std::array<float, 2> vel;
    std::array<float, 2> acc;
    std::array<float, 3> color;
};

#endif
