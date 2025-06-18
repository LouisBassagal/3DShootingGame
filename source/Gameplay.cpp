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
	m_isBallToShoot = true;
	m_balls[m_toShoot].body->setAcceleration({ 0, 0, 0 });
	m_balls[m_toShoot].body->setVelocity({ 0, 0, 0 });
	m_balls[m_toShoot].body->setPosition({ BALL_SPAWN[0], BALL_SPAWN[1], BALL_SPAWN[2]});
}

void Gameplay::update(double deltaTime) {
	m_time -= deltaTime / 0.003;

	if (m_isBallToShoot == false)
		m_changeBallTimer += deltaTime / 0.003;

	if (m_changeBallTimer > 3000 && m_isBallToShoot == false) {
		changeBallToShoot();
		m_changeBallTimer = 0;
	}
}

void Gameplay::setIsBallToShoot(bool b) {
	m_isBallToShoot = b;
}

void Gameplay::draw(int shadow) {
}

void Gameplay::changeBallToShoot() {
	m_balls[m_toShoot].body->setAcceleration(cyclone::Vector3::GRAVITY);
	m_toShoot = m_toShoot + 1 >= m_numberOfBalls ? 0 : m_toShoot + 1;

	m_balls[m_toShoot].body->setAwake(true);
	m_balls[m_toShoot].body->setAcceleration({ 0, 0, 0 });
	m_balls[m_toShoot].body->setVelocity({ 0, 0, 0 });
	m_balls[m_toShoot].body->setPosition({ BALL_SPAWN[0], BALL_SPAWN[1], BALL_SPAWN[2] });
	m_isBallToShoot = true;
}

int Gameplay::getBallToShoot() {
	return m_toShoot;
}

int Gameplay::getGameplayScore() {
	return m_score;
}

int Gameplay::getGameplayTime() {
	return m_time;
}