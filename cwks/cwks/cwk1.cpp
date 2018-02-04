#include "cwk1.h"

#include "utils.h"

GLuint vbo[2]; /* Create handles for two Vertex Buffer Objects */
GLuint vao; /* Create handles for one Vertex Array Object */
const GLfloat diamond[4][2] = {  /* We're going to create a simple diamond made from lines */
	{ 0.0,  0.5 },  /* Top point    */
	{ 0.5,  0.0 },  /* Right point  */
	{ 0.0, -0.5 },  /* Bottom point */
	{ -0.5,  0.0 }
}; /* Left point   */
const GLfloat colors[4][3] = {
	{ 1.0,  0.0,  0.0 },  /* Red   */
	{ 0.0,  1.0,  0.0 },  /* Green */
	{ 0.0,  0.0,  1.0 },  /* Blue  */
	{ 0.0,  1.0,  1.0 }
}; /* White */
GLchar *vertexsource, *fragmentsource; /* These pointers will receive the contents of our shader source code files */
GLuint vertexshader, fragmentshader; /* These are handles used to reference the shaders */
GLuint shaderprogram; /* This is a handle to the shader program */


void SetupShaders(void) 
{
	char text[1000];
	int length;
	fprintf(stderr, "Setting up shaders\n"); /* Allocate and assign two Vertex Buffer Objects to our handle */
	vertexsource = filetobuf("./tut2.vert"); /* Read our shaders into the appropriate buffers */
	fragmentsource = filetobuf("./tut2.frag");
	if (vertexsource && fragmentsource) {
		printf("Shader Code loaded\n");
	}
	else {
		printf("Shader files not found\n");
	}
	vertexshader = glCreateShader(GL_VERTEX_SHADER); /* Assign our handles a "name" to new shader objects */
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0); /* Associate the source code buffers with each handle */
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
	glCompileShader(fragmentshader);/* Compile our shader objects */
	glCompileShader(vertexshader);
	shaderprogram = glCreateProgram();/* Assign our program handle a "name" */
	glAttachShader(shaderprogram, vertexshader); /* Attach our shaders to our program */
	glAttachShader(shaderprogram, fragmentshader);
	glLinkProgram(shaderprogram); /* Link our program */
	glGetProgramInfoLog(shaderprogram, 1000, &length, text); // Check for errors
	if (length > 0) {
		fprintf(stderr, "Validate Shader Program\n%s\n", text);
		fprintf(stderr, "\n%s\n", text);
	}
	glUseProgram(shaderprogram); /* Set it as being actively used */
	printf("All Compiled\n");
}
void SetupGeometry(void) 
{
	fprintf(stderr, "Set up vertices\n");
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	fprintf(stderr, "vao %d\n", vao);
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glEnableVertexAttribArray(0);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), diamond, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glEnableVertexAttribArray(1);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(shaderprogram, 0, "aIn_Position");
	glBindAttribLocation(shaderprogram, 1, "aIn_Color");
	glBindVertexArray(0);
	printf("All Finished\n");
}
void Render(void) 
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vao);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glBindVertexArray(0);
	Check("Test point");
	glFlush();
}

int cwk1_main()
{
	GLFWwindow* window;
	if (!glfwInit()) {
		printf("Failed to start GLFW\n");
		exit(EXIT_FAILURE);
	}
	/*
	* Window Creation
	*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		printf("GLFW Failed to start\n");
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
#ifndef __APPLE__
	// IMPORTANT: make window curren must be done so glew recognises OpenGL
	glewExperimental = GL_TRUE;
	int err = glewInit();
	if (GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
#endif
	fprintf(stderr, "GL INFO %s\n", glGetString(GL_VERSION));
	SetupShaders();
	SetupGeometry();
	glClearColor(1., 1., 1., 1.);
	while (!glfwWindowShouldClose(window)) {// Main loop
		Render();// OpenGL rendering goes here...
		glfwSwapBuffers(window);// Swap front and back rendering buffers
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();// Close window and terminate GLFW

	return 0;
}