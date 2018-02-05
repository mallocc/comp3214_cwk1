#ifndef SHADER_RENDER_H_INCLUDED
#define SHADER_RENDER_H_INCLUDED

#include <RGL.h>
using namespace std;

class EF_Render
{
private:
public:
vector<glm::vec4> in_vertices;     //temporery store for vertices
vector<std::valarray<float> > vertices;     // vertices to render
glm::mat4 Transform = glm::mat4(1.);
bool flat;

inline virtual void fragment(std::valarray<float> p) {
//defined as virual so it can be oerridden in derived class, allows drawing code to be customized.
  int i = (int) p[0];
  int j = (int) p[1];
  RGL::SetPixel(i, j, RGL::World.ObjectColour);
}

void drawEFTriangle(std::valarray<float> v1, std::valarray<float> v2, std::valarray<float> v3);

int add(vector<glm::vec4> points);
int add(glm::vec4 point);
int size(void);
int remove(int n = 1);
void vertex(glm::vec4 point);
int draw();

}; // end of class render



#endif // SHADER_RENDER_H_INCLUDED
