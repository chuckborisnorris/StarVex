#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}  

GLFWwindow* InitWindow (GLFWwindow* window, int winH, int winW) {
	/* Initialize the library */
    glfwInit();

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(winW, winH, "StarVex", NULL, NULL);
    if (!window) {
        glfwTerminate();
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glViewport(0, 0, winW, winH); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, winW, winH, 0, -10, 10);

	//Back to the modelview so we can draw stuff 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
	return window;
}