#pragma once
#include "VerletObject.h"
#include "Container.h"

#include <memory>
#include <utils/SKMath.h>
#include <vector>


class Solver
{
private:

	static void solveVerletObjectCollision(VerletObject& object1, VerletObject& object2);

public:

	static void solveCollisions(std::vector<VerletObject>& VerletObjects, std::unique_ptr<Container>& container);
};

