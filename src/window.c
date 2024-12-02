#include "window.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void initiate_window(WindowDescr *w) {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glColor3f(1.0, 1.0, 1.0);

  glPointSize(1);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(w->width / -2.0f, w->width / 2.0f, w->height / -2.0f,
             w->height / 2.0f);
}

WindowDescr *new_window_description(uint32_t width, uint32_t height) {
  WindowDescr *p = (WindowDescr *)malloc(sizeof(WindowDescr));
  if (!p) {
    return NULL;
  }
  p->width = width;
  p->height = height;

  return p;
}
