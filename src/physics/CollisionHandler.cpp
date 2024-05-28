#include "CollisionHandler.h"

void CollisionHandler::addParticle(Particle p) {
	particles.push_back(std::move(p));
}

void CollisionHandler::removeParticle() {
	if (!particles.empty())
		particles.pop_back();
}

void CollisionHandler::setContainer(const char* strategyType, float w, float h, float r) {
    if (strcmp(strategyType, "box") == 0) {
        container = std::make_unique<BoxContainer>(w, h);
    }
    else if (strcmp(strategyType, "circle") == 0) {
        container = std::make_unique<CircleContainer>(r);
    }
    else
	{
		container = std::make_unique<BoxContainer>(w, h);
	}
}

void CollisionHandler::updatePositions(float dt, unsigned int sub_steps) {
    float sub_dt = dt / (float) sub_steps;
    for (unsigned int step = 0; step < sub_steps; ++step) {
        for (Particle& p : particles) {

            // Update particle properties
            p.vel[0] += p.acc[0] * sub_dt + gravity[0] * sub_dt;
            p.vel[1] += p.acc[1] * sub_dt + gravity[1] * sub_dt;

            p.pos[0] += p.vel[0] * sub_dt;
            p.pos[1] += p.vel[1] * sub_dt;
     
        }

        // Check border collisions
        handleWorldCollisions();
        /*for (Particle& p : particles) {
            //chack left and right
            if (abs(p.pos[0]) + p.radius > 1 / 2.0f) {
                p.pos[0] = (1 / 2.0f - p.radius) * (p.pos[0] > 0 ? 1 : -1);
                p.vel[0] *= -1 * BORDER_COLLISION_DAMPING; // Reverse velocity and slow down
            }
            //chack top and bottom
            if (abs(p.pos[1]) + p.radius > 1 / 2.0f) {
                p.pos[1] = (1 / 2.0f - p.radius) * (p.pos[1] > 0 ? 1 : -1);
                p.vel[1] *= -1 * BORDER_COLLISION_DAMPING;
            }
        }*/

        // Check particle collisions
        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t j = i + 1; j < particles.size(); ++j) {
                Particle& p1 = particles[i];
                Particle& p2 = particles[j];
                if (collide(p1, p2))
                    handleParticleCollisions(p1,p2);
            }
        }
    }
}

void CollisionHandler::handleWorldCollisions() {
	for (Particle& p : particles) {
		// Check border collisions
		container->handleBorderCollisions(p);
	}
}


void CollisionHandler::handleParticleCollisions(Particle& p1, Particle& p2) {
        std::array<float, 2>  diff = { p1.pos[0] - p2.pos[0], p1.pos[1] - p2.pos[1] };
        float dist = std::sqrt(diff[0] * diff[0] + diff[1] * diff[1]);
        float norm_squared = dist * dist;

        float dot_product = (p1.vel[0] - p2.vel[0]) * diff[0] + (p1.vel[1] - p2.vel[1]) * diff[1];
        float m1 = p1.mass;
        float m2 = p2.mass;

        float factor1 = (2 * m2 / (m1 + m2)) * dot_product / norm_squared;
        float factor2 = (2 * m1 / (m1 + m2)) * dot_product / norm_squared;

        float p1_vel_x = (p1.vel[0] - factor1 * diff[0]) * PARTICLE_COLLISION_DAMPING;
        float p1_vel_y = (p1.vel[1] - factor1 * diff[1]) * PARTICLE_COLLISION_DAMPING;

        float p2_vel_x = (p2.vel[0] + factor2 * diff[0]) * PARTICLE_COLLISION_DAMPING;
        float p2_vel_y = (p2.vel[1] + factor2 * diff[1]) * PARTICLE_COLLISION_DAMPING;

		p1.vel = { p1_vel_x, p1_vel_y };
		p2.vel = { p2_vel_x, p2_vel_y };

        // Adjust positions to avoid overlap
        float overlap = p1.radius + p2.radius - dist;
        // Normalized direction vector
        std::array<float, 2> dir = { diff[0] / dist, diff[1] / dist };

        // Move particles in opposite directions based on overlap
        p1.pos[0] += dir[0] * overlap / 2;
        p1.pos[1] += dir[1] * overlap / 2;
        p2.pos[0] -= dir[0] * overlap / 2;
        p2.pos[1] -= dir[1] * overlap / 2;
}


bool CollisionHandler::collide(Particle& p1, Particle& p2) {
    return (calculateDistance(p1, p2) <= p1.radius + p2.radius);
}


float CollisionHandler::calculateDistance(Particle& p1, Particle& p2) {
    float dx = p2.pos[0] - p1.pos[0];
    float dy = p2.pos[1] - p1.pos[1];
    return sqrt((dx * dx) + (dy * dy));
}

void CollisionHandler::changeGravity(char direction) {
    switch (direction)
    {
		case 'u':
            gravity = {0.0f , -GRAVITY};
			break;
		case 'r':
			gravity = { GRAVITY , 0.0f};
			break;
		case 'd':
            gravity = { 0.0f , GRAVITY };
			break;
		case 'l':
            gravity = { -GRAVITY , 0.0f };
			break;
		default:
			break;
    }
}

void CollisionHandler::applyAttraction(std::array<float, 2>& cursor_pos, float attraction_radius, float attraction_strength) {
    for (Particle& p : particles) {
        // Calculate the direction vector from the particle to the cursor
        std::array<float, 2> direction = { cursor_pos[0] - p.pos[0], cursor_pos[1] - p.pos[1] };

        // Calculate the distance
        float distance = std::sqrt(direction[0] * direction[0] + direction[1] * direction[1]);

        // Apply force only if the particle is within the attraction radius
        if (distance > 0 && distance <= attraction_radius) {
            direction[0] /= distance;
            direction[1] /= distance;

            // Proportional attraction strength based on distance
            float strength = attraction_strength * (attraction_radius - distance) / attraction_radius;

            // Apply an attraction force towards the cursor
            p.acc[0] = direction[0] * strength;
            p.acc[1] = direction[1] * strength;
        }
        else {
            // If outside the attraction radius, no acceleration
            p.acc[0] = 0;
            p.acc[1] = 0;
        }
    }
}