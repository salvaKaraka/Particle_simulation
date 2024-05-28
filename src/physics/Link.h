#pragma once
#include "VerletObject.h"

struct Link {
	VerletObject* object1;
	VerletObject* object2;
	float length;

	Link(VerletObject* obj1, VerletObject* obj2, float l) : object1{ obj1 }, object2{ obj2 }, length{ l } {}

	void solve() {
		VerletObject& obj1 = *object1;
		VerletObject& obj2 = *object2;

		Vec2 link = obj1.current_pos - obj2.current_pos;					//vector between the two objects
		const float link_abs = (link).magnitude();							//distance between the two objects
		const Vec2 link_normal = (link).normalize();						//normal of the vector between the two objects

		const float diff = length - link_abs;								//difference between the distance between the objects and the spected length of the link

		obj1.current_pos = obj1.current_pos + (link_normal * diff * 0.5);	//move object1 half the difference
		obj2.current_pos = obj2.current_pos - (link_normal * diff * 0.5);	//move object2 the other half
	}

};