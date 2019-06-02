#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Display
{
public:
	Display(int width, int height, const char* title);
	~Display();

	int& GetWidth() { return m_width; }
	int& GetHeight() { return m_height; }

	bool GetInputPressed(int key) { return m_inputs[key] & INPUT_PRESSED; }
	bool GetInputHeld(int key) { return m_inputs[key] & INPUT_HELD; }
	double GetScroll() { return m_mouse_scroll_dy; }
	double GetMouseDX() 
	{
		m_mouse_moved = false;
		return m_mouse_dx; 
	}
	double GetMouseDY() { return m_mouse_dy; }
	bool GetMouseMoved() { return m_mouse_moved; }

	GLFWwindow* GetWindow() { return m_window; }

	bool ShouldClose() { return glfwWindowShouldClose(m_window); }
	void Close() { glfwSetWindowShouldClose(m_window, true); }
	void Update();
	void Clear();

private:

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	enum
	{
		INPUT_PRESSED = 0b01,
		INPUT_HELD = 0b10
	};

	GLFWwindow* m_window = NULL;
	int m_width, m_height;
	bool m_mouse_first = true;
	bool m_mouse_moved = false;
	double m_mouse_lastx, m_mouse_lasty;
	double m_mouse_dx, m_mouse_dy;
	double m_mouse_scroll_dy;
	unsigned char m_inputs[350];
};



Display::Display(int width, int height, const char* title) : m_width(width), m_height(height)
{
	memset(m_inputs, 0, 350);
	// glfw: initialize and configure
// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	m_window = glfwCreateWindow(m_width, m_height, title, NULL, NULL);
	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, mouse_callback);
	glfwSetScrollCallback(m_window, scroll_callback);
	glfwSetKeyCallback(m_window, key_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glfwSetWindowUserPointer(m_window, (void*)this);
}

Display::~Display()
{
	glfwTerminate();
}

void Display::Clear()
{
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::Update()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Display::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	void* data = glfwGetWindowUserPointer(window);
	Display *_this = static_cast<Display *>(data);
	_this->m_width = width;
	_this->m_height = height;
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void Display::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	void* data = glfwGetWindowUserPointer(window);
	Display *_this = static_cast<Display *>(data);

	_this->m_mouse_moved = true;

	if (_this->m_mouse_first)
	{
		_this->m_mouse_lastx = xpos;
		_this->m_mouse_lasty = ypos;
		_this->m_mouse_first = false;
	}

	_this->m_mouse_dx = xpos - _this->m_mouse_lastx;
	_this->m_mouse_dy = _this->m_mouse_lasty - ypos; // reversed since y-coordinates go from bottom to top

	_this->m_mouse_lastx = xpos;
	_this->m_mouse_lasty = ypos;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void Display::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	void* data = glfwGetWindowUserPointer(window);
	Display *_this = static_cast<Display *>(data);

	_this->m_mouse_scroll_dy = yoffset;
}

void Display::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	void* data = glfwGetWindowUserPointer(window);
	Display *_this = static_cast<Display *>(data);

	switch (action)
	{
	case GLFW_PRESS:
		_this->m_inputs[key] |= INPUT_PRESSED;
		break;
	case GLFW_RELEASE:
		_this->m_inputs[key] &= ~INPUT_PRESSED & ~INPUT_HELD;
		break;
	case GLFW_REPEAT:
		_this->m_inputs[key] |= INPUT_HELD;
		break;
	default:
		break;
	}
}