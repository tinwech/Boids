#include <iostream>
#include <vector>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "object.h"
#include "camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "boid.h"
#include "flock.h"
#include "arena.h"
#include "prey.h"

using namespace std;

GLFWwindow* init();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void loadMaterialLight();
unsigned int createShader(const char* filename, const char* type);
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
void mainPanel(Arena* arena, Flock* flock, Prey* prey);
void renderUI(GLFWwindow* window, Arena* arena, Flock* flock, Prey* prey);

int windowWidth = 800, windowHeight = 600;

// camera
Camera camera(glm::vec3(80.0f, 0.0f, 80.0f));
float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool mouse_pressed = false;
bool first_press = false;

// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

Material material;
Light light;

int main()
{
	// Initialization
	GLFWwindow* window = init();

	// load material and light param
	loadMaterialLight();

	// Create shaders
	unsigned int vertexShader, fragmentShader, shaderProgram;
	vertexShader = createShader("../src/shaders/Phong.vert", "vert");
	fragmentShader = createShader("../src/shaders/Phong.frag", "frag");
	shaderProgram = createProgram(vertexShader, fragmentShader);

	// Init arena
	Arena *arena = new Arena(60, 40, 60, false);

	// Init flock
	Flock *flock = new Flock(30, arena->x_size, arena->y_size, arena->z_size);
	
	// Init prey
	Prey *prey = new Prey(1, arena->x_size, arena->y_size, arena->z_size);

	// Display loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		flock->update(deltaTime, prey);
		prey->update();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)windowWidth / (float)windowHeight, 0.1f, 200.0f);
        glm::mat4 view = camera.GetViewMatrix();

		// Draw boids
		glCullFace(GL_BACK);
		for (Boid *boid : flock->boids) {
			glUseProgram(shaderProgram);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "M"), 1, GL_FALSE, glm::value_ptr(boid->getModel()));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "V"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "P"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(boid->color));
			glUniform3fv(glGetUniformLocation(shaderProgram, "Ka"), 1, glm::value_ptr(material.ambient));
			glUniform3fv(glGetUniformLocation(shaderProgram, "Kd"), 1, glm::value_ptr(material.diffuse));
			glUniform3fv(glGetUniformLocation(shaderProgram, "Ks"), 1, glm::value_ptr(material.specular));
			glUniform1f(glGetUniformLocation(shaderProgram, "gloss"), material.gloss);
			glUniform3fv(glGetUniformLocation(shaderProgram, "La"), 1, glm::value_ptr(light.ambient));
			glUniform3fv(glGetUniformLocation(shaderProgram, "Ld"), 1, glm::value_ptr(light.diffuse));
			glUniform3fv(glGetUniformLocation(shaderProgram, "Ls"), 1, glm::value_ptr(light.specular));
			glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(light.position));
			glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, glm::value_ptr(camera.Position));
			glUniform1f(glGetUniformLocation(shaderProgram, "revertNormal"), 1);
			glBindVertexArray(boid->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 18);
		}

		// Draw prey
		glCullFace(GL_BACK);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "M"), 1, GL_FALSE, glm::value_ptr(prey->getModel()));
		glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(prey->color));
		glBindVertexArray(prey->VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Draw arena
		if (arena->visualize) {
			glCullFace(GL_FRONT);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "M"), 1, GL_FALSE, glm::value_ptr(arena->getModel()));
			glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(arena->color));
			glUniform1f(glGetUniformLocation(shaderProgram, "revertNormal"), -1);
			glBindVertexArray(arena->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// render GUI
		renderUI(window, arena, flock, prey);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void mainPanel(Arena* arena, Flock* flock, Prey* prey) {
    ImGui::SetNextWindowSize(ImVec2(300.0f, 250.0f), ImGuiCond_Once);
    ImGui::SetNextWindowCollapsed(0, ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(10.0f, 340.0f), ImGuiCond_Once);
    ImGui::SetNextWindowBgAlpha(0.2f);
    if (ImGui::Begin("Control")) {
		// Boids parameters
        ImGui::Text("Boids");
        ImGui::SliderFloat("Separation", &flock->separation, 0.0f, 20.0f);
        ImGui::SliderFloat("Alignment", &flock->alignment, 0.0f, 3.0f);
        ImGui::SliderFloat("Cohesion", &flock->cohesion, 0.0f, 3.0f);
        ImGui::SliderFloat("Visual Range", &flock->visualRange, 1.0f, 20.0f);

		// Prey parameters
        ImGui::Text("Prey");
		ImGui::Checkbox("Immortal", &prey->immortal);
        ImGui::SliderFloat("x", &prey->pos[0], flock->x_min, flock->x_max);
        ImGui::SliderFloat("y", &prey->pos[1], flock->y_min, flock->y_max);
        ImGui::SliderFloat("z", &prey->pos[2], flock->z_min, flock->z_max);

		// Arena
        ImGui::Text("Arena");
		ImGui::Checkbox("Visualize", &arena->visualize);
    }
    ImGui::End();
}

void renderUI(GLFWwindow* window, Arena* arena, Flock* flock, Prey* prey) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    mainPanel(arena, flock, prey);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GLFWwindow* init() {
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFE_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Boids Simulation", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	// glfwSetScrollCallback(window, scroll_callback);
	glfwSwapInterval(1);

	// Init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(1);
	}

	// Setup OpenGL
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE); 

	// Init GUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	return window;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        double xpos, ypos;
    	glfwGetCursorPos(window, &xpos, &ypos);
	    lastX = xpos;
		lastY = ypos;
		mouse_pressed = true;
		first_press = true;
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		mouse_pressed = false;
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

	if (first_press) {
        lastX = xpos;
        lastY = ypos;
		first_press = false;
	}

    if (mouse_pressed) {

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

unsigned int createShader(const char* filename, const char* type) {
	unsigned int shader;
	if (strcmp(type, "vert") == 0)
		shader = glCreateShader(GL_VERTEX_SHADER);
	else if (strcmp(type, "frag") == 0)
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	else
	{
		cout << "Unknown Shader Type.\n";
		return 0;
	}

	FILE* fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);  //same as rewind(fp);

	char* source = (char*)malloc(sizeof(char) * (fsize + 1));
	fread(source, fsize, 1, fp);
	fclose(fp);

	source[fsize] = 0;

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::" << filename << "::COMPLIATION_FAILED\n" << infoLog << endl;
		return -1;
	}

	return shader;
}

unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader) {
	unsigned int program = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	//Link our program
	glLinkProgram(program);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	int success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		char* infoLog = (char*)malloc(sizeof(char) * (maxLength));
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

		//We don't need the program anymore.
		glDeleteProgram(program);
		//Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		puts(infoLog);
		free(infoLog);

		return -1;
	}

	//Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}

void loadMaterialLight() {
	material.ambient = glm::vec3(1.0, 1.0, 1.0);
	material.diffuse = glm::vec3(1.0, 1.0, 1.0);
	material.specular = glm::vec3(0.7, 0.7, 0.7);
	material.gloss = 10.5;

	light.ambient = glm::vec3(0.2, 0.2, 0.2);
	light.diffuse = glm::vec3(0.8, 0.8, 0.8);
	light.specular = glm::vec3(0.5, 0.5, 0.5);
	light.position = glm::vec3(80, 100, 60);
}
