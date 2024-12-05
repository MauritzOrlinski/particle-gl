#include "particles.h"
#include "window.h"
#include <GL/freeglut_std.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#define N 1000000
#define SLEEP_TIME_MS 0
#define DT 0.1f
#define H 1500
#define W 2500
#define MAX_SPEED 20

particle *particles;
WindowDescr w;

float randomf(void) { return rand() / (float)RAND_MAX; }
struct timespec start, end;

void display(void) {
  clock_gettime(CLOCK_MONOTONIC, &start);
  update_particles(particles, N, DT, &w);
  clock_gettime(CLOCK_MONOTONIC, &end);
  double time_taken;
  time_taken = (end.tv_sec - start.tv_sec) * 1e9;
  time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
  printf("%f\n", time_taken);
  paint_particles(particles, N);
  usleep(1000 * SLEEP_TIME_MS);
}

int main(int argc, char **argv) {
  srand(time(NULL));
  particles = malloc(sizeof(particle) * N);
  w = *new_window_description(W, H);

  for (size_t i = 0; i < N; i++) {
    float weight = 10.0f + 10.0f * randomf();
    particles[i] = *new_particle(
        randomf() * W - W / 2.0f, randomf() * H - H / 2.0f,
        MAX_SPEED * 2 * (randomf() - 0.5f), MAX_SPEED * 2 * (randomf() - 0.5f),
        sqrt(weight / (4.0f * M_PI)), weight);
  }
  init(particles, N);
  sort_particle_array(particles, N);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  glutInitWindowSize(W, H);
  glutInitWindowPosition(0, 0);

  glutCreateWindow("particle-sim");

  initiate_window(&w);

  glutIdleFunc(display);
  glutDisplayFunc(display);
  glutMainLoop();
}
