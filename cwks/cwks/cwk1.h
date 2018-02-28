#pragma once


#include "opengl.h"
#include "utils.h"

#include "tiny_object_loader/tiny_obj_loader.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h> 
#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>


int cwk1_main();

//Returns random float
inline float		randf()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}





//// STUCTS ////

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct Particle
{
	glm::vec3 
		pos, 
		vel;
	float 
		life_time=1, 
		life=life_time;
	int 
		mortal = 0;

	Particle() {}

	Particle(glm::vec3 p, glm::vec3 v)
	{
		pos = p;
		vel = v;
	}

	void set_life_time(int l)
	{
		life_time = life = l;
		mortal = 1;
	}

	void update(float dt)
	{
		pos += vel * dt;
		life -= mortal;
	}
};

struct Entity
{
private:
	GLuint vao, buffer;

public:
	std::vector<Vertex> data;
	glm::vec3 
		rotation = glm::vec3(1, 0, 0), 
		scale = glm::vec3(1, 1, 1);
	GLfloat theta;
	Particle p;

	Entity() {}
	Entity(
		std::vector<Vertex> _data, 
		Particle _p, 
		glm::vec3 _rotation, GLfloat _theta, 
		glm::vec3 _scale)
	{
		data = _data;
		p = _p;
		rotation = _rotation;
		theta = _theta;
		scale = _scale;
	}

	void init();
	void draw_array(int wire_frame);
	void draw(int wire_frame);
};

struct Composite_Entity
{
	std::vector<Entity> entities;
	glm::vec3 
		rotation = glm::vec3(1, 0, 0), 
		scale = glm::vec3(1, 1, 1);
	GLfloat theta;
	Particle p;

	Composite_Entity() {};

	void init();
	void draw(int wire_frame);
	void add(Entity e);

};

struct Light
{
	glm::vec3 pos;
	glm::vec3 color;
	float brightness;
	float specular_scale;
	float shininess;
};


#define MAT4_HANDLE 0
#define VEC3_HANDLE 1
#define FLOAT_HANDLE 2
#define INTEGER_HANDLE 3
struct Var_Handle
{
private:
	const char * var_name;
	GLuint handle;
	glm::mat4 * data_m;
	glm::vec3 * data_v;
	GLfloat * data_f;
	GLuint * data_i;
	int handle_type = FLOAT_HANDLE;

public:

	Var_Handle() {}
	Var_Handle(const char * var_name_)
	{
		var_name = var_name_;
	}
	Var_Handle(const char * var_name_, glm::mat4 * data)
	{
		var_name = var_name_;
		data_m = data;
		handle_type = MAT4_HANDLE;
	}
	Var_Handle(const char * var_name_, glm::vec3 * data)
	{
		var_name = var_name_;
		data_v = data;
		handle_type = VEC3_HANDLE;
	}
	Var_Handle(const char * var_name_, GLfloat * data)
	{
		var_name = var_name_;
		data_f = data;
		handle_type = FLOAT_HANDLE;
	}
	Var_Handle(const char * var_name_, GLuint * data)
	{
		var_name = var_name_;
		data_i = data;
		handle_type = INTEGER_HANDLE;
	}

	void init(GLuint program)
	{
		handle = glGetUniformLocation(program, var_name);
	}

	void load()
	{
		if (handle_type == MAT4_HANDLE)
		{
			glm::mat4 mat = *data_m;
			glUniformMatrix4fv(handle, 1, GL_FALSE, &mat[0][0]);
		}
		else if (handle_type == VEC3_HANDLE)
			glUniform3f(handle, data_v->x, data_v->y, data_v->z);
		else if (handle_type == FLOAT_HANDLE)
			glUniform1f(handle, *data_f);
		else if (handle_type == INTEGER_HANDLE)
			glUniform1i(handle, *data_i);
	}
	void load(glm::mat4 data)
	{
		glUniformMatrix4fv(handle, 1, GL_FALSE, &data[0][0]);
	}
	void load(glm::vec3 data)
	{
		glUniform3f(handle, data.x, data.y, data.z);
	}
	void load(GLfloat data)
	{
		glUniform1f(handle, data);
	}
	void load(GLuint data)
	{
		glUniform1i(handle, data);
	}

	GLuint get_handle_id()
	{
		return handle;
	}

	const char * get_handle_name()
	{
		return var_name;
	}
};

struct Obj
{
private:
	GLuint vao, buffer, tex;

public:
	std::vector<Vertex> data;
	glm::vec3
		rotation = glm::vec3(1, 0, 0),
		scale = glm::vec3(1, 1, 1);
	GLfloat theta;
	Particle p;

	Obj(const char *filename, 
		glm::vec3 c);
	Obj(const char *filename, 
		glm::vec3 c, 
		Particle _p,
		glm::vec3 _rotation, 
		GLfloat _theta,
		glm::vec3 _scale);
	Obj(
		std::vector<Vertex>	_data,
		Particle _p,
		glm::vec3 _rotation, 
		GLfloat _theta,
		glm::vec3 _scale);

	void init(const char *texfilename);
	void draw();

	void loadTexturehandle(Var_Handle * handle)
	{
		handle->load(tex);
	}
};