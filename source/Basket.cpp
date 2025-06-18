#define _USE_MATH_DEFINES

#include <Basket.hpp>
#include <cmath>
#include <algorithm>
#include <iostream>

constexpr float LEFT_BOUND = -10.0f;
constexpr float RIGHT_BOUND = 10.0f;


Basket::Basket() {
    // Remplace la boucle for dans le constructeur de Basket par ceci :
    const float R = 5.0f;   // Rayon du cercle
    const int n = 10;       // Nombre de cubes

    for (int i = 0; i < n; i++) {
        // Calcul de l'angle pour chaque cube
        float theta = (2.0f * M_PI * i) / n;
        float x = m_position.x + R * cos(theta);
        float y = m_position.y;
        float z = m_position.z + R * sin(theta);

        // Positionne chaque cube sur le cercle
        m_boxes[i].setState({ x, y, z }, cyclone::Quaternion{}, { 1, 1, 1 }, { 0, 0, 0 });

        // Calcul de la normale (perpendiculaire à la tangente, donc radiale)
        cyclone::Vector3 normal(cos(theta), 0, sin(theta));
        normal.normalise();
        // Angle entre la normale et l'axe X
        float angle = atan2(normal.z, normal.x);
        m_boxes[i].body->setOrientation(cyclone::Quaternion::fromAxisAngle({0,1,0}, angle));
    }
}

Basket::~Basket() {}

void Basket::draw(int shadow) {
	for each(auto &b in m_boxes) {
		b.draw(shadow);
	}
}

Box *Basket::getBoxes() {
	return m_boxes;
}

void Basket::update(float duration) {
	for each(auto &b in m_boxes) {
		b.update(duration);
		b.calculateInternals();
	}
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
}
