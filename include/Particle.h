#ifndef PARTICLE_H
#define PARTICLE_H

#include <array>

class Particle {
private:
    std::array<float, 2> pos;
    std::array<float, 2> nextPos;
    float mass;
    float radius;
    std::array<float, 2> vel;
    std::array<float, 2> acc;
    std::array<float, 3> color;

public:
    Particle(std::array<float, 2> p, float m, std::array<float, 3> c, std::array<float, 2> v, std::array<float, 2> a);

    void setPosition(std::array<float, 2> p);
    void setNextPosition(std::array<float, 2> np);
    void setXPosition(float x);
    void setYPosition(float y);
    void setVelocity(std::array<float, 2> v);
    void setXVelocity(float vx);
    void setYVelocity(float vy);
    void setAcceleration(std::array<float, 2> a);
    void setXAcceleration(float ax);
    void setYAcceleration(float ay);
    void setColor(std::array<float, 3> c);
    void setMass(float m);

    const std::array<float, 2> getPosition();
    const std::array<float, 2> getNextPosition();
    const float getXPosition();
    const float getYPosition();
    const std::array<float, 2> getVelocity();
    const float getXVelocity();
    const float getYVelocity();
    const std::array<float, 2> getAcceleration();
    const float getXAcceleration();
    const float getYAcceleration();
    const std::array<float, 3> getColor();
    const float getRadius();
    const float getMass();

    void draw(); //it showld not be here but for the sake of simplicity we will leave it here for now
};

#endif
