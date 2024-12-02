#include "geometry.h"
#include <math.h>
#include <stddef.h>
#define PI 3.1415926535f

void draw_circle(float cx, float cy, float r) {
  glBegin(GL_POINTS);
  glVertex2f(cx, cy);
  size_t n = 2 * PI * r * 5 + 1;
  for (size_t i = 0; i < n; i++) {
    glVertex2f(cx + (r * cos(i * (2.0f * PI) / n)),
               cy + (r * sin(i * (2.0f * PI) / n)));
  }
  glEnd();
}

float fast_sqrt(float x) {
  union {
    float f;
    uint32_t i;
  } conv = {.f = x};
  conv.i = 0x5f3759df - (conv.i >> 1);
  // conv.f *= 1.5F - (x * 0.5F * conv.f * conv.f);
  return 1 / conv.f;
}

float dist(float x1, float y1, float x2, float y2) {
  float x_diff = x1 - x2;
  float y_diff = y1 - y2;
  return fast_sqrt(x_diff * x_diff + y_diff * y_diff);
}
