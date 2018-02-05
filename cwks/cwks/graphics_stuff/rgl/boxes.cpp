#include "RGL.h"
void RGL::FilledBox(int wxl, int wxr, int wyb, int wyt, Uint32 colour){
  wxl = std::max(0, wxl);
  wxr = std::min(WIDTH - 1, wxr);
  wyb = std::max(0, wyb);
  wyt = std::min(HEIGHT - 1, wyt);
  for(int i = wyb; i < wyt; i++)
    RGL::Line(wxl, i, wxr, i, colour);
}

void Box(int wxl, int wxr, int wyb, int wyt, Uint32 colour){
  RGL::Line(wxl, wyt, wxr, wyt, colour);
  RGL::Line(wxl, wyb, wxr, wyb, colour);
  RGL::Line(wxr, wyb, wxr, wyt, colour);
  RGL::Line(wxl, wyb, wxl, wyt, colour);
}

void Block(glm::vec2 p, glm::vec3 c){
  int r = (int) c[0];
  int g = (int) c[1];
  int b = (int) c[2];
  Uint32 cc  = RGL::Colour(r, g, b);
  RGL::FilledBox((int) p[0] - 4,(int) p[0] + 4,(int) p[1] - 4,(int) p[1] + 4, cc);
}
