#ifndef RGL_H_INCLUDED
#define RGL_H_INCLUDED

/*!

   RGL, stands for R Graphics Library where R is Rendering Class.

 */

/*
    Include Files
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <valarray>
#include <algorithm>

#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef SDL12
#include <SDL/SDL.h>
#endif // SDL12

#ifdef SDL20
#include <SDL2/SDL.h>
#endif // SDL 2.x

#define WHITE (RGL::Colour(255,255,255))
#define BLACK (RGL::Colour(0,0,0))
#define RED (RGL::Colour(255,0,0))
#define GREEN (RGL::Colour(0,255,0))
#define BLUE (RGL::Colour(0,0,255))
#define MAGENTA (RGL::Colour(255,255,0))
#define CYAN (RGL::Colour(255,0,255))
#define YELLOW (RGL::Colour(0,255,255))

/*
 * rgba colours as 4 element float vector
 */

#define hWHITE (glm::vec4(1.0f,1.0f,1.0f,1.0f))
#define hBLACK (glm::vec4(0.0f,0.0f,0.0f,1.0f))
#define hRED (glm::vec4(1.0f,0.0f,0.0f,1.0f))
#define hGREEN (glm::vec4(0.0f,1.0f,0.0f,1.0f))
#define hBLUE (glm::vec4(0.0f,0.0f,1.0f,1.0f))
#define hMAGENTA (glm::vec4(1.0f,1.0f,0.0f,1.0f))
#define hCYAN (glm::vec4(1.0f,0.0f,1.0f,1.0f))
#define hYELLOW (glm::vec4(0.0f,1.0f,1.0f,1.0f))


/*
    Default Screen Size, should use const int.
 */

#ifndef HEIGHT
#define HEIGHT 480
#endif
#ifndef WIDTH
#define WIDTH 640
#endif

#define FALSE 0
#define TRUE 1
#define Nothing 0;

#include "parameters.h"

namespace RGL //rgl2d interface to SDL and the windowing system.
{
extern Values World;
glm::vec4 _workstation(glm::vec4 a);

#ifdef SDL12

extern SDL_Surface *screen;

void _SET32(int, int, uint32_t);
void _SET16(int, int, uint32_t);
void _Set(int, int, uint32_t);

#endif // SDL12

#ifdef SDL20

extern SDL_Window *screen;     /* global variable for the screen, bad programming practice */
extern SDL_Renderer *renderer;

    #define _Set(x, y, z) SetPixel(x, y, z)
    #define _Set32(x, y, z) SetPixel(x, y, z)
    #define _Set16(x, y, z) SetPixel(x, y, z)

#endif // SDL 2.x

void StartGraphics(char *title);
void StopGraphics(int wait);
bool Animate(float * time);

void SetUp(void);
void Default(void);

void SetPixel(int, int, uint32_t);
void SetPixelRGB(int, int, int, int, int);
void _Lock(void);
void _UnLock(void);
void Lock(void);
void UnLock(void);
void SetPixelRGB(int x, int y, int r, int g, int b);
void SetPixel(int x, int y, uint32_t pixel);
void ClearScreen(void);
void Flip(void);
void UpdateScreen(void);
void UpdateWindow(int x, int y, int wx, int wy);

void FilledBox(int wxl, int wxr, int wyb, int wyt, uint32_t colour);
void Box(int wxl, int wxr, int wyb, int wyt, uint32_t colour);
void Block(glm::vec2 p, glm::vec3 c);
void Line(int x0, int y0, int x1, int y1, uint32_t colour);   //draw a real line
glm::vec2 interpolatedPosition(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t);




uint32_t Colour(int r, int g, int b);
uint32_t Colour(float r, float g, float b);
uint32_t Colour(glm:: vec4 cl);
uint32_t Colour(glm:: vec3 cl);

float GetTicks(void);

void dump(void); //Dump current screen to bmp file
} // End of 'RGL'



namespace HELP {
/*
    Helper routines
 */
void print(glm::vec2 x);
void print(glm::mat2 x);
void print(glm::vec3 x);
void print(glm::mat3 x);
void print(glm::vec4 x);
void print(glm::mat4 x);
void print(const char *x);
void print(float x);
void Print(const char *x);
void Print(float x);
void print(int x);
} // End of 'help'


#endif // RGL_H_INCLUDED
