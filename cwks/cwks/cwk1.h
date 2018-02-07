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
struct Entity
{
private:
	GLuint vert_b, colour_b, vao;

public:
	GLfloat * v_b, *c_b;
	int n;

	Entity() {}

	Entity(GLfloat * v, GLfloat * c, int _n);

	void init();

	void draw();
};



inline float		randf()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
