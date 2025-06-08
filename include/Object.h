#pragma once

#include <Cyclone/GL/glut.h>
#include <gl/glu.h>
#include <GL/gl.h>
#include <Cyclone/cyclone.h>
#include <Cyclone/collide_fine.h>

class Mover : public cyclone::CollisionBox {
	public:
		Mover();
		Mover(cyclone::Vector3, cyclone::Vector3 size);
		~Mover() {};

		void draw(int shadow);
		void stop();
		void update(float duration);
		void checkEdges();

		void setState(cyclone::Vector3 position, cyclone::Quaternion orientation, cyclone::Vector3 extents, cyclone::Vector3 velocity);
		void setIsSelected(bool selected);
		void setColor(cyclone::Vector3 color);
		void Mover::setSize(const cyclone::Vector3& size);

		cyclone::Vector3 &getSize();
		cyclone::RigidBody* getRigidBody();

	private:
		std::vector<cyclone::ParticleForceGenerator *> m_springForces;

		bool m_isSelected{ false };
		cyclone::Vector3 m_size{ 1.f, 1.f, 1.f };

		cyclone::Vector3 m_color{ 0.4, 0.0, 0.9 };
};