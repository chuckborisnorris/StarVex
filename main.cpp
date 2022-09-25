#include "GetProcessInfo.h"
#include "Window.h"
#include "DrawingFunctions.h"

int main(void)
{
	GLFWwindow* window = InitWindow(window, winH, winW);
	
	//-------------------------------------------------
	//Get Process ID
    int procId = getProcIdByName("retroarch");
    printf("retroarch:  %d \n", procId);
	pid_t pid = procId;
	
	//Get Heap Address
	size_t cols = 224;
	size_t rows = 150;
	size_t buffer = 32768;
	unsigned char* frameData = new unsigned char[buffer];
	
	long heapAddr = GetHeapAddress(procId);
	
	long frameAddr = heapAddr+0x220000; //needs soft-coding!
	struct iovec frameL = CreateReturnBuffer(buffer);
	struct iovec frameR = CreateRemoteBuffer(frameAddr, buffer);
	frameData  = ReadFullBuffer(pid, &frameL, &frameR);
	
	//Get Miscellaneous Address - These need adding to the frame data
	long miscAddr = GetMiscAddress(procId);
	
	long hudAddr = miscAddr+0x801f47;
	void *hudPtr = (void *)hudAddr;
    struct iovec hudL = CreateReturnBuffer(1);
    struct iovec hudR = CreateRemoteBuffer(hudAddr, 1);
	
	long hitAddr = miscAddr+0x95c;
	void *hitPtr = (void *)hitAddr;
    struct iovec hitL = CreateReturnBuffer(1);
    struct iovec hitR = CreateRemoteBuffer(hitAddr, 1);
	
 	long scrAddr = miscAddr+0x80311b;
	void *scrPtr = (void *)scrAddr;
    struct iovec scrL = CreateReturnBuffer(1);
    struct iovec scrR = CreateRemoteBuffer(scrAddr, 1); 
	
	//-------------Setup variables---------------
	int count, total = 0;
	int bCol = 255;
	int bombsVal, shieldVal, boostVal, bossmaxhpVal, bosshpVal  = 0;
	std::list<std::vector<int>> allPoints{{0, 0, 0, 0}};
	std::vector<int> p{0, 0, 0, 0};
	allPoints.push_back(p);
	bool next_blank = false;
	bool first = true;
	
    //----------------Main Loop-------------------
    while (!glfwWindowShouldClose(window)) {	
		glClear(GL_COLOR_BUFFER_BIT);
		allPoints.clear();
		allPoints.push_back(p);
		
		int scrVal  = ReadBuffer(pid, &scrL, &scrR);
		int hudVal  = ReadBuffer(pid, &hudL, &hudR);
		int hitVal  = ReadBuffer(pid, &hitL, &hitR);	
		if (hitVal == 32) { bCol = 0; }
		else { bCol = 255; }
		
		//-----------------Start Screen------------------
		if (scrVal == 32) {
			DrawTitle();
		}
		else if (hudVal < 255) {
			//-----------HUD Display-----------------
			SetHeliosColour(255,0,0);
			if ( bosshpVal > 0 ) DrawRect(210,14,5,-bosshpVal);		
			SetHeliosColour(255,0,255);
			for (int i = 0; i < bombsVal; i++) {
				DrawDiamond(210-10*i,162, 8, 8); 	//bombs
			} 			
			SetHeliosColour(255,255,255);
			DrawRect(168,168,9,44);		
			SetHeliosColour(0,255,255); 
			DrawRect(170,170,5,boostVal); //boost			
			SetHeliosColour(255,255,255);
			DrawRect(12,168,9,44);			
			SetHeliosColour(255,0,0);
			DrawRect(14,170,5,shieldVal); //shield
		}
		
		//-------------Frame Info---------------
		frameData  = ReadFullBuffer(pid, &frameL, &frameR);
		glLineWidth(2.0f);			
		count = 0;
		int v_cnt = 0;
		
		SetHeliosColour(0, 255, 0);
		r, b = 0;
		glBegin(GL_LINES);
		for (int i = 1; i < buffer-2; i += 2) {
			if ( count > total && total > 0 ) { break; }		
			else if (frameData[i] == 0xbe && frameData[i-1] == 0xef) { //start
				i += 2;	
				total = (frameData[i] << 8) + frameData[i-1];	
				i += 2;	
				shieldVal = frameData[i-1];		
				i += 2;	
				boostVal = frameData[i-1];	
				i += 2;	
				bombsVal = frameData[i-1];	
				i += 2;	
				bosshpVal = frameData[i-1];	
				i += 2;	
				bossmaxhpVal = frameData[i-1];
				count += 14;
			}
			else if (frameData[i] == 0xff && frameData[i-1] == 0xfe) { //next					
				next_blank = true;
				
				glEnd();
				glBegin(GL_LINES);
				
				count += 2;	
			}		
			else if (frameData[i] == 0xfa && frameData[i-1] == 0xce) { //sprites
				i += 8;
				glEnd();
				if (frameData[i] == 0 && frameData[i-2] == 0) {
					if (frameData[i-7] == 0x40 && frameData[i-6] == 0xe0) {
						SetHeliosColour(255,0,255);
						DrawBlaster(frameData[i-1], frameData[i], frameData[i-5]);
					} 
					else if (frameData[i-7] == 0x40 && frameData[i-6] == 0xc0) {
						SetHeliosColour(0,255,255);
						DrawBlaster(frameData[i-3], frameData[i-1], frameData[i-5]);
					}	
					else if (frameData[i-5] == 0x20 && frameData[i-4] == 0xc0 ||
							 frameData[i-5] == 0xc0 && frameData[i-4] == 0x80 ||
							 frameData[i-5] == 0xc0 && frameData[i-4] == 0xe0 ||
							 frameData[i-5] == 0xa0 && frameData[i-4] == 0xe0 ||
							 frameData[i-5] == 0xe0 && frameData[i-4] == 0x80 ) {
						SetHeliosColour(255,0,0);
						DrawBlaster(frameData[i-3], frameData[i-1], frameData[i-5]);
					}
					else if (frameData[i-7] == 0xe0 && frameData[i-6] == 0xa0) {
						SetHeliosColour(0,255,255);
						DrawSquare(frameData[i-3], frameData[i-1], frameData[i-5]);
					}
					else if (frameData[i-7] == 0x60 && frameData[i-6] == 0xe0) {
						SetHeliosColour(255,255,255);
						DrawSquare(frameData[i-3], frameData[i-1], frameData[i-5]);
					}
					else if (frameData[i-7] == 0x60 && frameData[i-6] == 0x80) {
						SetHeliosColour(255,255,0);
						DrawSquare(frameData[i-3], frameData[i-1], frameData[i-5]);
					}
					else if (frameData[i-7] == 0x60 && frameData[i-6] == 0xc0 ||
							 frameData[i-7] == 0x40 && frameData[i-6] == 0xa0 ) {
						SetHeliosColour(255,0,0);
						DrawSquare(frameData[i-3], frameData[i-1], frameData[i-5]);
					}
					else if (frameData[i-7] == 0x40 && frameData[i-6] == 0x80 ||
							 frameData[i-7] == 0x00 && frameData[i-6] == 0x80 ||
							 frameData[i-7] == 0x00 && frameData[i-6] == 0xc0 ||
							 frameData[i-7] == 0x80 && frameData[i-6] == 0xc0 ||
							 frameData[i-7] == 0x80 && frameData[i-6] == 0x80 ||
							 frameData[i-7] == 0x00 && frameData[i-6] == 0xa0 ||
							 frameData[i-7] == 0xc0 && frameData[i-6] == 0xc0 ) {
						SetHeliosColour(255,127,0);
						DrawBlaster(frameData[i-3], frameData[i-1], frameData[i-5]);
					}
					else if (frameData[i-7] == 0xa0 && frameData[i-6] == 0x80 ||
							 frameData[i-7] == 0x20 && frameData[i-6] == 0x80 ||
							 frameData[i-7] == 0x00 && frameData[i-6] == 0x40 ) {
						SetHeliosColour(192,192,192);
						DrawBlaster(frameData[i-3], frameData[i-1], frameData[i-5]);
					}
				}
				glBegin(GL_LINE_STRIP);
				count += 8;
			}
			else if (frameData[i] == 0xab && frameData[i-1] == 0xcd) { //change colour
				glEnd();
				glBegin(GL_LINE_STRIP);
				i += 2;					
				count += 4;
				SetHeliosColour(frameData[i],bCol*2,frameData[i-1]);
			}
			else if (frameData[i] == 0xde && frameData[i-1] == 0xad) { // draw
				HeliosTranslate(0, 0, 0);
				HeliosScale(1, 1, 0);
				
				i += 2;	
				int xpos = frameData[i-1];
				int ypos = frameData[i];	
				std::vector<int> points{xpos, ypos}; 				
				if (i != 0x1d6b) {
				
				
				
				if (next_blank && !first) {
					WriteHeliosBlank(xpos,ypos);
					next_blank = false;
				}
				
				i += 2;
				if (i != 0x1d6b) {
				xpos = frameData[i-1];
				ypos = frameData[i];
				points.push_back(xpos);
				points.push_back(ypos);
				
				bool repeat = std::find(allPoints.begin(), allPoints.end(), points) != allPoints.end();
				if (!repeat) { 
					WriteHeliosPoint(points[0], points[1]);
					WriteHeliosPoint(points[2], points[3]);

					std::vector<int> points_crs{allPoints.back()[2], allPoints.back()[3], points[0], points[1]};
					std::vector<int> points_crs_bwd{allPoints.back()[2], allPoints.back()[3], points[0], points[1]};
					std::vector<int> points_bwd{points[2], points[3], points[0], points[1]};
					allPoints.push_back(points_crs);	
					allPoints.push_back(points_crs_bwd);
					allPoints.push_back(points_bwd);
					allPoints.push_back(points);	

				}
				}
				}
				count += 6;	
				first = false;				
			} 			
		}

		glEnd();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		WriteHeliosFrame();
	}
	CloseHeliosDAC();
    glfwTerminate();
	
 
    return 0;
}