#pragma once

#include <Ball.hpp>

constexpr double BALL_SPAWN[3] = { 0, 15, -25 } ;
constexpr int TIMEOUT = 180000;

class Gameplay
{
	public:
		Gameplay(Ball *balls, int number);
		~Gameplay();

		void init();
		void draw(int shadow);
		void update(double deltaTime);
		void setIsBallToShoot(bool b);

		int getGameplayScore();
		int getGameplayTime();
		int getBallToShoot();

	private:
		void changeBallToShoot();

		size_t m_toShoot;
		size_t m_numberOfBalls;
		int m_score;
		int m_time{TIMEOUT};
		Ball *m_balls;

		bool m_isBallToShoot{ false };

		float m_changeBallTimer{ 0.0f };
};