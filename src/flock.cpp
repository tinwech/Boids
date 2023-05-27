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

void Flock::chasePrey(Boid* boid, Prey* prey) {
    if (prey->alive || prey->immortal) {
        glm::vec3 v = prey->pos - boid->pos;
        float distance = glm::length(v);
        if (distance < 3) {
            prey->alive = false;
        }
        boid->vel += chasefactor * glm::normalize(v) / distance;
    }
}

void Flock::avoidObstacles(Boid *boid, std::vector<Obstacle*> &obstacles) {
    glm::vec3 avoid_vel = glm::vec3(0.0f, 0.0f, 0.0f);
    float n_visible = 0;
    for (Obstacle *obstacle : obstacles) {
        if (visible(boid, obstacle)) {
            n_visible += 1;
            float distance = glm::length(boid->pos - obstacle->pos);
            avoid_vel += glm::normalize(boid->pos - obstacle->pos) / distance;
        }
    }
    if (n_visible > 0) {
        avoid_vel /= n_visible;
    }
    boid->vel += avoidfactor * avoid_vel;
}

bool Flock::visible(Boid *boid, Obstacle *obstacle) {
    glm::vec3 v = obstacle->pos - boid->pos;
    return glm::length(v) - obstacle->size / 2 < visualRange 
        && glm::dot(glm::normalize(boid->vel), glm::normalize(v)) > glm::cos(glm::radians(fov / 2.0f));
}

bool Flock::visible(Boid *boid, Boid *neighbor) {
    glm::vec3 v = neighbor->pos - boid->pos;
    return glm::length(v) < visualRange 
        && glm::dot(glm::normalize(boid->vel), glm::normalize(v)) > glm::cos(glm::radians(fov / 2.0f));
}

void Flock::update(float deltaTime, Prey *prey, std::vector<Obstacle*> &obstacles) {
    for (int i = 0; i < n_boids; i++) {
        float n_neighbors = 0;
        glm::vec3 s_vel = glm::vec3(0, 0, 0);
        glm::vec3 a_vel = glm::vec3(0, 0, 0);
        glm::vec3 c_vel = glm::vec3(0, 0, 0);
        glm::vec3 avoid_vel = glm::vec3(0, 0, 0);
        glm::vec3 avg_vel = glm::vec3(0, 0, 0);
        glm::vec3 avg_pos = glm::vec3(0, 0, 0);
        for (int j = 0; j < n_boids; j++) {
            if (i == j) {
                continue;
            }
            if (visible(boids[i], boids[j])) {
                n_neighbors += 1;
                // separation
                float distance = glm::length(boids[i]->pos - boids[j]->pos);
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
        chasePrey(boids[i], prey);

        // avoid obstacles
        avoidObstacles(boids[i], obstacles);

        // check border
        checkBorder(boids[i]);

        // update vel and pos
        boids[i]->update(deltaTime);
    }
}