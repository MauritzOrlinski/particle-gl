#include "particles.h"
#include "geometry.h"
#include "window.h"
#include <GL/gl.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#define DAMPF 0.8
#define THRESHOLD 0.05
#define G -9.81
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

particle *helper;
float max_dist;

int init(particle *arr, size_t n) {
  helper = (particle *)malloc(sizeof(particle) * n);
  if (!helper) {
    return -1; // unsuccessfull
  }
  max_dist = 0;
  for (size_t i = 0; i < n; i++) {
    if (arr[i].r * 2 > max_dist) {
      max_dist = arr[i].r * 2;
    }
  }
  return 0;
}

void paint_particles(particle *particles, size_t n) {
  glClear(GL_COLOR_BUFFER_BIT);

  for (size_t i = 0; i < n; ++i) {
    draw_circle(particles[i].x, particles[i].y, particles[i].r);
  }

  glFlush();
}

particle *merge(particle *arr, size_t l, size_t m, size_t u) {
  size_t p1 = l;
  size_t p2 = m + 1;
  size_t c = 0;
  while (p1 <= m && p2 <= u) {
    if (arr[p1].x <= arr[p2].x) {
      helper[c++] = arr[p1++];
    } else {
      helper[c++] = arr[p2++];
    }
  }
  while (p1 <= m) {
    helper[c++] = arr[p1++];
  }
  while (p2 <= u) {
    helper[c++] = arr[p2++];
  }
  for (size_t i = 0; i < c; i++) {
    arr[l + i] = helper[i];
  }
  return arr;
}

particle *merge_sort(particle *arr, size_t l, size_t u) {
  if (u - l <= 1) {
    return arr;
  }
  size_t m = (l + u) / 2;
  merge_sort(arr, l, m);
  merge_sort(arr, m + 1, u);
  merge(arr, l, m, u);
  return arr;
}

particle *sort_particle_array(particle *arr, size_t n) {
  // merge sort with x coordinate
  merge_sort(arr, 0, n - 1);
  return arr;
}

void update_particles(particle *particles, size_t n, float dt, WindowDescr *w) {
  for (size_t i = 0; i < n; i++) {
    for (size_t j = i + 1; j < n && particles[j].x - particles[i].x < max_dist;
         j++) {
      float min_dist = particles[i].r + particles[j].r;
      if (likely(fabs(particles[i].x - particles[j].x) > min_dist ||
                 fabs(particles[i].y - particles[j].y) > min_dist)) {
        continue;
      }
      float distance =
          dist(particles[i].x, particles[i].y, particles[j].x, particles[j].y);
      if (likely(distance <= min_dist)) {
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

  for (size_t i = 0; i < n; i++) {
    update_particle(&particles[i], dt, w);
  }
  sort_particle_array(particles, n);
}

void update_particle(particle *p, float dt, WindowDescr *window) {
  p->x += p->vx * dt;
  p->y += p->vy * dt;
  // p->vy += G * dt;
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
