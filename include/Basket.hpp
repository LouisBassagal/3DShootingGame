#pragma once

#include <Cyclone/GL/glut.h>
#include <gl/glu.h>
#include <GL/gl.h>
#include <Cyclone/cyclone.h>
#include <Cyclone/collide_fine.h>
#include <Box.hpp>

class Basket {
	public:
		Basket::Basket();
		Basket::~Basket();

		void draw(int shadow);
		void update(float duration);

		void startMove();

		void manageMovement(float duration);

		Box *getBoxes();
	private:
		Box m_boxes[10];
		Box m_panel;

		cyclone::Vector3 m_position{ .0f, 7.f, .0f };
		cyclone::Vector3 m_velocity{ 1.f, 0.f, .0f };

		bool m_isMoving{ true };
};