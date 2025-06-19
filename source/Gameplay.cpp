#include <Gameplay.hpp>
#include <iostream>
#include <string>

Gameplay::Gameplay(Ball* balls, int number) {
	m_balls = balls;
	m_numberOfBalls = number;
	m_toShoot = 0;
	m_score = 0;
}

Gameplay::~Gameplay() {}

void Gameplay::init() {
	m_isBallToShoot = true;
	m_balls[m_toShoot].body->setAcceleration({ 0, 0, 0 });
	m_balls[m_toShoot].body->setVelocity({ 0, 0, 0 });
	m_balls[m_toShoot].body->setPosition({ BALL_SPAWN[0], BALL_SPAWN[1], BALL_SPAWN[2]});
}

void Gameplay::update(double deltaTime, Basket *b) {
	m_time -= deltaTime / 0.003;

	if (m_isBallToShoot == false)
		m_changeBallTimer += deltaTime / 0.003;

	if (m_changeBallTimer > 3000 && m_isBallToShoot == false) {
		changeBallToShoot();
		m_changeBallTimer = 0;
	}

	if (m_time < 60000) {
		b->startMove();
	}

	if (m_time < 0) {
		m_time = 0;
		m_isGameOver = true;
	}
	checkBallGoal(b);
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

void Gameplay::checkBallGoal(Basket *b) {
	cyclone::Vector3 basketPosition = b->getPosition();

	for (size_t i = 0; i < m_numberOfBalls; i += 1) {
		cyclone::Vector3 pos = m_balls[i].body->getPosition();
		float x0 = basketPosition.x;
		float y0 = basketPosition.y;
		float z0 = basketPosition.z;
		float r = 5.0f;

		if (pos.y < y0) {
			float dx = pos.x - x0;
			float dz = pos.z - z0;
			float dist2 = dx * dx + dz * dz;

			if (dist2 < (r * r) && m_lastBallGoal != i) {
				m_score += 100;
				m_lastBallGoal = i;
			}
		}
	}
}

int Gameplay::getBallToShoot() {
	return m_toShoot;
}

bool Gameplay::isGameOver() const {
	return m_isGameOver;
}

int Gameplay::getGameplayScore() {
	return m_score;
}

int Gameplay::getGameplayTime() {
	return m_time;
}