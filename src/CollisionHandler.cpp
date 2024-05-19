#include "CollisionHandler.h"

const float borderCollisionDamping = 0.9;
const float particleCollisionDamping = 0.9999;


CollisionHandler::CollisionHandler(std::vector<Particle> p, int w, int h, float t, float g) {
	particles = p;
	width = w;
	height = h;
	dt = t;
    gravity = g;
}

void CollisionHandler::setParticles(std::vector<Particle> p) {
    particles = p;
}

void CollisionHandler::updatePositions() {
    for (Particle& p : particles) {

        // Update particle properties
        p.vel[0] += p.acc[0] * dt;
        p.vel[1] += p.acc[1] * dt - gravity * dt;

        p.pos[0] += p.vel[0] * dt;
        p.pos[1] += p.vel[1] * dt;


        // Check border collisions
        handleBorderCollisions(p);

        // Check particle collisions
        for (Particle& p2 : particles) {
            handleParticleCollisions(p, p2);
        }
    }
}

void CollisionHandler::handleBorderCollisions(Particle& p) {
    std::array<float, 2> velocity = p.vel;
    std::array<float, 2> position = p.pos;

    //chack leeft and right
    if (abs(p.pos[0]) + p.radius > float(width)/2) {
        position[0] = (float(width)/2 - p.radius) * sign(position[0]);
        velocity[0]*=-1 * borderCollisionDamping; // Reverse velocity and slow down
    }
    //chack top and bottom
    if (abs(p.pos[1]) + p.radius > height/2) {
        position[1] = (float(height)/2 - p.radius) * sign(position[1]);
        velocity[1] *= -1 * borderCollisionDamping;
    }


    p.pos=position;
    p.vel=velocity;
}

void CollisionHandler::handleParticleCollisions(Particle& p1, Particle& p2) {
    float xv1 = p1.vel[0];
    float yv1 = p1.vel[1];
    float xp1 = p1.pos[0];
    float yp1 = p1.pos[1];

    float xv2 = p2.vel[0];
    float yv2 = p2.vel[1];
    float xp2 = p2.pos[0];
    float yp2 = p2.pos[1];

    float m1 = p1.mass;
    float m2 = p2.mass;

    if (&p1 != &p2) { // Evitar comparar una partícula consigo misma
        if (collide(p1, p2)) {
            // Calcular la diferencia entre las posiciones
            std::array<float, 2> diff_x;
            diff_x[0] = xp1 - xp2;
            diff_x[1] = yp1 - yp2;

            // Calcular el producto punto
            float dot_product = (xv1 - xv2) * diff_x[0] + (yv1 - yv2) * diff_x[1];

            // Calcular la norma al cuadrado
            float norm_squared = diff_x[0] * diff_x[0] + diff_x[1] * diff_x[1];

            // Calcular la componente a componente de la fórmula
            float new_xv1 = (xv1 - (2 * m2 / (m1 + m2)) * dot_product / norm_squared * diff_x[0])* particleCollisionDamping;
            float new_yv1 = (yv1 - (2 * m2 / (m1 + m2)) * dot_product / norm_squared * diff_x[1]) * particleCollisionDamping;

            // Actualizar la velocidad de la partícula p1
            p1.vel={ new_xv1, new_yv1 };
            p1.pos={
                xp1 - (p2.pos[0] - p1.pos[0])/10,
                yp1 - (p2.pos[1] - p1.pos[1])/10
            };
        }
    }
}



bool CollisionHandler::collide(Particle p1, Particle p2) {
    return (calculateDistance(p1, p2) <= p1.radius + p2.radius);
}


float CollisionHandler::calculateDistance(Particle p1, Particle p2) {
    float dx = p2.pos[0] - p1.pos[0];
    float dy = p2.pos[1] - p1.pos[1];
    return sqrt((dx * dx) + (dy * dy));
}


float CollisionHandler::dotProduct(const std::array<float, 2>& v1, const std::array<float, 2>& v2) {
    return v1[0] * v2[0] + v1[1] * v2[1];
}

int CollisionHandler::sign(float i) {
    if (i == 0) return 0;
    else
    {
        return (i / abs(i));
    }
}

std::array<float, 2> CollisionHandler::normalize(const std::array<float, 2>& vec) {
    float magnitude = sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
    if (magnitude != 0) {
        return { vec[0] / magnitude, vec[1] / magnitude };
    }
    else {
        return { 0, 0 }; // En caso de que el vector sea el vector cero
    }
}

std::vector<Particle> CollisionHandler::getParticles() const{
    return particles;
}