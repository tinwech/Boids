#include "boid.h"
#include "prey.h"
#include <vector>

class Flock {
    public:
    Flock(int n_boids, float sx, float sy, float sz);
    void checkBorder(Boid *boid);
    void chasePrey(Boid *boid, Prey *prey);
    bool visible(Boid *b1, Boid *b2);
    void update(float deltaTime, Prey *prey);

    float separation = 12;
    float alignment = 0.5;
    float cohesion = 0.5;
    float turnfactor = 0.2;
    float chasefactor = 5;
    float visualRange = 5;
    float fov = 240;
    float x_min, x_max;
    float y_min, y_max;
    float z_min, z_max;
    int n_boids;
    std::vector<Boid*> boids;
};