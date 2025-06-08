#define NOMINMAX

#include <FL/Fl_Gl_Window.h>
#include <Fl/Fl.h>
#include <Fl/Fl_Value_Slider.H>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <Cyclone/GL/glut.h>
#include <Fl/Fl_Double_Window.h>

#include "stdio.h"
#include "math.h"
#include "3DUtils.h"
#include "Viewer.h"
#include <vector>
#include <Cyclone/core.h>
#include <Object.h>
#include <Fl/Fl_Light_Button.h>

#include <Cyclone/pfgen.h>
#include <SimpleScene.h>

class MyGlWindow : public Fl_Gl_Window {
public:
	MyGlWindow(int x, int y, int w, int h);
	Fl_Slider* time;
	int run;
	void update();
	void drawStuff();
	void doPick();
	void test();
	void putText(char* string, int x, int y, float r, float g, float b);
	void step();
	void setRun(Fl_Light_Button *run);
	void testValue(float value);

	int selected;

private:
	void draw();					// standard 
	int handle(int);				// standard FlTk
	void setProjection(int clearProjection = 1);
	void getMouseNDC(float& x, float& y);
	void setupLight(float x, float y, float z);

	float fieldOfView;
	unsigned int m_maxPossibleContact{20};
	
	Viewer *m_viewer;
	
	Fl_Light_Button *m_run;

	SimpleScene* m_simpleScene;
	Ball* m_balls;

	cyclone::Vector3 m_positionStartDrag;
	cyclone::Vector3 m_positionEndDrag;

	cyclone::Vector3 m_lastDragPosition;
	clock_t m_lastDragTime;

	clock_t m_timeStartDrag;
	clock_t m_timeEndDrag;
};

