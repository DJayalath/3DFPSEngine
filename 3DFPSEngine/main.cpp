/* 
NOTE: Runtime Library set to Multi-Threaded rather than Multi-Threaded Debug DLL

NOTE: GLAD header must be included before GLFW since both attempt
to include the OpenGL header and 'glad' will return an error if it detects this
*/

// OpenGL library headers
#include <glad/glad.h> // OpenGL loading library
#include <GLFW/glfw3.h> // Utility library for OpenGL (Graphics library framework)

// Standard headers
#include <iostream> // For printing errors/debugging

// Namespaces
using namespace std;

// Window constatnts
static constexpr int SCREEN_WIDTH = 1280;
static constexpr int SCREEN_HEIGHT = 720;
static constexpr const char* SCREEN_TITLE = "3DFPSEngine";

// Function definitions
void framebuffer_size_callback(GLFWwindow* window, int width, int height); // For resizing glViewport when window is resized

int main(int argc, char* argv[])
{
	// ========== Instantiate GLFW Window ==========
	glfwInit();
	// Use OpenGL version 3.3+
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Use OpenGL core profile and not compatibility profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* Apple operating systems require explicitly stating 
	forward compatibility of the window*/
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	// ========== Create Window Object ==========
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE, NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	// Make context of window the main context on this thread
	glfwMakeContextCurrent(window);

	// ========== Initialise GLAD ==========
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// ========== Setup Viewport ==========
	// Viewport should encompass entire screen
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	// GLFW should call the resize function when window resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ========== Render Loop ==========
	// Exit loop if GLFW instructed to close window
	while (!glfwWindowShouldClose(window))
	{
		// Uses double buffering so buffers must be swapped every frame
		glfwSwapBuffers(window);
		// Poll for triggered events
		glfwPollEvents();
	}

	// Cleanup all of GLFW's allocated resources
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}