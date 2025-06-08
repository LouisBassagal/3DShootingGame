#pragma once

#include <Cyclone/cyclone.h>
#include <Ball.hpp>
#include <Basket.hpp>

class SimpleScene
{
	public:
		SimpleScene();
		~SimpleScene();
		
		void reset();
		void generateContacts();
		void update(float duration);
		void draw(int shadow);

		Ball* getBalls();

		cyclone::Contact *contacts;
		cyclone::CollisionData *cData;
		cyclone::ContactResolver *resolver;

		Ball balls[5];
		Basket basket;

	private:
		int m_maxPossibleContact = 100;
};