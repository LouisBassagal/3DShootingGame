#include <SimpleScene.h>
#include <iostream>

SimpleScene::SimpleScene() {
	contacts = new cyclone::Contact[m_maxPossibleContact];
	cData = new cyclone::CollisionData();

	cData->contactArray = contacts;
	cData->contactCount = 0;
	resolver = new cyclone::ContactResolver(m_maxPossibleContact * 2, m_maxPossibleContact * 2, 0.001f, 0.001f);
	m_gameplay = std::make_unique<Gameplay>(this->getBalls(), 5);

	balls[0].setState({ 0, 6, 10 }, { 0, 0, 0, 1 }, { 2, 2, 2 }, { 0, -9.81f, 0 });
	balls[1].setState({ 0, 8, 10 }, { 0, 0, 0, 1 }, { 2, 2, 2 }, { 0, -9.81f, 0 });
	balls[2].setState({ 0, 10, 10 }, { 0, 0, 0, 1 }, { 2, 2, 2 }, { 0, -9.81f, 0 });
	balls[3].setState({ 0.1, 12, 10 }, { 0, 0, 0, 1 }, { 2, 2, 2 }, { 0, -9.81f, 0 });
	balls[4].setState({ -0.1, 14, 10 }, { 0, 0, 0, 1 }, { 2, 2, 2 }, { 0, -9.81f, 0 });
}

SimpleScene::~SimpleScene() {
	delete[] contacts;
	delete cData;
	delete resolver;
}

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

	auto panel = basket.getPanel();
	for (Ball* m = balls; m < balls + 5; m += 1) {
		if (!cData->hasMoreContacts())
			return;
		cyclone::CollisionDetector::boxAndSphere(*panel, *m, cData);
	}
}

void SimpleScene::update(float duration) {
	if (m_gameplay->isGameOver())
		return;
	generateContacts();
	resolver->resolveContacts(cData->contactArray, cData->contactCount, duration);

	basket.update(duration);
	for each(auto &m in balls) {
		m.update(duration);
		m.calculateInternals();
	}
	m_gameplay->update(duration, &basket);
}

void SimpleScene::draw(int shadow) {
	for each(auto & m in balls) {
		m.draw(shadow);
	}
	basket.draw(shadow);
	m_gameplay->draw(shadow);
}

void SimpleScene::initGameplay() {
	m_gameplay->init();
}

Ball* SimpleScene::getBalls()
{
	return balls;
}

int SimpleScene::getGameplayTime() {
	return m_gameplay->getGameplayTime();
}

int SimpleScene::getGameplayScore() {
	return m_gameplay->getGameplayScore();
}

void SimpleScene::setIsBallToShoot(bool isBallToShoot) {
	m_gameplay->setIsBallToShoot(isBallToShoot);
}

int SimpleScene::getBallToShoot() {
	return m_gameplay->getBallToShoot();
}