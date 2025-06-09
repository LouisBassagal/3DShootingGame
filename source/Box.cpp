#include <Box.hpp>
#include <cmath>

Box::Box() {
	body = new cyclone::RigidBody();
	body->setMass(1000000.f);
	halfSize = {0.5, 0.5, 1.7};
}

Box::~Box() {
	if (body) {
		delete body;
		body = nullptr;
	}
}

void Box::draw(int shadow) {
	if (shadow == 1) {
		glColor3f(0.f, 0.f, 0.f);
	} else {
		glColor3f(.8f, .2f, .2f);
	}


	glPushMatrix();
	const cyclone::Vector3& pos = body->getPosition();
	glTranslatef(pos.x, pos.y, pos.z);

	const cyclone::Quaternion& q = body->getOrientation();
	float rot[16] = {
		1 - 2 * (q.j * q.j + q.k * q.k), 2 * (q.i * q.j - q.k * q.r),     2 * (q.i * q.k + q.j * q.r),     0,
		2 * (q.i * q.j + q.k * q.r),     1 - 2 * (q.i * q.i + q.k * q.k), 2 * (q.j * q.k - q.i * q.r),     0,
		2 * (q.i * q.k - q.j * q.r),     2 * (q.j * q.k + q.i * q.r),     1 - 2 * (q.i * q.i + q.j * q.j), 0,
		0,                               0,                               0,                               1
	};
	glMultMatrixf(rot);

	glScalef(1.f, 1.f, 3.5f);
	glutSolidCube(1.0f);
	glPopMatrix();
}

void Box::update(float dt) {
	body->integrate(dt);
}

void Box::setState(cyclone::Vector3 position, cyclone::Quaternion orientation, cyclone::Vector3 extents, cyclone::Vector3 velocity)
{
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
	body->setCanSleep(true);
	body->calculateDerivedData();

	offset = cyclone::Matrix4{};
	calculateInternals();
}