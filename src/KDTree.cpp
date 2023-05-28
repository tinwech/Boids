#include "KDTree.h"
#include <iostream>
#include <algorithm>

void KDTree::buildKDTree(std::vector<Boid*> boids) {
    root = buildKDTree(boids, 0);
}

KDNode* KDTree::buildKDTree(std::vector<Boid*> boids, int depth) {
    if (boids.empty()) {
        return nullptr;
    }

    int axis = depth % 3;
    int median = boids.size() / 2;

    std::nth_element(boids.begin(), boids.begin() + median, boids.end(),
        [axis](const Boid* a, const Boid* b) {
            return  axis == 0 ? a->pos.x < b->pos.x : 
                    axis == 1 ? a->pos.y < b->pos.y : a->pos.z < b->pos.z;
        });

    KDNode* cur = new KDNode();
    cur->boid = boids[median];

    cur->left = buildKDTree(std::vector<Boid*>(boids.begin(), boids.begin() + median), depth + 1);
    cur->right = buildKDTree(std::vector<Boid*>(boids.begin() + median + 1, boids.end()), depth + 1);
    return cur;
}

void KDTree::searchNeighbors(const Boid* target, double radius, std::vector<Boid*>& neighbors) {
    searchNeighbors(root, target, radius*radius, neighbors);
}

void KDTree::searchNeighbors(KDNode* node, const Boid* target, double sqrradius, std::vector<Boid*>& neighbors) {
    if (node == nullptr) {
        return;
    }

    double distance = pow(target->pos.x - node->boid->pos.x, 2) + pow(target->pos.y - node->boid->pos.y, 2) + pow(target->pos.z - node->boid->pos.z, 2);

    if (distance <= sqrradius) {
        neighbors.push_back(node->boid);
    }

    int axis = neighbors.size() % 3;

    if (axis == 0 ? target->pos.x - sqrradius <= node->boid->pos.x : 
        axis == 1 ? target->pos.y - sqrradius <= node->boid->pos.y : target->pos.z - sqrradius <= node->boid->pos.z) {
        searchNeighbors(node->left, target, sqrradius, neighbors);
    }
    if (axis == 0 ? target->pos.x + sqrradius >= node->boid->pos.x : 
        axis == 1 ? target->pos.y + sqrradius >= node->boid->pos.y : target->pos.z + sqrradius >= node->boid->pos.z) {
        searchNeighbors(node->right, target, sqrradius, neighbors);
    }
}
