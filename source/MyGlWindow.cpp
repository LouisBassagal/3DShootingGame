

#include "MyGlWindow.h"
#include <iostream>
#include "drawUtils.h"
#include "timing.h"
#include <cmath>
#include <Cyclone/plinks.h>

static double DEFAULT_VIEW_POINT[3] = { 0, 20, -30 };
static double DEFAULT_VIEW_CENTER[3] = { 0, 10, 0 };
static double DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };

MyGlWindow::MyGlWindow(int x, int y, int w, int h) :
	Fl_Gl_Window(x, y, w, h)
	//==========================================================================
{

	mode(FL_RGB | FL_ALPHA | FL_DOUBLE | FL_STENCIL);

	fieldOfView = 90;

	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float)h);
	m_viewer = new Viewer(viewPoint, viewCenter, upVector, fieldOfView, aspect);
	m_viewer->setMoveable(false);

	m_simpleScene = new SimpleScene();

	m_balls = m_simpleScene->getBalls();
	m_simpleScene->initGameplay();

	TimingData::init();
	run = 0;
	selected = -1;
}


void MyGlWindow::setupLight(float x, float y, float z)
{

	// set up the lighting
	GLfloat lightPosition[] = { 500, 900.0, 500, 1.0 };
	GLfloat lightPosition2[] = { 1, 0, 0, 0 };
	GLfloat lightPosition3[] = { 0, -1, 0, 0 };

	GLfloat violetLight[] = { 0.5f, 0.1f, .5f, 1.0 };
	GLfloat whiteLight[] = { 1, 1, 1, 1.0 };
	GLfloat whiteLight2[] = { .3, .3, .3, 1.0 };
	GLfloat blueLight[] = { .1f,.1f,.3f,1.0 };


	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_DEPTH);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);


	lightPosition[0] = x;
	lightPosition[1] = y;
	lightPosition[2] = z;

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, whiteLight2);
	// 
	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, whiteLight);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}




void setupObjects(void)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0x0, 0x0);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	glStencilMask(0x1);		// only deal with the 1st bit
}


void MyGlWindow::drawStuff()
{
	glColor4f(1, 1, 0, 0.5);
	polygonf(4, 20., 0., -25., 20., 0., 25., -20., 30., 25., -20., 30., -25.);
}

//==========================================================================
void MyGlWindow::draw()
//==========================================================================
{
	m_balls = m_simpleScene->getBalls();
	glViewport(0, 0, w(), h());

	// clear the window, be sure to clear the Z-Buffer too
	glClearColor(0.529f, 0.808f, .922f, 0);		// background should be blue

	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);



	// now draw the ground plane
	setProjection();
	setupFloor();

	glPushMatrix();
	drawFloor(200, 20);
	glPopMatrix();


	setupLight(m_viewer->getViewPoint().x, m_viewer->getViewPoint().y, m_viewer->getViewPoint().z);

	glLineWidth(1.0f);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	//draw shadow
	setupShadows();
	
	m_simpleScene->draw(1);

	unsetupShadows();

	glPushMatrix();

	m_simpleScene->draw(0);

	glPopMatrix();

	glDisable(GL_BLEND);

	int time = m_simpleScene->getGameplayTime();
	int score = m_simpleScene->getGameplayScore();

	int minutes = (time / 1000) / 60;
	int seconds = (time / 1000) % 60;

	std::string timeLeftString = std::to_string(minutes) + ":" + std::to_string(seconds);
	putText(("Time Left : " + timeLeftString).data(), 10, 10, 1, 0, 1);
	putText(("Score : " + std::to_string(score)).data(), 10, 40, 1, 0, 1);
	putText("3DShootingGame", 10, 70, 1, 0, 1);

	glViewport(0, 0, w(), h());
	setProjection();
	glEnable(GL_COLOR_MATERIAL);
}

void MyGlWindow::test() {
	cyclone::Random r;
	cyclone::Vector3 v = r.randomVector({2, 20, 2}, {-2, 20, -2});
}

void MyGlWindow::update()
{

	TimingData::get().update();

	if (!run) 
		return;

	float duration = (float)TimingData::get().lastFrameDuration * 0.003;

	if (duration <= 0.0f)
		return;

	m_simpleScene->update(duration);
}


void MyGlWindow::doPick()
{
	make_current();		// since we'll need to do some GL stuff

	int mx = Fl::event_x(); // where is the mouse?
	int my = Fl::event_y();

	// get the viewport - most reliable way to turn mouse coords into GL coords
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	// set up the pick matrix on the stack - remember, FlTk is
	// upside down!
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPickMatrix((double)mx, (double)(viewport[3] - my), 5, 5, viewport);

	// now set up the projection
	setProjection(false);

	// now draw the objects - but really only see what we hit
	GLuint buf[100];
	glSelectBuffer(100, buf);
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);

	// draw the cubes, loading the names as we go
	for (size_t i = 0; i < 5; ++i) {
		glLoadName((GLuint)(i + 1));
		m_balls[i].draw(0);
	}


	// go back to drawing mode, and see how picking did
	int hits = glRenderMode(GL_RENDER);
	if (hits) {
		// warning; this just grabs the first object hit - if there
		// are multiple objects, you really want to pick the closest
		// one - see the OpenGL manual 
		// remember: we load names that are one more than the index
		selected = buf[3] - 1;
	} else {
		// nothing hit, nothing selected
		selected = -1;
	}
	//printf("Selected Cube %d\n", selectedCube);
}



void MyGlWindow::setProjection(int clearProjection)
//==========================================================================
{
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, w(), h());
	if (clearProjection)
		glLoadIdentity();
	// compute the aspect ratio so we don't distort things
	double aspect = ((double)w()) / ((double)h());
	gluPerspective(fieldOfView, aspect, 1, 1000);

	// put the camera where we want it to be
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// use the transformation in the ArcBall

	gluLookAt(
		m_viewer->getViewPoint().x, m_viewer->getViewPoint().y, m_viewer->getViewPoint().z,
		m_viewer->getViewCenter().x, m_viewer->getViewCenter().y, m_viewer->getViewCenter().z,
		m_viewer->getUpVector().x, m_viewer->getUpVector().y, m_viewer->getUpVector().z
	);

//	glDisable(GL_BLEND);
}

static int last_push;
int m_pressedMouseButton;
int m_lastMouseX;
int m_lastMouseY;

int MyGlWindow::handle(int e)
//==========================================================================
{

	switch (e) {
	case FL_SHOW:		// you must handle this, or not be seen!
		show();
		return 1;
	case FL_PUSH:
	{
		m_pressedMouseButton = Fl::event_button();
		m_lastMouseX = Fl::event_x();
		m_lastMouseY = Fl::event_y();

		if (m_pressedMouseButton == 1) {
			doPick();

			std::cout << "Selected ball: " << selected << std::endl;
			std::cout << "Current ball to shoot: " << m_simpleScene->getBallToShoot() << std::endl;

			if (selected != -1 && m_simpleScene->getBallToShoot() == selected) {
				m_positionStartDrag = m_balls[selected].body->getPosition();
				m_timeStartDrag = clock();
				m_balls[selected].setIsSelected(true);
			}
			damage(1);
			return 1;
		};
		break;


	}
	damage(1);
	return 1;
	case FL_RELEASE:
		m_pressedMouseButton = -1;
		if (selected != -1 && m_simpleScene->getBallToShoot() == selected) {
			auto *body = m_balls[selected].body;
			cyclone::Vector3 origin{ 0, 15, -25 };
			cyclone::Vector3 direction = (body->getPosition() - origin);
			float zPower;

			m_positionEndDrag = body->getPosition();
			m_timeEndDrag = clock();
			zPower = (50 * (static_cast<float>(m_timeEndDrag) - static_cast<float>(m_timeStartDrag)) / CLOCKS_PER_SEC);
			direction.z = zPower;
			direction.x *= 20.f;
			direction.y *= 20.f;
			m_run->value(1);
			m_balls[selected].setIsSelected(false);
			body->addForce(direction * body->getMass() * 5.f);
			m_simpleScene->setIsBallToShoot(false);
			selected = -1;
			run = 1;
		}
		damage(1);
		return 1;
	case FL_DRAG: // if the user drags the mouse
	{
		if (selected >= 0 && m_pressedMouseButton == 1 && m_simpleScene->getBallToShoot() == selected) {
			double r1x, r1y, r1z, r2x, r2y, r2z;
			getMouseLine(r1x, r1y, r1z, r2x, r2y, r2z);
			double rx, ry, rz;
			Ball& selectedBall = m_balls[selected];
			mousePoleGo(r1x, r1y, r1z, r2x, r2y, r2z,
				static_cast<double>(selectedBall.body->getPosition().x),
				static_cast<double>(selectedBall.body->getPosition().y),
				static_cast<double>(selectedBall.body->getPosition().z),
			rx, ry, rz, (Fl::event_state() & FL_CTRL) != 0);

			cyclone::Vector3 v(rx, ry, -25);
			
			m_balls[selected].body->setPosition(v);
			damage(1);
		} else {
			double fractionChangeX = static_cast<double>(Fl::event_x() - m_lastMouseX) / static_cast<double>(this->w());
			double fractionChangeY = static_cast<double>(m_lastMouseY - Fl::event_y()) / static_cast<double>(this->h());

			if (m_pressedMouseButton == 1) {
				m_viewer->rotate(fractionChangeX, fractionChangeY);
			}
			else if (m_pressedMouseButton == 2) {
				m_viewer->zoom(fractionChangeY);
			}
			else if (m_pressedMouseButton == 3) {
				m_viewer->translate(-fractionChangeX, -fractionChangeY, (Fl::event_key(FL_Shift_L) == 0) || (Fl::event_key(FL_Shift_R) == 0));
			}
			else {
				std::cout << "Warning: dragging with unknown mouse button!  Nothing will be done" << std::endl;
			}

			m_lastMouseX = Fl::event_x();
			m_lastMouseY = Fl::event_y();
			redraw();
		}
	}

	return 1;

	case FL_KEYBOARD:
		return 0;

	default:
		return 0;
	}
}



// get the mouse in NDC
//==========================================================================
void MyGlWindow::getMouseNDC(float& x, float& y)
//==========================================================================
{
	// notice, we put everything into doubles so we can do the math
	float mx = (float)Fl::event_x();	// remeber where the mouse went down
	float my = (float)Fl::event_y();

	// we will assume that the viewport is the same as the window size
	float wd = (float)w();
	float hd = (float)h();

	// remember that FlTk has Y going the wrong way!
	my = hd - my;

	x = (mx / wd) * 2.0f - 1.f;
	y = (my / hd) * 2.0f - 1.f;
}

void drawStrokeText(char* string, int x, int y, int z)
{
	char* c;
	glPushMatrix();
	glTranslated(x, y + 8, z);
	glScaled(0.2, 0.2, 0.2);
	for (c = string; *c != '\0'; c++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}
	glPopMatrix();
}



void MyGlWindow::putText(char* string, int x, int y, float r, float g, float b)
{
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	ortho();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glColor3f(r, g, b);
	drawStrokeText(string, x, y, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void MyGlWindow::step() {
	TimingData::get().update();
	float duration = 0.03f;

	std::cout << "Step" << std::endl;
}

void MyGlWindow::setRun(Fl_Light_Button *run)
{
	m_run = run;
}

void MyGlWindow::testValue(float value) {}

void MyGlWindow::resetScene() {
	delete m_simpleScene;
	m_simpleScene = new SimpleScene();
}

