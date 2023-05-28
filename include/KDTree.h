#include "boid.h"
#include <vector>

struct KDNode {
    Boid* boid;
    KDNode* left;
    KDNode* right;
};

class KDTree {
private:
    KDNode* root;

private:
    KDNode* buildKDTree(std::vector<Boid*> boids, int depth);
    void searchNeighbors(KDNode* node, const Boid* target, double sqrradius, std::vector<Boid*>& neighbors);

public:
    void buildKDTree(std::vector<Boid*> boids);
    void searchNeighbors(const Boid* target, double radius, std::vector<Boid*>& neighbors);
};