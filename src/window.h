#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glut.h>
#include <stdint.h>

typedef struct {
  uint32_t width;
  uint32_t height;
} WindowDescr;

void initiate_window(WindowDescr *);
WindowDescr *new_window_description(uint32_t width, uint32_t height);

#endif // !WINDOW_H
