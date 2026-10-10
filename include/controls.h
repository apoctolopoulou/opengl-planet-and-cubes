#pragma once

#include <GLFW/glfw3.h>
#include "config.h"

// callback for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// user keyboard input
void processInput(GLFWwindow* window, float deltaTime) {
	float cameraSpeed = 1.5f;

	// camera rotation
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		camX += cameraSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		camX -= cameraSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		camY -= cameraSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		camY += cameraSpeed * deltaTime;
	}

	// closing program
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// toggle animation
	static bool spacePressed = false;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePressed) {
		isMoving = !isMoving;
		spacePressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
		spacePressed = false;
	}
}
