#pragma once

#include <Cyclone/cyclone.h>
#include <Object.h>

class SimpleScene
{
	public:
		SimpleScene();
		~SimpleScene();
		
		void reset();
		void generateContacts();
		void update(float duration);
		void draw(int shadow);

		cyclone::Contact *contacts;
		cyclone::CollisionData *cData;
		cyclone::ContactResolver *resolver;

		Mover movers[5];

	private:
		int m_maxPossibleContact = 50;
};