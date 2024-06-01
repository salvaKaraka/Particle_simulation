#include "Solver.h"

void Solver::solve(std::vector<VerletObject>& VerletObjects, std::vector<Link>& links, std::unique_ptr<Container>& container) {
	for (VerletObject& object : VerletObjects) {                                                    //check for collisions with the container for each object
        container->solveBorderCollisions(object);
    }
	for (size_t i = 0; i < VerletObjects.size(); i++) {												//check for collisions between objects	O(n^2)	
        for (size_t j = i + 1; j < VerletObjects.size(); j++) {
            solveVerletObjectCollision(VerletObjects[i], VerletObjects[j]);
        }
    }

	for (Link& link : links) {
		link.solve();
	}
}

void Solver::solveVerletObjectCollision(VerletObject& object1, VerletObject& object2) {
	if (&object1 != &object2){
		const float distance = (object1.current_pos - object2.current_pos).magnitude();

		if (distance < (object1.radius + object2.radius)){														//if objects overlap
			const Vec2 normal = (object1.current_pos - object2.current_pos).normalize();
			const float overlap = (object1.radius + object2.radius) - distance;

			const float mass1 = object1.radius / (object1.radius + object2.radius);								//calculate mass from radii	
			const float mass2 = object2.radius / (object1.radius + object2.radius);

			const float response_coef = 0.75f;																	//coefficient

			object1.current_pos = object1.current_pos + (normal * overlap * mass2 * response_coef * 0.5);		// move object1 half the overlap distance
			object2.current_pos = object2.current_pos - (normal * overlap * mass1 * response_coef *0.5);		// move object2 the other half
		}
	}
}




