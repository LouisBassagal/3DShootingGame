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

		Box *getBoxes();
	private:
		Box m_boxes[10];

		cyclone::Vector3 m_position{ .0f, 10.f, .0f };
};