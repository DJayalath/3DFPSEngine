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

// Function definitions
void FramebufferSizeCallback(GLFWwindow* window, int width, int height); // For resizing glViewport when window is resized
void ProcessInput(GLFWwindow *window);

// Window constatnts
static constexpr int SCREEN_WIDTH = 1280;
static constexpr int SCREEN_HEIGHT = 720;
static constexpr const char* SCREEN_TITLE = "3DFPSEngine";

// Shaders
static const char *vertex_shader_source = 
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"out vec3 ourColor;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"   ourColor = aColor;\n"
	"}\0";
const char *fragment_shader_source = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 ourColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(ourColor, 1.0f);\n"
	"}\n\0";

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
#ifdef __APPLE__
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
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	// ========== Shader Compilation ==========
	// VERTEX SHADER
	// Create and assign shader object
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	// Provide shader source code and compile
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	// Test if compilation was successful
	int success;
	char info_log[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << endl;
	}
	// FRAGMENT SHADER
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);
	// Test for compilation errors
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << endl;
	}

	// ========== Linking Shaders ==========
	// Create a shader program
	unsigned int shader_program = glCreateProgram();
	// Attach and link vertex and fragment shaders
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	// Check for linking errors
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << endl;
	}
	// Shaders are no longer needed so can be deleted
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// ========== Test Object ==========
	float vertices[] = 
	{
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	// Create VBO, VAO and EBO
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// 1. Bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. Copy vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 4. Set the vertex attributes pointers
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Colour attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Safely unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// DO NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ========== Render Loop ==========
	// Exit loop if GLFW instructed to close window
	while (!glfwWindowShouldClose(window))
	{
		// INPUT
		ProcessInput(window);

		// RENDERING
		// Set clear colour and clear viewport
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use shader program
		glUseProgram(shader_program);
		// Draw the test object
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// EVENTS AND BUFFER SWAPPING
		// Uses double buffering so buffers must be swapped every frame
		glfwSwapBuffers(window);
		// Poll for triggered events
		glfwPollEvents();
	}

	// Cleanup all of GLFW's allocated resources
	glfwTerminate();

	return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}