////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Contains main function to create a window and run engine that
///        repeatedly generates a framebuffer and displays it.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <chrono>
#include <iostream>
#include <memory>
#include <algorithm>

// Engine
#include "GLInclude.h"

////////////////////////////////////////////////////////////////////////////////
// Global variables - avoid these

// Window
int g_width{ 1360 };
int g_height{ 768 };
int g_window{ 0 };

// Framebuffer
std::unique_ptr<glm::vec4[]> g_frame{ nullptr }; ///< Framebuffer

// Frame rate
const unsigned int FPS = 60;
float g_frameRate{ 0.f };
std::chrono::high_resolution_clock::time_point g_frameTime{
  std::chrono::high_resolution_clock::now() };
float g_delay{ 0.f };
float g_framesPerSecond{ 0.f };

////////////////////////////////////////////////////////////////////////////////
// Functions

////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize GL settings
void
initialize() {
	glClearColor(0.f, 0.f, 0.0f, 1.f);

	g_frame = std::make_unique<glm::vec4[]>(g_width * g_height);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback for resize of window
///
/// Responsible for setting window size (viewport) and projection matrix.

void
resize(GLint _w, GLint _h) {
	g_width = _w;
	g_height = _h;

	// Viewport 
	/* All the drawing will only be displayed in the viewport. Origin is the left bottom of the
	viewport, g_width * g_height would be the size of viewport*/
	glViewport(0, 0, g_width, g_height); 
	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();  //reset the current matrix (transformation like rotation/scaling)
	*/
	/*git test 1*/
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Timer function to fix framerate in a GLUT application
/// @param _v Value (not used here)
///
/// Note, this is rudametary and fragile.
void
timer(int _v) {
	if (g_window != 0) {
		glutPostRedisplay();

		g_delay = std::max(0.f, 1.f / FPS - g_frameRate);
		glutTimerFunc((unsigned int)(1000.f * g_delay), timer, 0);
	}
	else
		exit(0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Draw function for single frame
void
draw() {
	using namespace std::chrono;

	//////////////////////////////////////////////////////////////////////////////
	// Clear
	for (int i = 0; i < g_width * g_height; ++i)
		g_frame[i] = glm::vec4(0.f, 0.4f, 0.f, 0.f);

	//////////////////////////////////////////////////////////////////////////////
	// Draw

	// Simple static :P
	for (int i = 0; i < g_width * g_height; ++i)
		g_frame[i] = glm::vec4(float(rand()) / RAND_MAX, float(rand()) / RAND_MAX,
			float(rand()) / RAND_MAX, 1.f);

	glDrawPixels(g_width, g_height, GL_RGBA, GL_FLOAT, g_frame.get());

	//////////////////////////////////////////////////////////////////////////////
	// Show
	glutSwapBuffers();

	//////////////////////////////////////////////////////////////////////////////
	// Record frame time
	high_resolution_clock::time_point time = high_resolution_clock::now();
	g_frameRate = duration_cast<duration<float>>(time - g_frameTime).count();
	g_frameTime = time;
	g_framesPerSecond = 1.f / (g_delay + g_frameRate);
	printf("FPS: %6.2f\n", g_framesPerSecond);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback function for keyboard presses
/// @param _key Key
/// @param _x X position of mouse
/// @param _y Y position of mouse
void
keyPressed(GLubyte _key, GLint _x, GLint _y) {
	switch (_key) {
		// Escape key : quit application
	case 27:
		std::cout << "Destroying window: " << g_window << std::endl;
		glutDestroyWindow(g_window);
		g_window = 0;
		break;
		// Unhandled
	default:
		std::cout << "Unhandled key: " << (int)(_key) << std::endl;
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback function for keyboard presses of special keys
/// @param _key Key
/// @param _x X position of mouse
/// @param _y Y position of mouse
void
specialKeyPressed(GLint _key, GLint _x, GLint _y) {
	switch (_key) {
		// Arrow keys
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT:
		break;
		// Unhandled
	default:
		std::cout << "Unhandled special key: " << _key << std::endl;
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Main

////////////////////////////////////////////////////////////////////////////////
/// @brief main
/// @param _argc Count of command line arguments
/// @param _argv Command line arguments
/// @return Application success status
int
main(int _argc, char** _argv) {
	//////////////////////////////////////////////////////////////////////////////
	// Initialize GLUT Window
	std::cout << "Initializing GLUTWindow" << std::endl;
	// GLUT
	glutInit(&_argc, _argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(g_width, g_height); // HD size
	g_window = glutCreateWindow("Spiderling: A Rudamentary Game Engine");

	// GL
	initialize(); 

	//////////////////////////////////////////////////////////////////////////////
	// Assign callback functions
	std::cout << "Assigning Callback functions" << std::endl;
	glutReshapeFunc(resize); 
	glutDisplayFunc(draw); 
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(specialKeyPressed);
	glutTimerFunc(1000 / FPS, timer, 0);

	// Start application
	std::cout << "Starting Application" << std::endl;
	glutMainLoop();

	return 0;
}
