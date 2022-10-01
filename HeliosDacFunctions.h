#define MAX_POINTS 2000
#define PI 3.14159

#include "HeliosDac.h"
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <math.h>

HeliosPoint frame[MAX_POINTS];
int idx = 0;
int r, g, b = 0;
int xoff, yoff = 0;
int xs, ys = 0;
int x, y = 0;

HeliosDac helios;
int numDevs = helios.OpenDevices();

void HeliosTranslate(int _xoff, int _yoff, int _zoff) {
	xoff = _xoff;
	yoff = _yoff;
	glTranslatef(float(_xoff*SCALAR), float(_yoff*SCALAR), float(_zoff*SCALAR));
}

void HeliosScale(int _xs, int _ys, int _zs) {
	xs = _xs;
	ys = _ys;
	glScalef(float(_xs), float(_ys), float(_zs));
}

void SetHeliosColour(int _r, int _g, int _b) {
	r = _r;
	g = _g;
	b = _b;
	
	glColor3f(float(_r/255), float(_g/255), float(_b/255));
}

void WriteHeliosData(int _x, int _y) {
	
	x = (_x*xs + xoff)*SCALAR+OFFSET;
	y = (_y*ys + yoff)*SCALAR+OFFSET;
	
	if (frame[idx-1].x != x && frame[idx-1].y != y) {
		frame[idx].x = x;
		frame[idx].y = y;			
		frame[idx].r = r;
		frame[idx].g = g;
		frame[idx].b = b;
		frame[idx].i = 0xff;
	
		glVertex2f(_x*SCALAR, _y*SCALAR);
	
		if (idx < MAX_POINTS) idx++;
	}	
}

void WriteHeliosPoint(int _x, int _y) {
	
	x = (_x*xs + xoff)*SCALAR+OFFSET;
	y = (_y*ys + yoff)*SCALAR+OFFSET;
	
	frame[idx].x = x;
	frame[idx].y = y;			
	frame[idx].r = r;
	frame[idx].g = g;
	frame[idx].b = b;
	frame[idx].i = 0xff;
	
	glVertex2f(_x*SCALAR, _y*SCALAR);
	
	if (idx < MAX_POINTS) idx++;
}

void WriteHeliosBlank(int _x, int _y) {
		frame[idx].x = x;
		frame[idx].y = y;			
		frame[idx].r = 0x00;
		frame[idx].g = 0x00;
		frame[idx].b = 0x00;
		frame[idx].i = 0x00;
		
		if (idx < MAX_POINTS) idx++;
		
		frame[idx].x = (_x*xs + xoff)*SCALAR+OFFSET;
		frame[idx].y = (_y*ys + yoff)*SCALAR+OFFSET;			
		frame[idx].r = 0x00;
		frame[idx].g = 0x00;
		frame[idx].b = 0x00;
		frame[idx].i = 0x00;
		
		if (idx < MAX_POINTS) idx++;
	
}

void WriteHeliosFrame() {
	for (int j = 0; j < numDevs; j++) {
		if (helios.GetStatus(j) == 1) {
			helios.WriteFrame(j, 15*idx, HELIOS_FLAGS_DEFAULT, &frame[0], idx);
		}
	}	
	std::cout << idx << std::endl;	
	idx = 0;
}

void CloseHeliosDAC() {
	helios.CloseDevices();
}