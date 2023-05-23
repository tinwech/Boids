#include "boid.h"
#include <vector>

class Flock {
    public:
    Flock(int n_boids, float sx, float sy, float sz);
    void checkBorder(Boid *boid);
    void update();

    float separation;
    float alignment;
    float cohesion;
    float turnfactor = 0.2;
    float range;
    float dt = 0.01;
    float x_min, x_max;
    float y_min, y_max;
    float z_min, z_max;
    int n_boids;
    std::vector<Boid*> boids;
};