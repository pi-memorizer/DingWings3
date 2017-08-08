#include "Shader.h"
#include <string>
#include <glew.h>
#include <gl/GLU.h>

GLuint loadShaders(std::string fragment);
void changeShader(GLuint shader);

namespace shader {
	GLuint sDefault, sInvert;
	std::string default = std::string() +
		"#version 330 core\n" +
		"layout(location = 0) out vec4 color;" +
		"in vec2 UV;" +
		"uniform sampler2D sprite;" +
		"void main(){" +
		"color = texture(sprite,UV);" +
		"}";
	std::string invert = std::string() +
		"#version 330 core\n" +
		"layout(location = 0) out vec4 color;" +
		"in vec2 UV;" +
		"uniform sampler2D sprite;" +
		"void main(){" +
		"vec4 t = texture(sprite,UV);" +
		"color = vec4(1.0-t.x, 1.0-t.y, 1.0-t.z, t.w);" +
		"}";
}

void shadersInit()
{
	shader::sDefault = loadShaders(shader::default);
	shader::sInvert = loadShaders(shader::invert);
}

void sDefault()
{
	changeShader(shader::sDefault);
}

void sInvert()
{
	changeShader(shader::sInvert);
}