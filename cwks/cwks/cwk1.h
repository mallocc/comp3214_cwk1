#pragma once


#include "opengl.h"
#include "utils.h"

#include "tiny_object_loader\tiny_obj_loader.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h> 
#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>

int cwk1_main();



//// STUCTS ////

struct Particle
{

	glm::vec3 pos, vel;

	Particle() {}

	Particle(glm::vec3 p, glm::vec3 v);

	void update(float dt);
};



//Holds buffers, array object and data in one structure.
//Able to draw itself and initilise buffers.
struct Entity
{
private:
	GLuint vert_b, colour_b, vao;

public:
	GLfloat * v_b, *c_b;
	int n;
	Particle p;
	bool is_static;

	Entity() {}

	Entity(GLfloat * v, GLfloat * c, int _n);

	void init();

	void draw();
};


//Returns random float
inline float		randf()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
