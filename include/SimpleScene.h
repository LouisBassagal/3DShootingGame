#pragma once

#include <Cyclone/cyclone.h>
#include <Ball.hpp>
#include <Basket.hpp>
#include <Gameplay.hpp>
#include <memory>

class SimpleScene
{
	public:
		SimpleScene();
		~SimpleScene();
		
		void reset();
		void generateContacts();
		void update(float duration);
		void draw(int shadow);
		void initGameplay();
		void setIsBallToShoot(bool b);

		Ball* getBalls();
		int getGameplayTime();
		int getGameplayScore();
		int getBallToShoot();

		cyclone::Contact *contacts;
		cyclone::CollisionData *cData;
		cyclone::ContactResolver *resolver;

		Ball balls[5];
		Basket basket;

	private:
		int m_maxPossibleContact = 100;
		std::unique_ptr<Gameplay> m_gameplay;
};