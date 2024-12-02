#ifndef PARTICLES_H
#define PARTICLES_H

#include "window.h"
#include <GL/glut.h>
#include <stddef.h>

typedef struct {
  float x;
  float y;
  float vx;
  float vy;
  float r;
  float m;
} particle;

void paint_particles(particle *, size_t);
void update_particle(particle *, float, WindowDescr *);
particle *new_particle(float, float, float, float, float, float);
void handle_collisions(particle *, size_t);
#endif // !PARTICLES_H
