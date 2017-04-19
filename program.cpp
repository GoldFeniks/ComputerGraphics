#include "program.hpp"
#include <iostream>
#include "utils.hpp"
#include <glm/gtc/type_ptr.hpp>

void My::Program::SetShader(std::string path, GLuint type) {
	std::string s = My::ReadFile(path);
	const char* sourseCode = s.c_str();
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &sourseCode, NULL);
	glCompileShader(shader);
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		State = States::FAIL;
		GLchar* log = new GLchar[1024];
		glGetShaderInfoLog(shader, 1024, NULL, log);
		std::cout << "[SHADER COMPILATION ERROR]: " << path << std::endl << log << std::endl;
	}
	glAttachShader(Id, shader);
	glDeleteShader(shader);
}

void My::Program::LoadMVP() {
	glUniformMatrix4fv(glGetUniformLocation(Id, ProjectionAttribute.c_str()), 1, GL_FALSE, glm::value_ptr(Projection));
	glUniformMatrix4fv(glGetUniformLocation(Id, ViewAttribute.c_str()), 1, GL_FALSE, glm::value_ptr(View));
	glUniformMatrix4fv(glGetUniformLocation(Id, ModelAttribute.c_str()), 1, GL_FALSE, glm::value_ptr(Model));
}

void My::Program::Link() {
	GLint success;
	glLinkProgram(Id);
	glGetProgramiv(Id, GL_LINK_STATUS, &success);
	if (!success) {
		State = States::FAIL;
		GLchar* log = new GLchar[1024];
		glGetProgramInfoLog(Id, 1024, NULL, log);
		std::cout << "[PROGRAM LINKING ERROR]" << std::endl << log << std::endl;
	}
}

void My::Program::Enable() {
	glUseProgram(Id);
}

void My::Program::Disable() {
	glUseProgram(0);
}

bool My::Program::IsOk() {
	return State == States::OK;
}

GLint My::Program::GetAttributeLocation(std::string name) {
	return glGetAttribLocation(Id, name.c_str());
}
