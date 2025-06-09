#include <Gameplay.hpp>
#include <iostream>
#include <string>

Gameplay::Gameplay(Ball* balls, int number) {
	this->m_balls = balls;
	this->m_numberOfBalls = number;
	this->m_toShoot = 0;
}

Gameplay::~Gameplay() {}

void Gameplay::init() {
	m_balls[m_toShoot].body->setAcceleration({ 0, 0, 0 });
	m_balls[m_toShoot].body->setVelocity({ 0, 0, 0 });
	m_balls[m_toShoot].body->setPosition({ BALL_SPAWN[0], BALL_SPAWN[1], BALL_SPAWN[2]});
}

void Gameplay::update(double deltaTime) {
	m_time -= deltaTime / 0.003;
}

void Gameplay::draw(int shadow) {
}

void Gameplay::changeBallToShoot() {
	m_balls[m_toShoot].body->setAcceleration(cyclone::Vector3::GRAVITY);
	m_toShoot = m_toShoot + 1 >= m_numberOfBalls ? 0 : m_toShoot + 1;

	m_balls[m_toShoot].body->setAcceleration({ 0, 0, 0 });
	m_balls[m_toShoot].body->setVelocity({ 0, 0, 0 });
	m_balls[m_toShoot].body->setPosition({ BALL_SPAWN[0], BALL_SPAWN[1], BALL_SPAWN[2] });
}

int Gameplay::getGameplayScore() {
	return m_score;
}

int Gameplay::getGameplayTime() {
	return m_time;
}