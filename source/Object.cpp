#include <Object.h>
#include <Cyclone/cyclone.h>
#include <iostream>

Mover::Mover() {
	cyclone::Random r;

	m_size = { 1.f, 1.f, 1.f };
	body = new cyclone::RigidBody();
	halfSize = { 1.f, 1.f, 1.f };
}

Mover::Mover(cyclone::Vector3 position, cyclone::Vector3 size) : m_size{size} {
	body = new cyclone::RigidBody();
	halfSize = m_size / 2;
}

void Mover::draw(int shadow) {
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
	glScalef(m_size.x, m_size.y, m_size.z);
	glutSolidCube(1.f);

	if (shadow == 1) {
		glPopMatrix();
		return;
	}

	glScalef(1.f, 1.f, 1.f);
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glColor3f(0, 1, 0);
	glVertex3f(mat[3], mat[7], mat[11]);
	glVertex3f(mat[3] + 10, mat[7], mat[11]);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(mat[3], mat[7], mat[11]);
	glVertex3f(mat[3], mat[7] + 10, mat[11]);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(mat[3], mat[7], mat[11]);
	glVertex3f(mat[3], mat[7], mat[11] + 10);
	glEnd();
	glPopMatrix();

}

void Mover::update(float duration) {
	if (!body)
		return;

	if (body->getAwake()) {
		m_color = { 0.4, 0.0, 0.9 };
	} else {
		m_color = { 0.9, 0.4, 0.0 };
	}
	body->integrate(duration);
}

void Mover::stop() {

}

void Mover::checkEdges() {
	cyclone::Vector3 position{};
	cyclone::Vector3 velocity{};
}

void Mover::setState(cyclone::Vector3 position, cyclone::Quaternion orientation, cyclone::Vector3 extents, cyclone::Vector3 velocity)
{
	cyclone::Matrix3 tensor;
	cyclone::real mass = m_size.x * 2.f;

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

void Mover::setIsSelected(bool selected)
{
	m_isSelected = selected;
}

cyclone::Vector3 &Mover::getSize()
{
	return m_size;
}

cyclone::RigidBody* Mover::getRigidBody()
{
	return body;
}

void Mover::setColor(cyclone::Vector3 color)
{
	m_color = color;
}

void Mover::setSize(const cyclone::Vector3& size)
{
	m_size = size;
	halfSize = size / 2;
}

