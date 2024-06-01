#pragma once
#include <utils/SKMath.h>

#include "VerletObject.h"
#include "Link.h"
#include "Container.h"
#include "Solver.h"
#include <vector>

class PhysicsEngine {

public:
	PhysicsEngine() : gravity{0.0f, -9.8f} {};
	~PhysicsEngine() {};

	void udateSubSteps();

	VerletObject& addVerletObject(Vec2 pos, float rad);
	void addLink(VerletObject* obj1, VerletObject* obj2, float length) { Links.emplace_back(obj1, obj2, length); };
	void removeVerletObject();

	void applyAttraction(Vec2& cursor_pos, float attraction_radius, float attraction_strength);
	
	void setFrameRate(float frames) { frame_dt = 1.0f/frames; };
	void setDeltaTime(float dt) { frame_dt = dt; };
	void setSubSteps(unsigned int steps) { sub_steps = steps; };
	void setObjectVelocity(VerletObject& object, Vec2 vel) const;
	void setContainer(const char* strategyType, float w, float h, float r);
	void setGravity(Vec2 g) { gravity = g; };

	Vec2 getGravity() const { return gravity; };
	std::vector<VerletObject> getVerletObjects() const { return VerletObjects; };
	Container* getContainer() const { return container.get(); };
	float getTime() const { return time; };
	float getStepDt() const { return frame_dt / static_cast<float>(sub_steps); };


private:
	Vec2 gravity = {0.0f,-9.8f};
	std::vector<VerletObject> VerletObjects;
	std::vector<Link> Links;
	std::unique_ptr<Container> container = std::make_unique<BoxContainer>(1, 1);

	float time=0.0f;
	float frame_dt=0.0f;
	unsigned int sub_steps = 1;

	void update(float dt);
	void updatePositions(float dt);
	void applyGravity();
};