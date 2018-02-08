#include "cwk1.h"


using namespace glm;

//Cube entity
Entity cube, cone, cylinder;

//Window object  
GLFWwindow* window;
//Shader program
GLuint 	program_id;
//Matrix handle to vertex shader
GLuint 	mvp_handle, pos_handle;
//Shader paths
const char
* vertex_shader = "tut.vert",
* fragment_shader = "tut.frag";

glm::mat4 Projection, View;

//Window dimensions
const int width = 1280, height = 720;
// position
glm::vec3 position = glm::vec3(0, 0, 5);
// horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// vertical angle : 0, look at the horizon
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;
//Speed variable
float speed = 1.0f; // 3 units / second
//Mouse sensitivity
float mouseSpeed = 0.1f;
//Time delta
float dt = 0.01;
//FPS toggle
bool fps_on = 0;
//Camera vectors
glm::vec3 right, up(0, 1, 0), direction;

//Cube vertices count
const int cube_n = 36;
//Cube vertex data array
GLfloat cube_v_b[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

std::vector<glm::vec3> generate_cube()
{
	std::vector<glm::vec3> v;
	v.push_back(glm::vec3(-1.0f, -1.0f, -1.0f)); // triangle 1 : begin
	v.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	v.push_back(glm::vec3(-1.0f, 1.0f, 1.0f)); // triangle 1 : end
	v.push_back(glm::vec3(1.0f, 1.0f, -1.0f)); // triangle 2 : begin
	v.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	v.push_back(glm::vec3(-1.0f, 1.0f, -1.0f)); // triangle 2 : end
	v.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	v.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	v.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	v.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	v.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	v.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	v.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	v.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	v.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	v.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	v.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	v.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	v.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	v.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	v.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	v.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	v.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	v.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	v.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	v.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	v.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	v.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	v.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	v.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	v.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	v.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	v.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	v.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	v.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	return v;
}


std::vector<glm::vec3> generate_cone(int k)
{
	std::vector<glm::vec3> v;
	float step = 2. * 3.141596 / float(k);
	float Radius = 1., c=0., s = 0.;
	float len = 2.;
	for (float a = 0; a <= (2. * 3.141596); a += step)
	{
		v.push_back(vec3());
		c = Radius * cos(a);
		s = Radius * sin(a);
		v.push_back(vec3(c, s, len));
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);
		v.push_back(vec3(c, s, len));
	}
	for (float a = 0; a <= (2. * 3.141596); a += step)
	{
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);
		v.push_back(vec3(c, s, len));
		c = Radius * cos(a);
		s = Radius * sin(a);
		v.push_back(vec3(c, s, len));
		v.push_back(vec3(0., 0., len));
	}
	return v;
}

std::vector<glm::vec3> generate_cylinder(int k)
{
	std::vector<glm::vec3> v;
	glm::vec3 t1, t2;
	float len = 2.;
	float step = 2. * 3.141596 / float(k);
	float Radius = 1., c = 0., s = 0.;
	for (float a = 0; a <= (2. * 3.141596); a += step)
	{	
		v.push_back(vec3());
		c = Radius * cos(a);
		s = Radius * sin(a);
		v.push_back(vec3(c, s, 0.0));
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);
		v.push_back(vec3(c, s, 0.0));		
	}
	for (float a = 0; a > -(2. * 3.141596); a -= step)
	{		
		c = Radius * cos(a);
		s = Radius * sin(a);
		t1 = vec3(c, s, 0.0);
		t2 = vec3(c, s, len);
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);

		v.push_back(t1);
		v.push_back(t2);
		v.push_back(vec3(c, s, len));
		v.push_back(vec3(c, s, len));
		v.push_back(vec3(c, s, 0.0));
		v.push_back(t1);
	}
	for (float a = 0; a <= (2. * 3.141596); a += step)
	{
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);
		v.push_back(vec3(c, s, len));		
		c = Radius * cos(a);
		s = Radius * sin(a);
		v.push_back(vec3(c, s, len));
		v.push_back(vec3(0.,0.,len));
	}
	return v;
}

Particle::Particle(vec3 p, vec3 v)
{
	pos = p;
	vel = v;
}
void Particle::update(float dt)
{
	pos += vel * dt;
}

Entity::Entity(glm::vec3 * v, glm::vec3 * c, int _n)
{
	v_b = v;
	c_b = c;
	n = _n;
}
void Entity::init()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vert_b);

	glBindBuffer(GL_ARRAY_BUFFER, vert_b);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(glm::vec3), v_b, GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vert_b);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glGenBuffers(1, &colour_b);
	glBindBuffer(GL_ARRAY_BUFFER, colour_b);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(glm::vec3), c_b, GL_STATIC_DRAW);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colour_b);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
void Entity::draw()
{

	glm::mat4 Model = glm::translate(glm::mat4(1.), p.pos);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

											   // Send our transformation to the currently bound shader, in the "MVP" uniform
											   // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, &mvp[0][0]);

	glBindVertexArray(vao);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, n * sizeof(glm::vec3)); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glBindVertexArray(0);
}

//Randomises the colour buffer passed
void random_colour_buffer(glm::vec3 ** buffer_data, int n)
{
	*buffer_data = (glm::vec3*)std::malloc(n * sizeof(glm::vec3));
	for (int v = 0; v < n; v++)
			(*buffer_data)[v] = glm::vec3(randf(), randf(), randf());
}
//Randomises the colour buffer passed
void generate_colour_buffer(glm::vec3 ** buffer_data, int n, glm::vec3 colour)
{
	*buffer_data = (glm::vec3*)std::malloc(n * sizeof(glm::vec3));
	for (int v = 0; v < n; v++)
		(*buffer_data)[v] = colour;
}


//Error callback  
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}
//Loads shaders from their files into a shader program
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


//Initilise custom objects
void init_objects()
{
	std::vector<vec3> v = generate_cube();
	cube.v_b = v.data();
	cube.n = v.size();
	random_colour_buffer(&cube.c_b, cube.n);
	cube.p.pos = glm::vec3(0, 0., 0);
	cube.init();

	std::vector<vec3> v1 = generate_cone(30);
	cone.v_b = v1.data();
	cone.n = v1.size();
	random_colour_buffer(&cone.c_b, cone.n);
	cone.p.pos = glm::vec3(0, 2., 0);
	cone.init();

	std::vector<vec3> v2 = generate_cylinder(30);
	cylinder.v_b = v2.data();
	cylinder.n = v2.size();
	random_colour_buffer(&cylinder.c_b, cylinder.n);
	cylinder.p.pos = glm::vec3(0, -2., 0);
	cylinder.init();
}

//Key input callback  
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position += direction * dt * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= direction * dt * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * dt * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * dt * speed;
	}

	// FPS toggle
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		fps_on = !fps_on;
	}
}

//Custom graphics loop
void loop()
{
	cube.draw();
	cone.draw();
	cylinder.draw();
}


//GL window initialose
int initWindow()
{
	//Set the error callback  
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW  
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional  
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version  
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version  
	//glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing  
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  

	//Create a window and create its OpenGL context  
	window = glfwCreateWindow(width, height, "Test Window", NULL, NULL);

	//If the window couldn't be created  
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread.   
	glfwMakeContextCurrent(window);

	//Sets the key callback  
	glfwSetKeyCallback(window, key_callback);

	//Initialize GLEW  
	GLenum err = glewInit();

	//If GLEW hasn't initialized  
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return -1;
	}

	program_id = LoadShaders(vertex_shader, fragment_shader);

	glUseProgram(program_id);

	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	mvp_handle = glGetUniformLocation(program_id, "MVP");
	pos_handle = glGetUniformLocation(program_id, "pos");

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	init_objects();

}
//FPS code
void runFPSControls()
{
	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	// Reset mouse position for next frame
	double xmid = width / 2.0, ymid = height / 2.0;
	glfwSetCursorPos(window, xmid, ymid);
	
	// Compute new orientation
	horizontalAngle += mouseSpeed * dt * float(xmid - xpos);
	verticalAngle += mouseSpeed * dt * float(ymid - ypos);

	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);
	// Up vector : perpendicular to both direction and right
	up = glm::cross(right, direction);

	
}
//GL graphics loop
void glLoop(void(*graphics_loop)())
{
	//Main Loop  
	do
	{
		if(fps_on)
			runFPSControls();

		position = glm::quat(glm::vec3(0.05 * dt, 0.1*dt, 0)) * position;

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

		// Or, for an ortho camera :
		//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

		// Camera matrix
		View = glm::lookAt(
			position, // Camera is at (4,3,3), in World Space
			-position, // and looks at the origin
			up  // Head is up (set to 0,-1,0 to look upside-down)
		);

		//Clear color buffer  
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(.5,.5,.5,1.);

		graphics_loop();

		//Swap buffers  
		glfwSwapBuffers(window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();

	} //Check if the ESC key had been pressed or if the window had been closed  
	while (!glfwWindowShouldClose(window));

	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(window);
	//Finalize and clean up GLFW  
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

//cwk1 main function
int cwk1_main()
{
	initWindow();
	glLoop(loop);

	return 0;
}