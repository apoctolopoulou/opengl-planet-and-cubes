#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "include/loaders.h"
#include "include/cube_data.h"
#include "include/shader_source.h"
#include "include/config.h"
#include "include/controls.h"

// link with OpenGL libs
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

int main() {
	// init GLFW
	if (!glfwInit()) {
		std::cerr << "Oops! Failed to init GLFW" << std::endl;
		return -1;
	}

	// setup window options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create the window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Space Simulation 2026", NULL, NULL);
	if (!window) {
		std::cerr << "Oops! Failed to init GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// make the window not close
	glewExperimental = GL_TRUE;

	// init GLEW after creating window
	if (glewInit() != GLEW_OK) {
		std::cerr << "Oops! Failed to init GLEW" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);	// 3D objs dont overlap

	// load sphere data from file
	std::vector<float> sphereVetrices;
	if (!loadOBJ("planet/planet.obj", sphereVetrices)) {
		return -1;
	}

	// turn cube data to vector
	std::vector<float> cubeVertices(vertices, vertices + sizeof(vertices) / sizeof(float));

	// setup gpu data
	// SPHERE vao/vbo
	unsigned int sphereVAO, sphereVBO, cubeVAO, cubeVBO;
	setupVAOVBO(sphereVAO, sphereVBO, sphereVetrices);
	setupVAOVBO(cubeVAO, cubeVBO, cubeVertices);

	// compile and link shaders
	unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

	// get uniform locations once to save cpu cycles
	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
	GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
	GLint isPlanetLoc = glGetUniformLocation(shaderProgram, "isPlanet");

	stbi_set_flip_vertically_on_load(true);	// so the image will look right side up

	unsigned int textureCube = loadTexture("texture/container.png");
	unsigned int texturePlanet = loadTexture("planet/planet_Quom1200.png");

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// time steps
		float curTime = (float)glfwGetTime();
		float deltaTime = curTime - lastTime;
		lastTime = curTime;
		if (isMoving) totalTime += deltaTime;	// time passes only when animation is not paused

		// keyboard input
		processInput(window, deltaTime);

		// rendering
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);	// gray background
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);

		// perspective projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// camera matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -40.0f));		// start back from center
		view = glm::rotate(view, camX, glm::vec3(1.0f, 0.0f, 0.0f));	// rotate on X
		view = glm::rotate(view, camY, glm::vec3(0.0f, 1.0f, 0.0f));	// rotate on Y
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		// draw planet from here
		// model matrix for planet
		glm::mat4 planetModel = glm::mat4(1.0f);

		// planet needs to travel around
		planetModel = glm::rotate(planetModel, totalTime * 0.6f, glm::vec3(0.0f, 1.0f, 0.0f));
		planetModel = glm::translate(planetModel, glm::vec3(10.0f, 0.0f, 0.0f));	// planet orbit

		// update light pos based on planet pos
		glm::vec3 curLightPos = glm::vec3(planetModel[3]);
		glUniform3fv(lightPosLoc, 1, glm::value_ptr(curLightPos));

		// planet self rotates as well
		glUniform1i(isPlanetLoc, 1);
		glm::mat4 plFinRender = glm::rotate(planetModel, totalTime * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(plFinRender));

		// draw planet texture and VAO
		glBindTexture(GL_TEXTURE_2D, texturePlanet);
		glBindVertexArray(sphereVAO);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)sphereVetrices.size() / 8);

		// draw 6 cubes
		glUniform1i(isPlanetLoc, 0);	// lighting to cubes
		glBindTexture(GL_TEXTURE_2D, textureCube);
		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 6; i++) {
			glm::mat4 model = planetModel;	// orbit planet
			float offsetAngle = i * glm::radians(60.0f);

			// rotate around planet
			model = glm::rotate(model, totalTime * 1.2f + offsetAngle, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));

			// self rotate, unique for each cube
			float selfRotate = totalTime * (1.0f + i * 1.5f);
			glm::vec3 rotationAxis = glm::vec3(0.2f * i, 1.0f, 0.5f - (0.1f * i));
			model = glm::rotate(model, selfRotate, rotationAxis);

			// send model to shader
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			// draw the cubes
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// swap buffers and check events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean up GPU memory
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &sphereVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &sphereVBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}
