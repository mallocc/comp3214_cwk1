#pragma once


#include "opengl.h"
#include "utils.h"

#include "tiny_obj_loader.h"

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
struct Var_Handle
{
private:
	const char * var_name;
	GLuint handle;
	glm::mat4 * data_m;
	glm::vec3 * data_v;
	GLfloat * data_f;
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
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	glm::vec3 colour = glm::vec3(0.5f, 1.0f, 0.5f);
	std::vector< glm::vec3 > colours;
	GLuint vao, buffer;
	glm::vec3
		rotation = glm::vec3(1, 0, 0),
		scale = glm::vec3(1, 1, 1);
	GLfloat theta;
	Particle p;


	Obj(const char *filename, glm::vec3 c)
	{
		vertices.clear();
		colours.clear();

		colour = c;

		tinyobj::LoadObj(shapes, materials, filename, NULL);
		for (int i = 0; i < shapes.size(); i++)
			for (int j = 0; j < shapes[i].mesh.indices.size(); j++)
				vertices.push_back(glm::vec3(
					shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3],
					shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3 + 1],
					shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3 + 2]
				));
		for (int i = 0; i < vertices.size(); i++)
			colours.push_back(glm::vec3(randf(), randf(),randf()));

		//readObj(filename, &vertices, &uvs, &normals);
	}

	bool readObj(
		const char *path,
		std::vector< glm::vec3 > * out_vertices,
		std::vector< glm::vec2 > * out_uvs,
		std::vector< glm::vec3 > * out_normals)
	{

		std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		std::vector< glm::vec3 > temp_vertices;
		std::vector< glm::vec2 > temp_uvs;
		std::vector< glm::vec3 > temp_normals;

		FILE * file = fopen(path, "r");
		if (file == NULL) {
			printf("Impossible to open the file !\n");
			return false;
		}
		while (1) {

			char lineHeader[128];
			// read the first word of the line
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.

					   // else : parse lineHeader
			if (strcmp(lineHeader, "v") == 0) {
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0) {
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				temp_uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0) {
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					break;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
		}
		std::cout << vertexIndices.size() << std::endl;
		// For each vertex of each triangle
		for (unsigned int i = 0; i < vertexIndices.size(); i++)
			out_vertices->push_back(temp_vertices[vertexIndices[i] - 1]);
		for (unsigned int i = 0; i < uvIndices.size(); i++)
			out_uvs->push_back(temp_uvs[uvIndices[i] - 1]);
		for (unsigned int i = 0; i < normalIndices.size(); i++)
			out_normals->push_back(temp_normals[normalIndices[i] - 1]);
		std::cout << "test" << std::endl;
		return true;
	}

	void init();

	void draw();
};