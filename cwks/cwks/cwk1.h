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


struct Entity
{
private:
	GLuint vert_b, colour_b, normal_b, vao;

public:
	glm::vec3 * v_b, * c_b, *n_b;
	int n;
	Particle p;
	bool is_static;

	Entity() {}

	Entity(glm::vec3 * v, glm::vec3 * c, int _n);

	void init();

	void draw();
};

//Returns random float
inline float		randf()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

struct gl_LightSourceParameters
{
	glm::vec4 ambient;              // Aclarri   
	glm::vec4 diffuse;              // Dcli   
	glm::vec4 specular;             // Scli   
	glm::vec4 position;             // Ppli   
	glm::vec4 halfVector;           // Derived: Hi   
	glm::vec3 spotDirection;        // Sdli   
	float spotExponent;        // Srli   
	float spotCutoff;          // Crli                              
							   // (range: [0.0,90.0], 180.0)   
	float spotCosCutoff;       // Derived: cos(Crli)                 
							   // (range: [1.0,0.0],-1.0)   
	float constantAttenuation; // K0   
	float linearAttenuation;   // K1   
	float quadraticAttenuation;// K2  
};

