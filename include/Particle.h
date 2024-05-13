#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

class Particle {
private:
    std::vector<float> pos;
    std::vector<float> nextPos;
    float mass;
    float radius;
    std::vector<float> vel;
    std::vector<float> acc;
    std::vector<float> color;

public:
    Particle(std::vector<float> p, float m, std::vector<float> c, std::vector<float> v, std::vector<float> a);

    void setPosition(std::vector<float> p);
    void setNextPosition(std::vector<float> np);
    void setXPosition(float x);
    void setYPosition(float y);
    void setVelocity(std::vector<float> v);
    void setXVelocity(float vx);
    void setYVelocity(float vy);
    void setAcceleration(std::vector<float> a);
    void setXAcceleration(float ax);
    void setYAcceleration(float ay);
    void setColor(std::vector<float> c);
    void setMass(float m);

    std::vector<float> getPosition();
    std::vector<float> getNextPosition();
    float getXPosition();
    float getYPosition();
    std::vector<float> getVelocity();
    float getXVelocity();
    float getYVelocity();
    std::vector<float> getAcceleration();
    float getXAcceleration();
    float getYAcceleration();
    std::vector<float> getColor();
    float getRadius();
    float getMass();

    void draw();
};

#endif
