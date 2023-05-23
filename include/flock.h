#include "boid.h"
#include <vector>

class Flock {
    public:
    Flock(int n_boids, float sx, float sy, float sz);
    void update();

    float separation;
    float alignment;
    float cohesion;
    float range;
    float dt = 0.01;
    int n_boids;
    std::vector<Boid*> boids;
};