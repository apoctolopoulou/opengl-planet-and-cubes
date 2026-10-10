#pragma once

// shaders
// vertex shader: positions, normals, light and texture coords
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormal;\n"
"layout (location = 2) in vec2 aTexCoords;\n"
"out vec3 FragPos; out vec3 Normal; out vec2 TexCoord;\n"
"uniform mat4 model; uniform mat4 view; uniform mat4 projection;\n"
"void main() {\n"
"	FragPos = vec3(model * vec4(aPos, 1.0));\n"
"	Normal = mat3(transpose(inverse(model))) * aNormal;\n"
"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"	TexCoord = aTexCoords;\n"
"}\0";

// fragment shader: final pixel colors with lighting
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 FragPos; in vec3 Normal; in vec2 TexCoord;\n"
"uniform vec3 lightPos; uniform sampler2D ourTexture; uniform bool isPlanet;\n"
"void main() {\n"
"	if(isPlanet) {\n"	// planet is the light source
"		FragColor = texture(ourTexture, TexCoord);\n"
"	} else {\n"
"		vec3 norm = normalize(Normal);\n"
"		vec3 lightDir = normalize(lightPos - FragPos);\n"
"		float score = max(dot(norm, lightDir), 0.0);\n"
"		FragColor = vec4(score * texture(ourTexture, TexCoord).rgb, 1.0);\n"
"	}\n"
"}\n\0";
