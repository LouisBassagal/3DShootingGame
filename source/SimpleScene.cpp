#include <SimpleScene.h>

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
	cData->restitution = .6f;
	cData->tolerance = .1f;
	p.direction = { 0.f, 1.f, 0.f };
	p.offset = 0;

	for (Mover* m = movers; m < movers + 5; m += 1) {
		if (!cData->hasMoreContacts())
			return;

		cyclone::CollisionDetector::boxAndHalfSpace(*m, p, cData);
		for (Mover* o = m + 1; o < movers + 5; o += 1) {
			if (!cData->hasMoreContacts())
				return;
			cyclone::CollisionDetector::boxAndBox(*m, *o, cData);
		}
	}
}

void SimpleScene::update(float duration) {
	generateContacts();
	resolver->resolveContacts(cData->contactArray, cData->contactCount, duration);

	for each(auto &m in movers) {
		m.update(duration);
		m.calculateInternals();
	}
}

void SimpleScene::draw(int shadow) {
	for each(auto & m in movers) {
		m.draw(shadow);
	}
}
