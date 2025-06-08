#include <SimpleScene.h>
#include <iostream>

SimpleScene::SimpleScene() {
	contacts = new cyclone::Contact[m_maxPossibleContact];
	cData = new cyclone::CollisionData();

	cData->contactArray = contacts;
	cData->contactCount = 0;
	resolver = new cyclone::ContactResolver(m_maxPossibleContact * 2, m_maxPossibleContact * 2, 0.001f, 0.001f);
}

SimpleScene::~SimpleScene() {}

void SimpleScene::reset() {}

void SimpleScene::generateContacts() {
	cyclone::CollisionPlane p;

	cData->reset(m_maxPossibleContact);
	cData->friction = .1f;
	cData->restitution = .9f;
	cData->tolerance = .1f;
	p.direction = { 0.f, 1.f, 0.f };
	p.offset = 0;

	for (Ball* m = balls; m < balls + 5; m += 1) {
		if (!cData->hasMoreContacts())
			return;

		cyclone::CollisionDetector::sphereAndHalfSpace(*m, p, cData);
		for (Ball* o = m + 1; o < balls + 5; o += 1) {
			if (!cData->hasMoreContacts())
				return;
			cyclone::CollisionDetector::sphereAndSphere(*m, *o, cData);
		}
	}

	auto boxes = basket.getBoxes();
	for (Ball* m = balls; m < balls + 5; m += 1) {
		for (Box* b = boxes; b < boxes + 10; b += 1) {
			if (!cData->hasMoreContacts())
				return;
			cyclone::CollisionDetector::boxAndSphere(*b, *m, cData);
		}
	}
}

void SimpleScene::update(float duration) {
	generateContacts();
	resolver->resolveContacts(cData->contactArray, cData->contactCount, duration);

	basket.update(duration);
	for each(auto &m in balls) {
		m.update(duration);
		m.calculateInternals();
	}
}

void SimpleScene::draw(int shadow) {
	for each(auto & m in balls) {
		m.draw(shadow);
	}
	basket.draw(shadow);
}

Ball* SimpleScene::getBalls()
{
	return balls;
}
