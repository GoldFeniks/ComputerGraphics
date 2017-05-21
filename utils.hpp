#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>

namespace My {

	namespace Utils {

		std::string ReadFile(std::string path);
		GLuint NoiseTexture(GLuint width, GLuint height, GLfloat scale = 1, bool perlin = true, GLuint seed = 0);
		GLfloat RandomFloat(GLfloat min = 0, GLfloat max = 1);

	}

}
