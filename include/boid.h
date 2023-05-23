#ifndef BOID_H
#define BOID_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Boid {
    public:
    Boid();
    Boid(float x, float y, float z);
    ~Boid();
    void createVAO();
    glm::mat4 getModel();

    unsigned int VAO, VBO;
    glm::vec3 pos;
    glm::vec3 vel;
    glm::mat4 model = glm::mat4(1.0f);
};

#endif