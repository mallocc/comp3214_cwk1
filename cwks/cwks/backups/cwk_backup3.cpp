#include "cwk1.h"
#include "stb_image/stb_image.h"

using namespace glm;

#define SCREEN_A 0
#define SCREEN_B 1
#define SCREEN_C 2
#define SCREEN_D 3
#define SCREEN_E 4

int screen_number = SCREEN_A;

//colours
glm::vec3 
	WHITE(1, 1, 1), 
	BLACK(0,0,0), 
	GREY(.5,.5,.5), 
	OFF_BLACK(.1,.1,.1),
	ambient_color = OFF_BLACK;


Entity sphere, ground;

Composite_Entity rocket;

//Window object  
GLFWwindow* window;
//Shader program
GLuint
	program_id,
	program_a,
	program_b,
	program_c,
	program_d,
	program_e,
	current_program,
//handles to shader
	m_handle,
	v_handle,
	p_handle;

// self contained custom variable handlers
Var_Handle
mat4_handles[3],
light_handles[5],
ambient_color_handle,
texture_handle;

//Shader paths
const char
	*vertex_shader     = "shaders/tut.vert",
	*fragment_shader   = "shaders/tut.frag",
	*vertex_shader_a   = "shaders/A.vert",
	*fragment_shader_a = "shaders/A.frag",
	*vertex_shader_b   = "shaders/B.vert",
	*fragment_shader_b = "shaders/B.frag",
	*vertex_shader_c   = "shaders/B.vert",
	*fragment_shader_c = "shaders/B.frag",
	*vertex_shader_d   = "shaders/D.vert",
	*fragment_shader_d = "shaders/D.frag",
	*vertex_shader_e   = "shaders/E.vert",
	*fragment_shader_e = "shaders/E.frag";

//proj and view matrix
glm::mat4 
	Projection, 
	View;

//Window dimensions
const int 
	width                       = 1280, 
	height                      = 720;
// eye position
glm::vec3
	//Camera vectors
	eye_position                = glm::vec3(0, 0, 5),
	eye_direction               = glm::vec3(0, 0, 0),
	right,
	up(0, 1, 0),
	direction;

	float 
	horizontalAngle             = 3.14f,
	verticalAngle               = 0.0f,
	// Initial Field of View
	initialFoV                  = 45.0f,
	// Speed variable
	speed                       = 20.0f,
	// Mouse sensitivity
	mouseSpeed                  = 0.1f,
	// Time delta
	dt                          = 0.01;
	// toggles
bool 
	fps_on                      = 0,
	wire_frame                  = 0;

int test1                       = 0;

Light lights = { glm::vec3(0,0,10),glm::vec3(1,1,1),50,0.9,500 };

Obj model = Obj("objects/XL5-BASE.obj", WHITE);
//Obj model = Obj("bb8.obj", "bb8.png", WHITE);


GLuint loadTexturePNG(const char *fname)
{
	int w, h, n;
	unsigned char *data = stbi_load(fname, &w, &h, &n, 0);
	if (data == NULL) {
		fprintf(stderr, "Image not loaded: %s", fname);
		const char *error = stbi_failure_reason();
		fprintf(stderr, "Failure reason %s\n", error);
	}
	GLuint tex = 1;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	// set the texture wrapping parameters
	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	delete data;
	return tex;
}

//shape generators non indexed triangles
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
std::vector<glm::vec3>			generate_cube()
{
	std::vector<glm::vec3> v;
	for (int i = 0; i < 36; i++)
		v.push_back(glm::vec3(cube_v_b[i*3], cube_v_b[i * 3+1], cube_v_b[i * 3+2]));
	return v;
}
std::vector<glm::vec3>			generate_cone(int k)
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
std::vector<glm::vec3>			generate_cylinder(int k, float len)
{
	std::vector<glm::vec3> v;
	glm::vec3 t1, t2;
	float step = 2. * 3.141596 / float(k);
	float Radius = 1., c = 0., s = 0.;
	for (float a = 0; a <= (2. * 3.141596); a += step)
	{	
		v.push_back(vec3(0.,0., 0));
		c = Radius * cos(a);
		s = Radius * sin(a);
		v.push_back(vec3(c, s, 0));
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);
		v.push_back(vec3(c, s, 0));
	}
	for (float a = 0; a > -(2. * 3.141596); a -= step)
	{		
		c = Radius * cos(a);
		s = Radius * sin(a);
		t1 = vec3(c, s, 0);
		t2 = vec3(c, s, len);
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);

		v.push_back(t1);
		v.push_back(t2);
		v.push_back(vec3(c, s, len));
		v.push_back(vec3(c, s, len));
		v.push_back(vec3(c, s, 0));
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
std::vector<glm::vec3>			generate_sphere(int lats, int longs)
{
	std::vector<glm::vec3> v;
	glm::vec3 t1, t2;
	float len = 1.;
	float step_lats = 2. * 3.141596 / float(lats);
	float step_longs = 2. * 3.141596 / float(longs);
	float Radius = 1., x, y, z;
	for (float a = step_lats; a <= (2. * 3.141596) + step_lats; a += step_lats)
	{
		for (float b = 0; b <= (2. * 3.141596); b += step_longs)
		{
			x = Radius * cos(a) * cos(b);
			y = Radius * cos(a) * sin(b);
			z = Radius * sin(a);
			v.push_back(glm::vec3(x, y, z));
			x = Radius * cos(a + step_lats) * cos(b);
			y = Radius * cos(a + step_lats) * sin(b);
			z = Radius * sin(a + step_lats);
			v.push_back(glm::vec3(x, y, z));
			x = Radius * cos(a + step_lats) * cos(b + step_longs);
			y = Radius * cos(a + step_lats) * sin(b + step_longs);
			z = Radius * sin(a + step_lats);
			v.push_back(glm::vec3(x, y, z));

			v.push_back(glm::vec3(x, y, z));
			x = Radius * cos(a) * cos(b + step_longs);
			y = Radius * cos(a) * sin(b + step_longs);
			z = Radius * sin(a);
			v.push_back(glm::vec3(x, y, z));
			x = Radius * cos(a) * cos(b);
			y = Radius * cos(a) * sin(b);
			z = Radius * sin(a);
			v.push_back(glm::vec3(x, y, z));
		}
	}

	return v;
}
std::vector<glm::vec3>			generate_normals(std::vector<glm::vec3> v)
{
	std::vector<glm::vec3> n;
	for (int i = 0; i < v.size(); i += 3)
	{
		glm::vec3 nm = glm::normalize(glm::cross(v[i + 1] - v[i], v[i + 2] - v[i]));
		for (int j = 0; j < 3; ++j)
			//n.push_back(-v[i]);
			n.push_back(nm);
	}
	return n;
}
std::vector<glm::vec3>			generate_rect()
{
	std::vector<glm::vec3> n;
	n.push_back(glm::vec3(1, 0, 1));
	n.push_back(glm::vec3(1, 0, -1));
	n.push_back(glm::vec3(-1, 0, -1));
	n.push_back(glm::vec3(-1, 0, -1));
	n.push_back(glm::vec3(-1, 0, 1));
	n.push_back(glm::vec3(1, 0, 1));
	return n;
}
std::vector<vec3>				generate_colour_buffer(glm::vec3 colour, int n)
{
	std::vector<vec3> v;
	for (int i = 0; i < n; i++)
		v.push_back(colour);
	return v;
}
std::vector<vec3>				random_colour_buffer(glm::vec3 max, int n)
{
	std::vector<vec3> v;
	for (int i = 0; i < n; i++)
		v.push_back(glm::vec3(max.x*randf(), max.y*randf(), max.z*randf()));
	return v;
}
std::vector<vec3>				random_intesity_colour_buffer(glm::vec3 colour, int n)
{
	std::vector<vec3> v;
	float f;
	for (int i = 0; i < n; i++)
	{
		f = randf();
		v.push_back(glm::vec3(colour.x*f, colour.y*f, colour.z*f));
	}
	return v;
}
std::vector<Vertex>				pack_object(std::vector<glm::vec3> * v, std::vector<glm::vec3> * c, std::vector<glm::vec3> * n, std::vector<glm::vec2> * uv)
{
	std::vector<Vertex> object;
	for (int i = 0; i < v->size(); ++i)
	{
		Vertex vert;
		if(v != NULL)
		vert.position = (*v)[i];
		if (c != NULL)
		vert.color = (*c)[i];
		if (n != NULL)
		vert.normal = (*n)[i];
		if (uv != NULL)
		vert.uv = (*uv)[i];
		object.push_back(vert);
	}
	return object;
}

std::vector<vec2>				generate_random_uvs(std::vector<glm::vec3> v)
{
	std::vector<vec2> uv;
	for (int i = 0; i < v.size(); i++)
		uv.push_back(glm::normalize(glm::vec2(v[i].x, v[i].z)));
	return uv;
}

void Entity::init()
{
	Vertex * d = data.data();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(struct Vertex), d, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
		(const GLvoid*)offsetof(struct Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
		(const GLvoid*)offsetof(struct Vertex, color));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
		(const GLvoid*)offsetof(struct Vertex, normal));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	glFlush();
}
void Entity::draw(int wire_frame)
{
	glm::mat4 Model = 
		glm::translate(glm::mat4(1.), p.pos) * 
		glm::rotate(glm::mat4(1.), theta, rotation) * 
		glm::scale(glm::mat4(1.), scale);
	mat4_handles[0].load(Model);
	mat4_handles[1].load();
	mat4_handles[2].load();

	ambient_color_handle.load();

	for (Var_Handle v : light_handles)
		v.load();

	draw_array(wire_frame);
}
void Entity::draw_array(int wire_frame)
{
	if (p.life > 0)
	{
		glBindVertexArray(vao);
		glDrawArrays(wire_frame ? GL_LINE_LOOP : GL_TRIANGLES, 0, data.size());
		glBindVertexArray(0);
		glFinish();
	}
}
void Composite_Entity::init()
{
	for (Entity e : entities)
		e.init();
}
void Composite_Entity::draw(int wire_frame)
{
	glm::mat4 comp_Model = glm::translate(glm::mat4(1.), p.pos) * glm::rotate(glm::mat4(1.), theta, rotation) * glm::scale(glm::mat4(1.), scale);
	mat4_handles[1].load();
	mat4_handles[2].load();

	ambient_color_handle.load();

	for (Var_Handle v : light_handles)
		v.load();

	for (Entity e : entities)
	{
		glm::mat4 Model = glm::translate(glm::mat4(1.), e.p.pos) * glm::rotate(glm::mat4(1.), e.theta, e.rotation);
		Model = comp_Model * Model;
		mat4_handles[0].load(Model);
		e.draw_array(wire_frame);
	}
}
void Composite_Entity::add(Entity e)
{
	entities.push_back(e);
}
Obj::Obj(const char *filename, glm::vec3 c)
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	std::vector< glm::vec3 > colours;

	tinyobj::LoadObj(shapes, materials, filename, NULL);

	for (int i = 0; i < shapes.size(); i++)
		for (int j = 0; j < shapes[i].mesh.indices.size(); j++)
			vertices.push_back(glm::vec3(
				shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3],
				shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3 + 1],
				shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3 + 2]
			));
	colours = random_colour_buffer(c, vertices.size());
	normals = generate_normals(vertices);
	uvs = generate_random_uvs(vertices);

	data = pack_object(&vertices, &colours, &normals, &uvs);
}
Obj::Obj(const char *filename, glm::vec3 c,
	Particle _p, glm::vec3 _rotation, GLfloat _theta, glm::vec3 _scale)
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	std::vector< glm::vec3 > colours;

	tinyobj::LoadObj(shapes, materials, filename, NULL);

	for (int i = 0; i < shapes.size(); i++)
		for (int j = 0; j < shapes[i].mesh.indices.size(); j++)
			vertices.push_back(glm::vec3(
				shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3],
				shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3 + 1],
				shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3 + 2]
			));
	colours = random_colour_buffer(c, vertices.size());
	normals = generate_normals(vertices);
	uvs = generate_random_uvs(vertices);

	data = pack_object(&vertices, &colours, &normals, &uvs);

	p = _p;
	rotation = _rotation;
	theta = _theta;
	scale = _scale;
}
Obj::Obj(std::vector<Vertex> _data,
	Particle _p, glm::vec3 _rotation, GLfloat _theta, glm::vec3 _scale)
{
	data = _data;
	p = _p;
	rotation = _rotation;
	theta = _theta;
	scale = _scale;
}
void Obj::init(const char *texfilename)
{
	if (texfilename != "")
		tex = loadTexturePNG(texfilename);
	Vertex * d = data.data();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(struct Vertex), d, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
		(const GLvoid*)offsetof(struct Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
		(const GLvoid*)offsetof(struct Vertex, color));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
		(const GLvoid*)offsetof(struct Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer((GLuint)3, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
		(const GLvoid*)offsetof(struct Vertex, uv));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0);
	glFlush();
}
void Obj::draw()
{
	glm::mat4 Model =
		glm::translate(glm::mat4(1.), p.pos) *
		glm::rotate(glm::mat4(1.), theta, rotation) *
		glm::scale(glm::mat4(1.), scale);
	mat4_handles[0].load(Model);
	mat4_handles[1].load();
	mat4_handles[2].load();

	ambient_color_handle.load();
	
	loadTexturehandle(&texture_handle);

	for (Var_Handle v : light_handles)
		v.load();

	glActiveTexture(GL_TEXTURE0 + tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, data.size());
	glBindVertexArray(0);
	glFinish();
}



void			reset_rocket()
{
	rocket.p.pos = glm::vec3();
	rocket.p.vel = glm::vec3();
}



//Error callback  
static void		error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}
//Loads shaders from their files into a shader program (from opengl-tutorials.org)
GLuint			LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

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


void			setup_program_handles(GLuint prog)
{
	texture_handle = Var_Handle("uTex");
	texture_handle.init(prog);

	ambient_color_handle = Var_Handle("ambient_color", &ambient_color);
	ambient_color_handle.init(prog);

	mat4_handles[0] = Var_Handle("M");
	mat4_handles[1] = Var_Handle("V", &View);
	mat4_handles[2] = Var_Handle("P", &Projection);
	for (int i = 0; i < 3; ++i)
		mat4_handles[i].init(prog);

	light_handles[0] = Var_Handle("light", &lights.pos);
	light_handles[1] = Var_Handle("diffuse_color", &lights.color);
	light_handles[2] = Var_Handle("brightness", &lights.brightness);
	light_handles[3] = Var_Handle("shininess", &lights.shininess);
	light_handles[4] = Var_Handle("specular_scale", &lights.specular_scale);

	for (int i = 0; i < 5; ++i)
		light_handles[i].init(prog);
}
//Initilise custom objects
void			init_objects()
{
	model.scale *= 0.05f;
	model.init("tex_grad1.bmp");


	// create sphere for a and b
	std::vector<vec3> v = generate_sphere(100,100);
	std::vector<vec3> n = generate_normals(v);
	std::vector<vec3> c = random_intesity_colour_buffer(WHITE,v.size());
	std::vector<Vertex> o = pack_object(&v, &c, &n, NULL);
	sphere = Entity(
		o,
		Particle(glm::vec3(), glm::vec3()),
		glm::vec3(1, 0,0),
		glm::radians(90.0f),
		glm::vec3(1,1,1)
		);
	sphere.init();

	Entity temp;

	// create rocket
	// cone
	v = generate_cone(100);
	n = generate_normals(v);
	c = generate_colour_buffer(GREY, v.size());
	o = pack_object(&v, &c, &n, NULL);
	temp = Entity(
		o,
		Particle(glm::vec3(), glm::vec3()),
		glm::vec3(1, 0, 0),
		glm::radians(90.0f),
		glm::vec3(1, 1, 1)
	);
	temp.init();
	rocket.add(temp);
	// cone2
	temp = Entity(
		o,
		Particle(glm::vec3(0,-7,0), glm::vec3()),
		glm::vec3(1, 0, 0),
		glm::radians(90.0f),
		glm::vec3(1, 1, 1)
	);
	temp.init();
	rocket.add(temp);
	// cylinder
	v = generate_cylinder(100, 5);
	n = generate_normals(v);
	c = generate_colour_buffer(WHITE, v.size());
	o = pack_object(&v, &c, &n, NULL);
	temp = Entity(
		o,
		Particle(glm::vec3(0,-2,0), glm::vec3()),
		glm::vec3(1, 0, 0),
		glm::radians(90.0f),
		glm::vec3(1, 1, 1)
	);
	temp.init();
	rocket.add(temp);
	// sphere
	v = generate_sphere(100, 100);
	n = generate_normals(v);
	c = generate_colour_buffer(WHITE, v.size());
	o = pack_object(&v, &c, &n, NULL);
	temp = Entity(
		o,
		Particle(glm::vec3(0,-7,0), glm::vec3()),
		glm::vec3(1, 0, 0),
		glm::radians(0.0f),
		glm::vec3(1, 1, 1)
	);
	temp.init();
	rocket.add(temp);

	rocket.scale *= 0.1f;

	//ground
	v = generate_rect();
	n = generate_normals(v);
	c = generate_colour_buffer(WHITE, v.size());
	o = pack_object(&v, &c, &n, NULL);
	ground = Entity(
		o,
		Particle(glm::vec3(0,-1.0f,0), glm::vec3()),
		glm::vec3(1, 0, 0),
		glm::radians(0.0f),
		glm::vec3(10, 1, 10)
	);
	ground.init();
}
//Key input callback  
static void		key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			// changing screen
		case GLFW_KEY_A:
			screen_number = SCREEN_A;
			current_program = program_a;
			eye_position = vec3(0, 0, 5);
			eye_direction = vec3(0, 0, 0);
			lights.pos = glm::vec3(0, 0, 10);
			setup_program_handles(current_program);
			break;
		case GLFW_KEY_B:
			screen_number = SCREEN_B;
			current_program = program_b;
			eye_position = vec3(0, 0, 5);
			eye_direction = vec3(0, 0, 0);
			lights.pos = glm::vec3(0, 0, 10);
			setup_program_handles(current_program);
			break;
		case GLFW_KEY_C:
			screen_number = SCREEN_C;
			current_program = program_c;
			eye_position = vec3(0, 0, 5);
			eye_direction = vec3(0, 0, 0);
			lights.pos = glm::vec3(0, 0, 10);
			setup_program_handles(current_program);
			reset_rocket();
			break;
		case GLFW_KEY_D:
			screen_number = SCREEN_D;
			current_program = program_d;
			eye_position = vec3(0, 0, 5);
			eye_direction = vec3(0, 0, 0);
			lights.pos = glm::vec3(0, 0, 10);
			setup_program_handles(current_program);
			break;
		case GLFW_KEY_E:
			screen_number = SCREEN_E;
			current_program = program_e;
			eye_position = vec3(0, 0, 5);
			eye_direction = vec3(0, 0, 0);
			lights.pos = glm::vec3(0,5,10);
			setup_program_handles(current_program);
			break;

			//other
		case GLFW_KEY_ESCAPE:
		case GLFW_KEY_Q:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_UP:
			eye_position += direction * dt * speed;
			break;
		case GLFW_KEY_DOWN:
			eye_position -= direction * dt * speed;
			break;
		case GLFW_KEY_RIGHT:
			eye_position = glm::quat(glm::vec3(0, glm::radians(10.0f), 0)) * eye_position;
			break;
		case GLFW_KEY_LEFT:
			eye_position = glm::quat(glm::vec3(0, -glm::radians(10.0f), 0)) * eye_position;
			break;
		case GLFW_KEY_ENTER:
			fps_on = !fps_on;
			break;
		case GLFW_KEY_R:
			break;
		case GLFW_KEY_W:
			wire_frame = !wire_frame;
			break;
		case GLFW_KEY_I:
			test1++;
			break;
		}
	}
}
//Custom graphics loop
void			loop()
{
	switch (screen_number)
	{
	case SCREEN_A:
		sphere.draw(1);
		break;
	case SCREEN_B:
		sphere.draw(0);
		break;
	case SCREEN_C:
		rocket.p.vel += glm::vec3(0, 0.01, 0) * dt;
		eye_direction = rocket.p.pos;
		rocket.p.update(dt);
		rocket.draw(wire_frame);
		ground.draw(wire_frame);
		break;
	case SCREEN_D:
		
		break;
	case SCREEN_E:
		model.draw();
		break;
	}
}


//GL window initialose
int				initWindow()
{
	//Set the error callback  
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW  
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

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
	program_a = LoadShaders(vertex_shader_a, fragment_shader_a);
	program_b = LoadShaders(vertex_shader_b, fragment_shader_b);
	program_c = LoadShaders(vertex_shader_c, fragment_shader_c);
	program_d = LoadShaders(vertex_shader_d, fragment_shader_d);
	program_e = LoadShaders(vertex_shader_e, fragment_shader_e);
	current_program = program_a;

	setup_program_handles(current_program);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_2D);

	init_objects();

}
//GL graphics loop
void			glLoop(void(*graphics_loop)())
{
	//Main Loop  
	do
	{
		glUseProgram(current_program);

		auto start = std::chrono::high_resolution_clock::now();

		direction = -glm::normalize(eye_position);
		right = glm::normalize(glm::cross(direction, up));

		Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
		View = glm::lookAt(eye_position, eye_direction, up);

		//Clear color buffer  
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 back_color = ambient_color * 1.0f;
		glClearColor(back_color.x, back_color.y, back_color.z,1.);

		graphics_loop();

		// check OpenGL error
		//GLenum err;
		//while ((err = glGetError()) != GL_NO_ERROR) {
		//	std::cerr << "OpenGL error: " << err << std::endl;
		//}


		//Swap buffers  
		glfwSwapBuffers(window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();


		auto finish = std::chrono::high_resolution_clock::now();
		int ms = float(std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count());
		long newWait = 5 - ms;// -(gm.gameSpeed);
		newWait = newWait < 0 ? 0 : newWait;
		std::this_thread::sleep_for(std::chrono::milliseconds(newWait));

	} //Check if the ESC key had been pressed or if the window had been closed  
	while (!glfwWindowShouldClose(window));

	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(window);
	//Finalize and clean up GLFW  
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

//cwk1 main function
int				cwk1_main()
{
	initWindow();
	glLoop(loop);

	return 0;
}