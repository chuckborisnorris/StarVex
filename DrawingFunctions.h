#define W_WIDTH 224
#define W_HEIGHT 192
#define SCALAR 2
#define OFFSET 2048

#define CLEAR 0
#define WHITE 1
#define CYAN 2
#define BLUE 3
#define MAGENTA 4
#define PURPLE 5
#define RED 6
#define ORANGE 7
#define YELLOW 8
#define GREEN_B 9


#include <GLFW/glfw3.h>
#include "HeliosDacFunctions.h"
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unistd.h>

int winW = W_WIDTH*SCALAR;
int winH = W_HEIGHT*SCALAR;

vector<vector<string>> content;
vector<string> row;
string line, word;
ifstream file ("halfSplash.csv", ios::in);

void DrawTitle() {
	if(file.is_open()) {
		while(getline(file, line))	{
			row.clear();
			stringstream str(line);
			while(getline(str, word, ','))
				row.push_back(word);
			content.push_back(row);
		}
	}
	else cout<<"Could not open the file\n";
	glBegin(GL_LINE_STRIP);
	for(int i=0;i<content.size();i++) {
		switch ( stoi(content[i][2]) ) {
			case WHITE: SetHeliosColour(255, 255, 255); break;
			case CYAN: SetHeliosColour(0, 255, 255); break;
			case BLUE: SetHeliosColour(0, 0, 255); break;
			case PURPLE: SetHeliosColour(128, 0, 255); break;
			case MAGENTA: SetHeliosColour(255, 0, 255); break;
			case RED: SetHeliosColour(255, 0, 0); break;
			case ORANGE: SetHeliosColour(255, 128, 0); break;
			case YELLOW: SetHeliosColour(255, 255, 0); break;
			case GREEN_B: SetHeliosColour(0, 255, 0); break;
			default: 
			glEnd();
			glBegin(GL_LINE_STRIP);
			break;
		}
		WriteHeliosPoint(stoi(content[i][0]), 20+W_HEIGHT-stoi(content[i][1]));
	}
	glEnd();
}

void DrawRect(int _dx, int _dy, int _h, int _w)
{
  glPushMatrix(); 
    HeliosTranslate(_dx, _dy, 0.0f);
	HeliosScale(1, 1, 0);
    glBegin(GL_LINE_STRIP); 
	  WriteHeliosBlank(0, 0);
      WriteHeliosPoint(0, 0); 
      WriteHeliosPoint(0, _h);
      WriteHeliosPoint(_w, _h);
      WriteHeliosPoint(_w, 0);  
	  WriteHeliosPoint(0, 0); 
      WriteHeliosBlank(0, 0); 	  
    glEnd();
  glPopMatrix();
}

void DrawDiamond(int _dx, int _dy, int _h, int _w)
{
  glPushMatrix(); 
    HeliosTranslate(_dx, _dy, 0.0f);
	HeliosScale(1, 1, 0);
    glBegin(GL_LINE_STRIP);
	  WriteHeliosBlank(-_w/2, 0); 	
      WriteHeliosPoint(-_w/2, 0);  
      WriteHeliosPoint(0, _h/2);
      WriteHeliosPoint(_w/2, 0);
      WriteHeliosPoint(0, -_h/2); 
	  WriteHeliosPoint(-_w/2, 0); 
	  WriteHeliosBlank(-_w/2, 0);	  
    glEnd();
  glPopMatrix();
}

void DrawBlaster(int _dx, int _dy, int scale)
{
  glPushMatrix(); 
    HeliosTranslate(_dx, _dy, 0.0f); 
	HeliosScale(scale/2, scale/2, 0);
    glBegin(GL_LINE_STRIP);
	    WriteHeliosBlank(-1, 0); 
		WriteHeliosPoint(-1, 0); 
		WriteHeliosPoint(0, 1);
		WriteHeliosPoint(1, 0);
		WriteHeliosPoint(0, -1); 
		WriteHeliosPoint(-1, 0);
	    WriteHeliosBlank(-1, 0);		
    glEnd();
  glPopMatrix();
}

void DrawSquare(int _dx, int _dy, int scale)
{
  glPushMatrix(); 
    HeliosTranslate(_dx, _dy, 0.0f);
	HeliosScale(scale/2, scale/2, 0);
    glBegin(GL_LINE_STRIP); 
		WriteHeliosBlank(1, 1); 
		WriteHeliosPoint(1, 1); 
		WriteHeliosPoint(-1, 1);
		WriteHeliosPoint(-1, -1);
		WriteHeliosPoint(1, -1);
		WriteHeliosPoint(1, 1);
		WriteHeliosBlank(1, 1);
    glEnd();
  glPopMatrix();
}


