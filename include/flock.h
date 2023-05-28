#include "boid.h"
#include "KDTree.h"
#include "prey.h"
#include "obstacle.h"
#include <vector>

class Flock {
    public:
    Flock(int n_boids, float sx, float sy, float sz);
    void checkBorder(Boid *boid);
    void chasePrey(Boid *boid, Prey *prey);
    void avoidObstacles(Boid *boid, std::vector<Obstacle*> &obstacles);
    bool visible(Boid *boid, Obstacle* obstacle);
    bool visible(Boid *boid, Boid *neighbor);
    void update(float deltaTime, Prey *prey, std::vector<Obstacle*> &obstacles);

    float separation = 12;
    float alignment = 0.5;
    float cohesion = 0.5;
    float turnfactor = 0.2;
    float avoidfactor = 5;
    float chasefactor = 5;
    float visualRange = 5;
    float fov = 240;
    float x_min, x_max;
    float y_min, y_max;
    float z_min, z_max;
    int n_boids;
    std::vector<Boid*> boids;
};