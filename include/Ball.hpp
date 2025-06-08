#pragma once

#include <Cyclone/GL/glut.h>
#include <gl/glu.h>
#include <GL/gl.h>
#include <Cyclone/cyclone.h>
#include <Cyclone/collide_fine.h>

class Ball : public cyclone::CollisionSphere {
	public:
		Ball();
		Ball(cyclone::Vector3, float radius);
		~Ball();

		void draw(int shadow);
		void stop();
		void update(float duration);

		void setState(cyclone::Vector3 position, cyclone::Quaternion orientation, cyclone::Vector3 extents, cyclone::Vector3 velocity);
		void setIsSelected(bool selected);
		void setColor(cyclone::Vector3 color);
		void setRadius(float radius);

		float getRadius();
		cyclone::RigidBody* getRigidBody();

	private:
		bool m_isSelected{ false };
		float m_radius{ 1.f };
		cyclone::Vector3 m_color{ 0.4, 0.0, 0.9 };
};