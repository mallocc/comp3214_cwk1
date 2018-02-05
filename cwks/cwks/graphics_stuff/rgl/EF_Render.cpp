#include <RGL.h>
#include <EF_Render.h>
class EDGE {
public:
int X, Y, dX, dY;
EDGE(int x0, int y0, int x1, int y1){
  X = x0;
  Y = y0;
  dX = x1 - X;
  dY = y1 - Y;
}
int eval(int i, int j){
  return (i - X) * dY - (j - Y) * dX;
}
};

bool sorty(std::valarray<float> a, std::valarray<float> b) {
  return a[1] > b[1];
}
void printv(char const *t, std::valarray<float> a) {
  printf("V: %20s:", t);
  for(int i = 0; i < a.size(); i++) {
    printf(" %8.1f", a[i]);
  }
  printf(";\n");
}
int EF_Render::add(vector<glm::vec4> points) {     // adds a vector of vertices
  int newObjects = points.size();
  for(int i = 0; i < newObjects; i++)
    EF_Render::in_vertices.push_back(points[i]);
  return EF_Render::in_vertices.size();
}
int EF_Render::add(glm::vec4 point)  {    // adds a single vertices
  EF_Render::in_vertices.push_back(point);
  return EF_Render::in_vertices.size();
}
int EF_Render::size(void) {     // returns the size.
  return EF_Render::in_vertices.size();
}
int EF_Render::remove(int n) {     // removes one or more vertices
  int objects = EF_Render::in_vertices.size();
  assert(n > 0);
  if(n >= objects)
  {
    EF_Render::in_vertices.clear();
  }
  else
  {
    for(int i = 0; i < n; i++)
      EF_Render::in_vertices.pop_back();
  }
  return EF_Render::in_vertices.size();
}
void EF_Render::vertex(glm::vec4 point) {     // this constructs the geometry data
  glm::vec4 t;
  t = Transform * point;
  t[0] = t[0] / t[3];     // homogenous normalization
  t[1] = t[1] / t[3];
  t[2] = t[2] / t[3];
  t[3] = 1.0;
  t = RGL::_workstation(t);

  std::valarray<float> vv = {t[0], t[1], t[2], t[3]};        //x, y, z, w, b brings all the attributes togethrer for rendering.

  EF_Render::vertices.push_back(vv);
}
void EF_Render::drawEFTriangle(std::valarray<float> v1, std::valarray<float> v2, std::valarray<float> v3) {
  assert(v1.size() == v2.size() && v2.size() == v3.size());
  assert(v1.size() >= 4);
  assert(v2.size() >= 4);
  assert(v3.size() >= 4);

  std::vector<std::valarray<float> > co = {v1, v2, v3};
  std::sort(co.begin(), co.end(), sorty);

  EDGE e0(co[0][0], co[0][1], co[1][0], co[1][1]);
  EDGE e1(co[1][0], co[1][1], co[2][0], co[2][1]);
  EDGE e2(co[2][0], co[2][1], co[0][0], co[0][1]);

  for(int n = 0; n < WIDTH; n++) {
    for(int m = 0; m < HEIGHT; m++) {
      int e = e0.eval(n, m);
      int f = e1.eval(n, m);
      int g = e2.eval(n, m);
      if(e < 0 && f < 0 && g < 0)
        RGL::SetPixel(n, m, RED);
      else
        RGL::SetPixel(n, m, BLUE);
    }
  }


}


int EF_Render::draw()
{
  int objects = in_vertices.size();
  /*
   * Convert vertices to screen coordiates.
   */
  EF_Render::vertices.clear();       // need this otherwise draw previous values as well
  for(int i = 0; i < objects; i++)
  {
    EF_Render::vertex(EF_Render::in_vertices[i]);
  }
  /*
   * Render every 3 as a triangle..
   */
  for(int i = 0; i < objects; i += 3)
  {
    EF_Render::drawEFTriangle(EF_Render::vertices[i], EF_Render::vertices[i + 1], EF_Render::vertices[i + 2]);
  }
}
