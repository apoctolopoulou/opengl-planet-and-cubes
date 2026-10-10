#pragma once

// screen settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// simulation state
float camX = 0.0f, camY = 0.0f;		// camera rotation
bool isMoving = true;				// toggle for animation
float lastTime = 0.0f;
float totalTime = 0.0f;
