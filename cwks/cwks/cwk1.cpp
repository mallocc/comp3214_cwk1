// screen A
// wire mesh : coloured uniformly blue
// screen B
// triangle faced : coloured uniformly blue, ambient is dark grey, light is red




#include "cwk1.h"
#include "stb_image/stb_image.h"

using namespace glm;

#define GEN_NORMS 0x1
#define GEN_TANGS 0x2
#define GEN_UVS_POLAR 0x4
#define GEN_UVS_RECTS 0x8
#define GEN_UVS_SPHERE 0x80
#define GEN_ALL (GEN_NORMS | GEN_TANGS | GEN_UVS_POLAR)
#define GEN_COLOR 0x10
#define GEN_COLOR_RAND 0x20
#define GEN_COLOR_RAND_I 0x40
#define GEN_DEFAULT (GEN_NORMS | GEN_COLOR)

#define SCREEN_A 0
#define SCREEN_B 1
#define SCREEN_C 2
#define SCREEN_D 3
#define SCREEN_E 4

int screen_number = SCREEN_A;

//colours
glm::vec3
	WHITE                       (1, 1, 1),
	BLACK                       (0, 0, 0),
	GREY                        (.5, .5, .5),
	OFF_BLACK                   (.1, .1, .1),
	RED                         (1, 0, 0),
	SKY_BLUE					(0.0f, 0.6f, 0.8f),
	ambient_color = BLACK;

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
	current_program;

// self contained custom variable handlers
Var_Handle
	mat4_handles[3],
	light_handles[5],
	ambient_color_handle,
	texture_handle, normal_handle,
	eye_dir_handle;

//Shader paths
const char
	*vertex_shader              = "shaders/tut.vert",
	*fragment_shader            = "shaders/tut.frag",
	*vertex_shader_a            = "shaders/A.vert",
	*fragment_shader_a          = "shaders/A.frag",
	*vertex_shader_b            = "shaders/B.vert",
	*fragment_shader_b          = "shaders/B.frag",
	*vertex_shader_c            = "shaders/C.vert",
	*fragment_shader_c          = "shaders/C.frag",
	*vertex_shader_d            = "shaders/D.vert",
	*fragment_shader_d          = "shaders/D.frag",
	*vertex_shader_e            = "shaders/E.vert",
	*fragment_shader_e          = "shaders/E.frag";

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
	eye_position                = glm::vec3(0, 0, 3),
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
	speed                       = 5.0f,
	// Mouse sensitivity
	mouseSpeed                  = 0.1f,
	// Time delta
	dt                          = 0.002;
// toggles
bool 
	fps_on                      = 0,
	wire_frame                  = 0,
	spin						= 0;
int test1                       = 0;

// light object
Light lights = { glm::vec3(0,0,10),glm::vec3(1,1,1),200,0.8,100};



// object declarations
Obj tex_sphere, a_sphere, b_sphere, planet, model, bunny;
Composite_Obj rocket;



// loads an image into a gl texture
GLuint load_texture_from_image(const char *fname)
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
// converts cartesian to polar
glm::vec2 cart_polar(glm::vec3 v)
{
	float r = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	return glm::vec2(atan(v.y / v.x), acos(v.z / r));
}
// converts polar to cartesian
glm::vec3 polar_cart(float theta, float phi)
{
	return glm::vec3(cos(theta)*cos(phi), cos(theta) * sin(phi), sin(theta));
}



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

///shape generators non indexed triangles
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
	float step = 2.0f * 3.141596f / float(k);
	float c = 0.0f, s = 0.0f;
	float len = 2.0f;
	//cone
	for (float a = 0; a <= (2.0f * 3.141596f); a += step)
	{
		v.push_back(vec3());
		c = cos(a);
		s = sin(a);
		v.push_back(vec3(c, s, len));
		c = cos(a - step);
		s = sin(a - step);
		v.push_back(vec3(c, s, len));
	}
	//circle
	for (float a = 0; a <= (2.0f * 3.141596f); a += step)
	{
		c = cos(a - step);
		s = sin(a - step);
		v.push_back(vec3(c, s, len));
		c = cos(a);
		s = sin(a);
		v.push_back(vec3(c, s, len));
		v.push_back(vec3(0.0f, 0.0f, len));
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
	float step_lats = glm::radians(360.0f) / float(lats);
	float step_longs = glm::radians(360.0f) / float(longs);
	float Radius = 1., x, y, z;
	for (float a = 0; a <= glm::radians(360.0f); a += step_lats)
		for (float b = 0; b <= glm::radians(360.0f); b += step_longs)
		{
			v.push_back(polar_cart(a, b));
			v.push_back(polar_cart(a + step_lats, b));
			v.push_back(polar_cart(a + step_lats, b + step_longs));
			v.push_back(polar_cart(a + step_lats, b + step_longs));
			v.push_back(polar_cart(a, b + step_longs));
			v.push_back(polar_cart(a, b));

		}
	return v;
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
// generates normals from every triangle
std::vector<glm::vec3>			generate_normals(std::vector<glm::vec3> v)
{
	std::vector<glm::vec3> n;
	for (int i = 0; i < v.size(); i += 3)
	{
		glm::vec3 nm = glm::normalize(glm::cross(v[i + 1] - v[i], v[i + 2] - v[i]));
		for (int j = 0; j < 3; ++j)
			n.push_back(nm);
	}
	return n;
}
// generates a second normal (tangent) for every normal (used for normal mapping)
std::vector<glm::vec3>			generate_tangents(std::vector<glm::vec3> v)
{
	std::vector<glm::vec3> n;
	for (int i = 0; i < v.size(); i += 6)
	{
		glm::vec3 nm = glm::normalize(v[i + 2] - v[i]);
		for (int j = 0; j < 6; ++j)
			n.push_back(nm);
	}
	return n;
}
// generates uvs from converted carts to polar
std::vector<glm::vec2>			generate_polar_uvs(std::vector<glm::vec3> v)
{
	std::vector<vec2> uv;
	for (int i = 0; i < v.size(); i++)
		uv.push_back(cart_polar(v[i]));
	return uv;
}
std::vector<glm::vec2>			generate_sphereical_uvs(std::vector<glm::vec3> v)
{
	std::vector<vec2> uv;
	for (int i = 0; i < v.size(); i++)
	{
		uv.push_back(vec2((atan(v[i].y, v[i].x) / 3.1415926 + 1.0) * 0.5, (asin(v[i].z) / 3.1415926 + 0.5)));
	}
	return uv;
}
std::vector<glm::vec2>			generate_repeated_rect_uvs(std::vector<glm::vec3> v)
{
	std::vector<vec2> uv;
	for (int i = 0; i < v.size(); i+=6)
	{
		uv.push_back(glm::vec2(0, 0));
		uv.push_back(glm::vec2(1, 0));
		uv.push_back(glm::vec2(1, 1));
		uv.push_back(glm::vec2(1, 1));
		uv.push_back(glm::vec2(0, 1));
		uv.push_back(glm::vec2(0, 0));
	}
	return uv;
}
// generates a single color buffer
std::vector<glm::vec3>			generate_colour_buffer(glm::vec3 colour, int n)
{
	std::vector<vec3> v;
	for (int i = 0; i < n; i++)
		v.push_back(colour);
	return v;
}
// generates a random color buffer where max is the cap color
std::vector<glm::vec3>			random_colour_buffer(glm::vec3 max, int n)
{
	std::vector<vec3> v;
	for (int i = 0; i < n; i++)
		v.push_back(glm::vec3(max.x*randf(), max.y*randf(), max.z*randf()));
	return v;
}
// generates a single color buffer of random intensities
std::vector<glm::vec3>			random_intesity_colour_buffer(glm::vec3 colour, int n)
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
// creates a vector of Vertices to pass to Obj
std::vector<Vertex>				pack_object(std::vector<glm::vec3> * v, unsigned int flags, glm::vec3 color)
{
	std::vector<Vertex> object;
	std::vector<vec3> n, c, t;
	std::vector<vec2> uv;

	if (flags == NULL)
		flags = GEN_DEFAULT;

	if((flags & GEN_NORMS) == GEN_NORMS)
		n = generate_normals(*v);
	if ((flags & GEN_COLOR) == GEN_COLOR)
		c = generate_colour_buffer(color, v->size());
	if ((flags & GEN_COLOR_RAND) == GEN_COLOR_RAND)
		c = random_colour_buffer(color, v->size());
	if ((flags & GEN_COLOR_RAND_I) == GEN_COLOR_RAND_I)
		c = random_intesity_colour_buffer(color, v->size());
	if ((flags & GEN_UVS_POLAR) == GEN_UVS_POLAR)
		uv = generate_polar_uvs(*v);
	if ((flags & GEN_UVS_RECTS) == GEN_UVS_RECTS)
		uv = generate_repeated_rect_uvs(*v);
	if ((flags & GEN_UVS_SPHERE) == GEN_UVS_SPHERE)
		uv = generate_sphereical_uvs(*v);
	if ((flags & GEN_TANGS) == GEN_TANGS)
		t = generate_tangents(*v);

	for (int i = 0; i < v->size(); ++i)
	{
		Vertex vert;
		if (v->size() != 0)
			vert.position = (*v)[i];
		if (c.size() != 0)
			vert.color = c[i];
		if (n.size() != 0)
			vert.normal = n[i];
		if (uv.size() != 0)
			vert.uv = uv[i];
		if (t.size() != 0)
			vert.tangent =t[i];
		object.push_back(vert);
	}
	return object;
}
// normal packer of custom properties
std::vector<Vertex>				pack_object(std::vector<glm::vec3> * v, std::vector<glm::vec3> * c, std::vector<glm::vec3> * n, std::vector<glm::vec2> * uv, std::vector<glm::vec3> * t)
{
	std::vector<Vertex> object;
	for (int i = 0; i < v->size(); ++i)
	{
		Vertex vert;
		if(v != NULL)
			vert.position = (*v)[i];
		if (c != NULL)
			vert.color    = (*c)[i];
		if (n != NULL)
			vert.normal   = (*n)[i];
		if (uv != NULL)
			vert.uv       = (*uv)[i];
		if (t != NULL)
			vert.tangent = (*t)[i];
		object.push_back(vert);
	}
	return object;
}

// Obj code bodies
Obj::Obj(const char *filename,
	glm::vec3 c,
	Particle _p,
	glm::vec3 _rotation,
	GLfloat _theta,
	glm::vec3 _scale)
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector< glm::vec3 > vertices;
	std::vector< Vertex > o;

	tinyobj::LoadObj(shapes, materials, filename, NULL);

	for (int i = 0; i < shapes.size(); i++)
		for (int j = 0; j < shapes[i].mesh.indices.size(); j++)
			vertices.push_back(glm::vec3(
				shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3],
				shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3 + 1],
				shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3 + 2]
			));

	std::vector<Vertex> data = pack_object(&vertices, GEN_ALL | GEN_COLOR, c);

	p = _p;
	rotation = _rotation;
	theta = _theta;
	scale = _scale;

	init(&data);
}
Obj::Obj(
	const char *filename, 
	const char *texfilename, 
	const char *normfilename,
	glm::vec3 c,
	Particle _p,
	glm::vec3 _rotation,
	GLfloat _theta,
	glm::vec3 _scale)
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector< glm::vec3 > vertices;
	std::vector< Vertex > o;

	tinyobj::LoadObj(shapes, materials, filename, NULL);

	for (int i = 0; i < shapes.size(); i++)
		for (int j = 0; j < shapes[i].mesh.indices.size(); j++)
			vertices.push_back(glm::vec3(
				shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3],
				shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3 + 1],
				shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3 + 2]
			));
	
	std::vector<Vertex> data = pack_object(&vertices, GEN_ALL | GEN_COLOR, c);

	p = _p;
	rotation = _rotation;
	theta = _theta;
	scale = _scale;

	load_textures(texfilename, normfilename);
	init(&data);
}
Obj::Obj(const char *texfilename, const char *normfilename,
	std::vector<Vertex>	data,
	Particle _p,
	glm::vec3 _rotation,
	GLfloat _theta,
	glm::vec3 _scale)
{
	p = _p;
	rotation = _rotation;
	theta = _theta;
	scale = _scale;

	load_textures(texfilename, normfilename);
	init(&data);
}
void Obj::load_textures(const char *texfilename, const char *normfilename)
{
	if (texfilename != "")
		tex = load_texture_from_image(texfilename);
	if (normfilename != "")
		norm = load_texture_from_image(normfilename);
}
void Obj::init(std::vector<Vertex> * d)
{
	data_size = d->size();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, data_size * sizeof(struct Vertex), d->data(), GL_STATIC_DRAW);
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
	glVertexAttribPointer((GLuint)4, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
		(const GLvoid*)offsetof(struct Vertex, tangent));
	glEnableVertexAttribArray(4);
	glBindVertexArray(0);
	glFlush();
}
void Obj::draw(int wire_frame)
{
	glm::mat4 Model =
		glm::translate(glm::mat4(1.), p.pos) *
		glm::rotate(glm::mat4(1.), theta, rotation) *
		glm::scale(glm::mat4(1.), scale);
	mat4_handles[0].load(Model);
	mat4_handles[1].load();
	mat4_handles[2].load();

	ambient_color_handle.load();
	
	eye_dir_handle.load();

	for (Var_Handle v : light_handles)
		v.load();

	draw_array(wire_frame);
}
void Obj::draw_array(int wire_frame)
{
	load_texture_handle(&texture_handle);
	load_normal_handle(&normal_handle);

	glActiveTexture(GL_TEXTURE0 + tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glActiveTexture(GL_TEXTURE0 + norm);
	glBindTexture(GL_TEXTURE_2D, norm);

	glBindVertexArray(vao);
	glDrawArrays(wire_frame ? GL_LINE_LOOP : GL_TRIANGLES, 0, data_size);
	glBindVertexArray(0);

	glFinish();
}

Composite_Obj::Composite_Obj(Particle _p, glm::vec3 _rotation, GLfloat _theta, glm::vec3 _scale)
{
	p = _p;
	rotation = _rotation;
	theta = _theta;
	scale = _scale;
}
void Composite_Obj::draw(int wire_frame)
{
	glm::mat4 comp_Model = glm::translate(glm::mat4(1.), p.pos) * glm::rotate(glm::mat4(1.), theta, rotation) * glm::scale(glm::mat4(1.), scale);
	mat4_handles[1].load();
	mat4_handles[2].load();

	ambient_color_handle.load();

	eye_dir_handle.load();
	
	for (Var_Handle v : light_handles)
		v.load();

	for (Obj e : objects)
	{
		glm::mat4 Model = glm::translate(glm::mat4(1.), e.p.pos) * glm::rotate(glm::mat4(1.), e.theta, e.rotation) * glm::scale(glm::mat4(1.), e.scale);
		Model = comp_Model * Model;
		mat4_handles[0].load(Model);
		e.draw_array(wire_frame);
	}

}
void Composite_Obj::add(Obj e)
{
	objects.push_back(e);
}



// random functions
void			reset_rocket()
{
	rocket.p.pos = glm::vec3(0.0f, 0.0f, 2.0f);
}
void			reset_model()
{
	model.p.pos = vec3(1.0f, 0.0f, 1.5f);
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


// setup my Var_Handles that are handles to shader variables
void			setup_program_handles(GLuint prog)
{
	eye_dir_handle = Var_Handle("u_eye_pos", &eye_position);
	eye_dir_handle.init(prog);

	texture_handle = Var_Handle("u_tex");
	texture_handle.init(prog);
	normal_handle = Var_Handle("u_norm");
	normal_handle.init(prog);

	ambient_color_handle = Var_Handle("u_ambient_color", &ambient_color);
	ambient_color_handle.init(prog);

	mat4_handles[0] = Var_Handle("u_m");
	mat4_handles[1] = Var_Handle("u_v", &View);
	mat4_handles[2] = Var_Handle("u_p", &Projection);
	for (int i = 0; i < 3; ++i)
		mat4_handles[i].init(prog);

	light_handles[0] = Var_Handle("u_light_pos", &lights.pos);
	light_handles[1] = Var_Handle("u_diffuse_color", &lights.color);
	light_handles[2] = Var_Handle("u_brightness", &lights.brightness);
	light_handles[3] = Var_Handle("u_shininess", &lights.shininess);
	light_handles[4] = Var_Handle("u_specular_scale", &lights.specular_scale);

	for (int i = 0; i < 5; ++i)
		light_handles[i].init(prog);
}
//Custom graphics loop
void			loop()
{

	if (spin)
		lights.pos = glm::quat(glm::vec3(dt, dt, dt)) * lights.pos;

	switch (screen_number)
	{
	case SCREEN_A:
		a_sphere.draw(1);
		a_sphere.theta += dt;
		break;
	case SCREEN_B:
		b_sphere.draw(0);
		break;
	case SCREEN_C:
		rocket.p.pos = glm::quat(glm::vec3(0, -dt, 0)) * rocket.p.pos;
		rocket.draw(wire_frame);
		planet.draw(wire_frame);
		break;
	case SCREEN_D:
		planet.draw(0);
		break;
	case SCREEN_E:
		bunny.draw(0);
		tex_sphere.draw(0);
		model.draw(0);
		
		break;
	}
}
//Initilise custom objects
void			init_objects()
{
	// import a ship for screen E
	model = Obj(
		"objects/XL5-BASE.obj",                             //model file
		"199.bmp",				                            //texture file
		"199_norm.bmp",			                            //normal map file
		BLACK,					                            //vertex color to be used
		Particle(glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3()), //particle physics properties
		glm::vec3(1, 0, 0),                                 //rotation axis
		glm::radians(90.0f),                                //rotation amount
		glm::vec3(1, 1, 1) * 0.1f                          //scale vector
	);

	// import a bunny for screen E
	bunny = Obj(
		"objects/bunny.obj", 
		"197.bmp",				                            //texture file
		"197_norm.bmp",
		BLACK,
		Particle(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3()),
		glm::vec3(0, 1, 0),
		glm::radians(90.0f),
		glm::vec3(1, 1, 1)
	);

	// create sphere data for screen A, B and D
	std::vector<vec3> v = generate_sphere(200,200);
	std::vector<Vertex> o = pack_object(&v, GEN_ALL | GEN_COLOR | GEN_UVS_SPHERE, GREY);

	// make sphere A
	a_sphere = Obj("", "",
		o,													//object data
		Particle(glm::vec3(), glm::vec3()),
		glm::vec3(1, 0,0),
		glm::radians(90.0f),
		glm::vec3(1,1,1)
		);

	// make sphere B
	b_sphere = Obj("", "",
		o,
		Particle(glm::vec3(), glm::vec3()),
		glm::vec3(1, 0, 0),
		glm::radians(90.0f),
		glm::vec3(1, 1, 1)
	);
	
	// make sphere D (this on has a texture!)
	tex_sphere = Obj(
		"172.bmp",				                            //texture file
		"172_norm.bmp",			                            //normal map file
		o,
		Particle(glm::vec3(), glm::vec3()),
		glm::vec3(1, 0, 0),
		glm::radians(90.0f),
		glm::vec3(1, 1, 1));

	// another sphere for screen C
	planet = Obj(
		"5672_mars_4k_color.jpg",				//texture file
		"5672_mars_4k_normal.jpg",			//normal map file
		o,
		Particle(glm::vec3(0.0f, 0, 0), glm::vec3()),
		glm::vec3(1, 0, 0),
		glm::radians(90.0f),
		glm::vec3(1, 1, 1)
	);

	
	// make rocket from composite obj for screen C
	rocket.rotation = vec3(0.0f, 0.0f, 1.0f);
	rocket.theta = glm::radians(90.0f);
	rocket.scale *= 0.1f;

	Obj temp;
	// cone
	v = generate_cone(100);
	o = pack_object(&v, GEN_DEFAULT | GEN_COLOR_RAND_I, RED);
	temp = Obj("", "",
		o,
		Particle(glm::vec3(), glm::vec3()),
		glm::vec3(1, 0, 0),
		glm::radians(90.0f),
		glm::vec3(1, 1, 1)
	);
	rocket.add(temp);
	// cone2
	temp = Obj("", "",
		o,
		Particle(glm::vec3(0,-7,0), glm::vec3()),
		glm::vec3(1, 0, 0),
		glm::radians(90.0f),
		glm::vec3(1, 1, 1)
	);
	rocket.add(temp);
	// cylinder
	v = generate_cylinder(100, 5);
	o = pack_object(&v, GEN_DEFAULT | GEN_COLOR_RAND_I, RED);
	temp = Obj("", "",
		o,
		Particle(glm::vec3(0,-2,0), glm::vec3()),
		glm::vec3(1, 0, 0),
		glm::radians(90.0f),
		glm::vec3(1, 1, 1)
	);
	rocket.add(temp);
	// sphere
	v = generate_sphere(100, 100);
	o = pack_object(&v, GEN_DEFAULT | GEN_COLOR_RAND_I, RED);
	temp = Obj("", "",
		o,
		Particle(glm::vec3(0,-7,0), glm::vec3()),
		glm::vec3(1, 0, 0),
		glm::radians(0.0f),
		glm::vec3(1, 1, 1)
	);
	rocket.add(temp);	
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
			eye_position = vec3(0, 0, 3);
			eye_direction = vec3(0, 0, 0);
			lights.pos = glm::vec3(0, 0, 10);
			lights.brightness = 100;
			setup_program_handles(current_program);
			ambient_color = BLACK;
			break;
		case GLFW_KEY_B:
			screen_number = SCREEN_B;
			current_program = program_b;
			eye_position = vec3(0, 0, 3);
			eye_direction = vec3(0, 0, 0);
			lights.pos = glm::vec3(0, 0, 10);
			lights.brightness = 100;
			setup_program_handles(current_program);
			spin = 0;
			ambient_color = BLACK;
			break;
		case GLFW_KEY_C:
			screen_number = SCREEN_C;
			current_program = program_c;
			eye_position = vec3(0, 2, 5);
			eye_direction = vec3(0, 0, 0);
			lights.pos = glm::vec3(0, 0, 10);
			lights.brightness = 100;
			setup_program_handles(current_program);
			reset_rocket();
			spin = 0;
			ambient_color = OFF_BLACK;
			break;
		case GLFW_KEY_D:
			screen_number = SCREEN_D;
			current_program = program_d;
			eye_position = vec3(0, 0, 3);
			eye_direction = vec3(0, 0, 0);
			lights.pos = glm::vec3(0, 0, 10);
			lights.brightness = 100;
			setup_program_handles(current_program);
			spin = 1;
			ambient_color = BLACK;
			break;
		case GLFW_KEY_E:
			screen_number = SCREEN_E;
			current_program = program_e;
			eye_position = vec3(0, 0, 5);
			eye_direction = vec3(0, 0, 0);
			lights.pos = glm::vec3(5, 2, 10);
			lights.brightness = 100;
			spin = 1;
			setup_program_handles(current_program);
			ambient_color = SKY_BLUE / 3.0f;
			break;

			//other
		case GLFW_KEY_ESCAPE:
		case GLFW_KEY_Q:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_UP:
			eye_position += direction * (speed/20.0f);
			break;
		case GLFW_KEY_DOWN:
			eye_position -= direction * (speed/20.0f);
			break;
		case GLFW_KEY_RIGHT:
			eye_position = glm::quat(glm::vec3(0, glm::radians(speed), 0)) * eye_position;
			break;
		case GLFW_KEY_LEFT:
			eye_position = glm::quat(glm::vec3(0, -glm::radians(speed), 0)) * eye_position;
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
		case GLFW_KEY_S:
			spin = !spin;
			break;
		}
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

	// load the shader programs
	program_id = LoadShaders(vertex_shader, fragment_shader);
	program_a = LoadShaders(vertex_shader_a, fragment_shader_a);
	program_b = LoadShaders(vertex_shader_b, fragment_shader_b);
	program_c = LoadShaders(vertex_shader_c, fragment_shader_c);
	program_d = LoadShaders(vertex_shader_d, fragment_shader_d);
	program_e = LoadShaders(vertex_shader_e, fragment_shader_e);
	// set the current program
	current_program = program_a;

	// setup variable handles
	setup_program_handles(current_program);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	// enable texturing
	glEnable(GL_TEXTURE_2D);

	// init objects
	init_objects();

}
//GL graphics loop
void			glLoop(void(*graphics_loop)())
{
	//Main Loop  
	do
	{
		// start clock for this tick
		auto start = std::chrono::high_resolution_clock::now();

		// set current shader program
		glUseProgram(current_program);

		// calculate some useful vectors
		direction = -glm::normalize(eye_position);
		right = glm::normalize(glm::cross(direction, up));

		// calculate projection and view matrix
		Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.001f, 1000.0f);
		View = glm::lookAt(eye_position, eye_direction, up);

		//Clear color buffer  
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// set clear color
		glm::vec3 back_color = ambient_color * 1.0f;
		glClearColor(back_color.x, back_color.y, back_color.z,1.);

		// call the graphics loop
		graphics_loop();

		//Swap buffers  
		glfwSwapBuffers(window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();

		// stop clock
		auto finish = std::chrono::high_resolution_clock::now();
		int ms = float(std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count());
		long newWait = 5 - ms;// -(gm.gameSpeed);
		newWait = newWait < 0 ? 0 : newWait;
		// throttle the graphics loop to cap at a certain fps
		std::this_thread::sleep_for(std::chrono::milliseconds(newWait));

	} //Check if the ESC or Q key had been pressed or if the window had been closed  
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