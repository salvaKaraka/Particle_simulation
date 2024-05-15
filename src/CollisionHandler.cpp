#include "CollisionHandler.h"

const float borderCollisionDamping = 0.9;
const float particleCollisionDamping = .9999;


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
        p.setXVelocity(p.getXVelocity() + p.getXAcceleration() * dt);
        p.setYVelocity(p.getYVelocity() + p.getYAcceleration() * dt - gravity * dt);

        p.setXPosition(p.getXPosition() + p.getXVelocity() * dt);
        p.setYPosition(p.getYPosition() + p.getYVelocity() * dt);

        // Check border collisions
        handleBorderCollisions(p);

        // Check particle collisions
        for (Particle& p2 : particles) {
            handleParticleCollisions(p, p2);
        }

        p.draw();
    }
}

void CollisionHandler::handleBorderCollisions(Particle& p) {
    std::array<float, 2> velocity = p.getVelocity();
    std::array<float, 2> position = p.getPosition();

    //chack leeft and right
    if (abs(p.getXPosition()) + p.getRadius() > width/2) {
        position[0] = (width/2 - p.getRadius()) * sign(position[0]);
        velocity[0]*=-1 * borderCollisionDamping; // Reverse velocity and slow down
    }
    //chack top and bottom
    if (abs(p.getYPosition()) + p.getRadius() > height/2) {
        position[1] = (height/2 - p.getRadius()) * sign(position[1]);
        velocity[1] *= -1 * borderCollisionDamping;
    }


    p.setPosition(position);
    p.setVelocity(velocity);
}

void CollisionHandler::handleParticleCollisions(Particle& p1, Particle& p2) {
    float xv1 = p1.getVelocity()[0];
    float yv1 = p1.getVelocity()[1];
    float xp1 = p1.getPosition()[0];
    float yp1 = p1.getPosition()[1];

    float xv2 = p2.getVelocity()[0];
    float yv2 = p2.getVelocity()[1];
    float xp2 = p2.getPosition()[0];
    float yp2 = p2.getPosition()[1];

    float m1 = p1.getMass();
    float m2 = p2.getMass();

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
            p1.setVelocity({ new_xv1, new_yv1 });
            p1.setPosition({
                xp1 - (p2.getPosition()[0] - p1.getPosition()[0])/10,
                yp1 - (p2.getPosition()[1] - p1.getPosition()[1])/10
            });
        }
    }
}



bool CollisionHandler::collide(Particle p1, Particle p2) {
    return (calculateDistance(p1, p2) <= p1.getRadius() + p2.getRadius());
}


float CollisionHandler::calculateDistance(Particle p1, Particle p2) {
    float dx = p2.getPosition()[0] - p1.getPosition()[0];
    float dy = p2.getPosition()[1] - p1.getPosition()[1];
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