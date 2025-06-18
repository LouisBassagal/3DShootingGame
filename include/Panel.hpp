#pragma once

#include <Cyclone/GL/glut.h>
#include <gl/glu.h>
#include <GL/gl.h>
#include <Cyclone/cyclone.h>
#include <Cyclone/collide_fine.h>

class Panel : public cyclone::CollisionBox
{
	public:
		Panel();
		~Panel();

		void draw(int shadow);
		void update(float duration);
		void setState(cyclone::Vector3 position, cyclone::Quaternion orientation, cyclone::Vector3 extents, cyclone::Vector3 velocity);
};