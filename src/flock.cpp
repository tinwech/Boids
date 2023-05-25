#include "flock.h"
#include <iostream>

Flock::Flock(int n_boids, float sx, float sy, float sz) : 
    n_boids(n_boids),
    x_min(-sx / 2),
    x_max(sx / 2),
    y_min(-sy / 2),
    y_max(sy / 2),
    z_min(-sz / 2),
    z_max(sz / 2) {
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

void Flock::update(float deltaTime, Prey *prey) {
    for (int i = 0; i < n_boids; i++) {
        float n_neighbors = 0;
        glm::vec3 s_vel = glm::vec3(0, 0, 0);
        glm::vec3 a_vel = glm::vec3(0, 0, 0);
        glm::vec3 c_vel = glm::vec3(0, 0, 0);
        glm::vec3 avoid_vel = glm::vec3(0, 0, 0);
        glm::vec3 avg_vel = glm::vec3(0, 0, 0);
        glm::vec3 avg_pos = glm::vec3(0, 0, 0);
        for (int j = 0; j < n_boids; j++) {
            // boids[i]: center boid
            // boids[j]: neighbor boid
            if (i == j) {
                continue;
            }
            glm::vec3 v = boids[j]->pos - boids[i]->pos;
            float distance = glm::length(v);
            if (distance < visualRange 
                && glm::dot(glm::normalize(boids[i]->vel), glm::normalize(v)) > -0.5) {
                n_neighbors += 1;
                // separation
                avoid_vel += glm::normalize(boids[i]->pos - boids[j]->pos) / distance;
                // alignment
                avg_vel += boids[j]->vel;
                // cohesion
                avg_pos += boids[j]->pos;
            } 
        }
        if (n_neighbors > 0) {
            // separation
            s_vel = avoid_vel / n_neighbors;
            // alignment
            avg_vel /= n_neighbors;
            a_vel = avg_vel - boids[i]->vel;
            // cohesion
            avg_pos /= n_neighbors;
            c_vel = avg_pos - boids[i]->pos;
        }

        boids[i]->vel += separation * s_vel + alignment * a_vel + cohesion * c_vel;

        // chase prey
        if (prey->alive) {
            glm::vec3 v = prey->pos - boids[i]->pos;
            float distance = glm::length(v);
            if (distance < 3) {
                prey->alive = false;
            }
            boids[i]->vel += chasefactor * glm::normalize(v) / distance;
        }

        // check border
        checkBorder(boids[i]);

        // update vel and pos
        if (glm::length(boids[i]->vel) < minSpeed) {
            boids[i]->vel = glm::normalize(boids[i]->vel) * minSpeed;
        }
        else if (glm::length(boids[i]->vel) > maxSpeed) {
            boids[i]->vel = glm::normalize(boids[i]->vel) * maxSpeed;
        }
        boids[i]->pos += deltaTime * boids[i]->vel;
    }
}