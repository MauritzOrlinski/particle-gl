# ParticleGL

> A particle simulation written in C using OpenGL for graphics

## Summary

I used this project to get first experience in graphics programming.

## How to run it

To run this project you need OpenGL, on Linux you can do this by installing the **freeglut** Library.

### Fedora

Just run the following command to install freeglut on fedora

```
sudo dnf install freeglut
sudo yum install freeglut-devel
```

## Physics

I'm using Newtonian mechanics for computing the next position based on the current state.
For collision handling I'm using elastic 2D-Collision handling (check for more information:
[Wikipedia](https://en.wikipedia.org/wiki/Elastic_collision))

## Algorithmic optimisations

The usual collision detection/handling is in $\theta(n^2)$, that is pretty bad and doesn't scale well. To make it run faster I decided to Sort the array of points before doing collision detection in respect to the x-coords, and only test the points that could be colliding (based on the x-axis). This leads to a huge reduction of constant factors in the run time making it run up to 1.000.000 Particles in acceptable speed.
By switching to more efficient data structures like KD-Trees I could archive an even bigger speed-off.

```

```
