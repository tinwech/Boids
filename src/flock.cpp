#include "flock.h"
#include <iostream>

Flock::Flock(int n_boids, float sx, float sy, float sz) : n_boids(n_boids) {
    boids.reserve(n_boids);
    for (int i = 0; i < n_boids; i++) {
        float x = rand() % (int)sx - sx / 2;
        float y = rand() % (int)sy - sy / 2;
        float z = rand() % (int)sz - sz / 2;
        boids.emplace_back(new Boid(x, y, z));
    }
}

void Flock::update() {
    for (int i = 0; i < n_boids; i++) {
        boids[i]->pos += dt * boids[i]->vel;
        // for each neighboring boid:
            // alignment
            // separation
            // cohesion
        // check border
        // update vel and pos
        // update translation and rotation
    }
}