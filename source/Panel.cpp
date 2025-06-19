#include "Panel.hpp"

Panel::Panel() {
	body = new cyclone::RigidBody();
	body->setMass(1000000.f);
	halfSize = { 10, 7.5, 0.5 };
}

Panel::~Panel() {
	delete body;
	body = nullptr;
}

void Panel::draw(int shadow) {
	if (shadow == 1) {
		glColor3f(0.f, 0.f, 0.f);
	} else {
		glColor3f(.9f, .9f, .9f);
	}

	glPushMatrix();
	const cyclone::Vector3& pos = body->getPosition();
	glTranslatef(pos.x, pos.y, pos.z);

	glScalef(20.f, 15.f, 1.f);
	glutSolidCube(1.0f);
	glPopMatrix();
}

void Panel::update(float dt) {
	body->integrate(dt);
}

void Panel::setState(cyclone::Vector3 position, cyclone::Quaternion orientation, cyclone::Vector3 extents, cyclone::Vector3 velocity) {
	cyclone::Matrix3 tensor;
	cyclone::real mass = 1000000.f;

	body->setPosition(position);
	body->setOrientation(orientation);
	body->setVelocity(velocity);
	body->setRotation({ 0.f, 0.f, 0.f });
	body->setMass(mass);

	tensor.setBlockInertiaTensor(extents, mass);
	body->setInertiaTensor(tensor);

	body->setLinearDamping(1.f);
	body->setAngularDamping(1.f);
	body->clearAccumulators();
	body->setAcceleration({ 0.f, 0.f, 0.f });
	body->setAwake();
	body->setCanSleep(false);
	body->calculateDerivedData();

	offset = cyclone::Matrix4{};
	calculateInternals();
}