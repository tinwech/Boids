#include "boid.h"
#include "prey.h"
#include <vector>

class Flock {
    public:
    Flock(int n_boids, float sx, float sy, float sz);
    void checkBorder(Boid *boid);
    void update(float deltaTime, Prey *prey);

    float separation = 12;
    float alignment = 0.5;
    float cohesion = 0.5;
    float turnfactor = 0.2;
    float chasefactor = 5;
    float visualRange = 5;
    float x_min, x_max;
    float y_min, y_max;
    float z_min, z_max;
    float minSpeed = 5;
    float maxSpeed = 20;
    int n_boids;
    std::vector<Boid*> boids;
};