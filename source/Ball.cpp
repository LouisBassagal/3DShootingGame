#include <Ball.hpp>
#include <Cyclone/cyclone.h>
#include <iostream>

Ball::Ball() {
	cyclone::Random r;

	m_radius = r.randomReal(0.5f, 1.5f);
	body = new cyclone::RigidBody();
	radius = m_radius;
}

Ball::Ball(cyclone::Vector3 position, float radius) : m_radius{ radius } {
	body = new cyclone::RigidBody();
	radius = m_radius;
}

Ball::~Ball() {
	if (body) {
		delete body;
		body = nullptr;
	}
}

void Ball::draw(int shadow) {
	cyclone::Vector3 pos{};
	GLfloat mat[16];

	pos = body->getPosition();
	body->getGLTransform(mat);

	if (shadow == 1) {
		glColor4f(0.1, 0.1, 0.1, 0.5);
	} else {
		glColor4f(m_color.x, m_color.y, m_color.z, 1.0);
	}

	glPushMatrix();

	glMultMatrixf(mat);
	glutSolidSphere(m_radius, 20, 20);
	glPopMatrix();
}

void Ball::update(float duration) {
	if (!body)
		return;

	if (body->getAwake()) {
		m_color = { 0.4, 0.0, 0.9 };
	} else {
		m_color = { 0.9, 0.4, 0.0 };
	}

	body->integrate(duration);
	if (m_isSelected)
		body->setVelocity({0, 0, 0 });
}

void Ball::stop() {

}

void Ball::setState(cyclone::Vector3 position, cyclone::Quaternion orientation, cyclone::Vector3 extents, cyclone::Vector3 velocity)
{
	cyclone::Matrix3 tensor;
	cyclone::real mass = m_radius * 2.f;

	body->setPosition(position);
	body->setOrientation(orientation);
	body->setVelocity(velocity);
	body->setRotation({ 0.f, 0.f, 0.f });
	body->setMass(mass);

	tensor.setBlockInertiaTensor(extents, mass);
	body->setInertiaTensor(tensor);

	body->setLinearDamping(0.9f);
	body->setAngularDamping(0.8f);
	body->clearAccumulators();
	body->setAcceleration({ 0.f, -9.81f, 0.f });
	body->setAwake();
	body->setCanSleep(true);
	body->calculateDerivedData();

	offset = cyclone::Matrix4{};
	calculateInternals();
}

void Ball::setIsSelected(bool selected)
{
	if (selected) {
		body->setAwake();
		body->setCanSleep(false);
		body->setAcceleration({ 0.f, 0.f, 0.f });
	} else {
		body->setCanSleep(true);
		body->setAcceleration({ 0.f, -9.81f, 0.f });
	}
	m_isSelected = selected;
}

float Ball::getRadius()
{
	return m_radius;
}

cyclone::RigidBody* Ball::getRigidBody()
{
	return body;
}

void Ball::setColor(cyclone::Vector3 color)
{
	m_color = color;
}

void Ball::setRadius(float radius)
{
	m_radius = radius;
	radius = radius;
}

