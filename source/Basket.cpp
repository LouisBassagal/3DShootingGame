#define _USE_MATH_DEFINES

#include <Basket.hpp>
#include <cmath>
#include <algorithm>
#include <iostream>

constexpr float LEFT_BOUND = -15.0f;
constexpr float RIGHT_BOUND = 15.0f;


Basket::Basket() {
    const float R = 5.0f;
    const int n = 10;

    for (int i = 0; i < n; i++) {
        float theta = (2.0f * M_PI * i) / n;
        float x = m_position.x + R * cos(theta);
        float y = m_position.y;
        float z = m_position.z + R * sin(theta);

        m_boxes[i].setState({ x, y, z }, cyclone::Quaternion{}, { 1, 1, 1 }, { 0, 0, 0 });
        cyclone::Vector3 normal(cos(theta), 0, sin(theta));
        normal.normalise();
        float angle = atan2(normal.z, normal.x);
        m_boxes[i].body->setOrientation(cyclone::Quaternion::fromAxisAngle({0,1,0}, angle));
    }

	m_panel.setState({ 0, 12, 6 }, cyclone::Quaternion{}, { 1, 1, 1 }, { 0, 0, 0 });
}

Basket::~Basket() {}

void Basket::draw(int shadow) {
	for each(auto &b in m_boxes) {
		b.draw(shadow);
	}
	m_panel.draw(shadow);
}

Box *Basket::getBoxes() {
	return m_boxes;
}

Panel* Basket::getPanel() {
	return &m_panel;
}

void Basket::update(float duration) {
	for each(auto &b in m_boxes) {
		b.update(duration);
		b.calculateInternals();
	}
	m_panel.update(duration);
	manageMovement(duration);
}

void Basket::startMove() {
	m_isMoving = true;
}

void Basket::manageMovement(float duration) {
    if (!m_isMoving)
        return;

	m_position.x += m_velocity.x * duration;

    if (m_position.x < LEFT_BOUND || m_position.x > RIGHT_BOUND)
        m_velocity.x = -m_velocity.x;

    for (auto& box : m_boxes)
        box.body->setVelocity(m_velocity);
	m_panel.body->setVelocity(m_velocity);
}

void Basket::setVelocity(cyclone::Vector3 velocity) {
	m_velocity = velocity;
}

cyclone::Vector3& Basket::getVelocity() {
	return m_velocity;
}

cyclone::Vector3& Basket::getPosition() {
	return m_position;
}
