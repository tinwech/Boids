#include "flock.h"
#include <iostream>

Flock::Flock(int n_boids, float sx, float sy, float sz) : n_boids(n_boids) {
    x_min = -sx / 2;
    x_max = sx / 2;
    y_min = -sy / 2;
    y_max = sy / 2;
    z_min = -sz / 2;
    z_max = sz / 2;
    boids.reserve(n_boids);
    for (int i = 0; i < n_boids; i++) {
        float x = rand() % (int)sx - sx / 2;
        float y = rand() % (int)sy - sy / 2;
        float z = rand() % (int)sz - sz / 2;
        boids.emplace_back(new Boid(x, y, z));
    }
}

void Flock::checkBorder(Boid *boid) {
    if (boid->pos[0] < x_min) {
        boid->vel[0] += turnfactor;
    }
    if (boid->pos[1] < y_min) {
        boid->vel[1] += turnfactor;
    }
    if (boid->pos[2] < z_min) {
        boid->vel[2] += turnfactor;
    }
    if (boid->pos[0] > x_max) {
        boid->vel[0] -= turnfactor;
    }
    if (boid->pos[1] > y_max) {
        boid->vel[1] -= turnfactor;
    }
    if (boid->pos[2] > z_max) {
        boid->vel[2] -= turnfactor;
    }
}

void Flock::update() {
    for (int i = 0; i < n_boids; i++) {
        // alignment
        // separation
        // cohesion
        // check border
        checkBorder(boids[i]);
        // update vel and pos
        boids[i]->pos += dt * boids[i]->vel;
    }
}