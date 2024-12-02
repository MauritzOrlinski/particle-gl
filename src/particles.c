#include "particles.h"
#include "geometry.h"
#include "window.h"
#include <GL/gl.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#define DAMPF 0.8
#define THRESHOLD 0.95
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

void paint_particles(particle *particles, size_t n) {
  glClear(GL_COLOR_BUFFER_BIT);

  for (size_t i = 0; i < n; ++i) {
    draw_circle(particles[i].x, particles[i].y, particles[i].r);
  }

  glFlush();
}

void handle_collisions(particle *particles, size_t n) {
  for (size_t i = 0; i < n; i++) {
    for (size_t j = i + 1; j < n; j++) {
      float min_dist = particles[i].r + particles[j].r;
      if (likely(fabs(particles[i].x - particles[j].x) > min_dist ||
                 fabs(particles[i].y - particles[j].y) > min_dist)) {
        continue;
      }
      float distance =
          dist(particles[i].x, particles[i].y, particles[j].x, particles[j].y);
      if (distance <= min_dist) {
        float mass_sum = particles[i].m + particles[j].m;

        float sub_res1 = ((particles[i].vx - particles[j].vx) *
                              (particles[i].x - particles[j].x) +
                          (particles[i].vy - particles[j].vy) *
                              (particles[i].y - particles[j].y)) /
                         (distance * distance);
        float sub_res2 = ((particles[j].vx - particles[i].vx) *
                              (particles[j].x - particles[i].x) +
                          (particles[j].vy - particles[i].vy) *
                              (particles[j].y - particles[i].y)) /
                         (distance * distance);
        float new_vx1 =
            particles[i].vx - (particles[i].x - particles[j].x) * sub_res1 *
                                  (2.0f * particles[j].m / mass_sum);
        float new_vy1 =
            particles[i].vy - (particles[i].y - particles[j].y) * sub_res1 *
                                  (2.0f * particles[j].m / mass_sum);
        float new_vx2 =
            particles[j].vx - (particles[j].x - particles[i].x) * sub_res2 *
                                  (2.0f * particles[i].m / mass_sum);
        float new_vy2 =
            particles[j].vy - (particles[j].y - particles[i].y) * sub_res2 *
                                  (2.0f * particles[i].m / mass_sum);
        particles[i].vx = new_vx1 * DAMPF;
        particles[i].vy = new_vy1 * DAMPF;
        particles[j].vx = new_vx2 * DAMPF;
        particles[j].vy = new_vy2 * DAMPF;

        float shift = 1 - distance / (min_dist);

        particles[i].x -= (particles[j].x - particles[i].x) * shift;
        particles[i].y -= (particles[j].y - particles[i].y) * shift;
        particles[j].x -= (particles[i].x - particles[j].x) * shift;
        particles[j].y -= (particles[i].y - particles[j].y) * shift;
      }
    }
  }
}

void update_particle(particle *p, float dt, WindowDescr *window) {
  p->x += p->vx * dt;
  p->y += p->vy * dt;
  if (p->x < window->width / -2.0f || p->x > window->width / 2.0f) {
    p->x = window->width / 2.0f * (p->x < 0 ? -1 : 1);
    p->vx *= -1 * DAMPF;
  }
  if (p->y < window->height / -2.0f || p->y > window->height / 2.0f) {
    p->y = window->height / 2.0f * (p->y < 0 ? -1 : 1);
    p->vy *= -1 * DAMPF;
  }
}

particle *new_particle(float init_x, float init_y, float init_vx, float init_vy,
                       float r, float m) {
  particle *p = (particle *)malloc(sizeof(particle));
  if (!p) {
    return NULL;
  }
  p->x = init_x;
  p->y = init_y;
  p->vx = init_vx;
  p->vy = init_vy;
  p->r = r;
  p->m = m;

  return p;
}
