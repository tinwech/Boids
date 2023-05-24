#include "boid.h"
#include <iostream>

Boid::Boid() {
    createVAO();    
    pos = glm::vec3(0.0f, 0.0f, 0.0f);
    vel = glm::vec3(2.0f, 3.0f, 1.0f);
}

Boid::Boid(float x, float y, float z) {
	createVAO();
	pos = glm::vec3(x, y, z);
    vel = glm::vec3(2.0f, 3.0f, 1.0f);
}

Boid::~Boid() {
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

glm::mat4 Boid::getModel() {
	glm::mat4 model = glm::mat4(1.0f);
	// translation
	model = glm::translate(model, pos);
	// rotation
	glm::vec3 dir = glm::vec3(0, 1, 0);
	glm::vec3 axis = glm::cross(dir, vel);
	float angle = glm::acos(glm::dot(glm::normalize(dir), glm::normalize(vel)));
	model = glm::rotate(model, angle, axis);
	return model;
}

void Boid::createVAO() {
    float vertices[] = {    
		// x, y, z, nx, ny, nz

		// bottom face
		-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
		-1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
		-1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f,

		// 4 sides
		 0.0f,  2.0f,  0.0f, 0, 0.31622777, 0.9486833,
		-1.0f, -1.0f,  1.0f, 0, 0.31622777, 0.9486833,
		 1.0f, -1.0f,  1.0f, 0, 0.31622777, 0.9486833,

		 0.0f,  2.0f,  0.0f, 0.9486833, 0.31622777, 0,
		 1.0f, -1.0f,  1.0f, 0.9486833, 0.31622777, 0,
		 1.0f, -1.0f, -1.0f, 0.9486833, 0.31622777, 0,

		 0.0f,  2.0f,  0.0f, 0, 0.31622777, -0.9486833,
		 1.0f, -1.0f, -1.0f, 0, 0.31622777, -0.9486833,
		-1.0f, -1.0f, -1.0f, 0, 0.31622777, -0.9486833,

		 0.0f,  2.0f,  0.0f, -0.9486833, 0.31622777, 0,
		-1.0f, -1.0f,  1.0f, -0.9486833, 0.31622777, 0,
		-1.0f, -1.0f, -1.0f, -0.9486833, 0.31622777, 0
	};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
