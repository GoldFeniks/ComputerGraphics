#include "utils.hpp"
#include <fstream>
#include <cstdlib>
#include <glm/vec2.hpp>
#include <glm/gtc/noise.hpp>
#include <vector>

std::string My::Utils::ReadFile(std::string path) {
	std::ifstream stream(path.c_str());
	std::string out, line;
	while (std::getline(stream, line))
		out.append(line + "\n");
	return out;
}
//TODO: return My::Scene::Texture
GLuint My::Utils::NoiseTexture(GLuint width, GLuint height, GLfloat scale, bool perlin, GLuint seed) {
	GLuint id;
	glGenTextures(1, &id);
	std::srand(seed);
	glm::vec2 point = { RandomFloat(0, 1024), RandomFloat(0, 1024) };
	GLfloat step = 1.f / scale;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> vals(9, 0);
	for (size_t i = 0; i < height; ++i)
		for (size_t j = 0; j < width; ++j) 
			if (perlin)
				normals.push_back((glm::perlin(point + glm::vec2(i * step, j * step)) + 1) / 2.f);
			else 
				normals.push_back((glm::simplex(point + glm::vec2(i * step, j * step)) + 1) / 2.f);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, normals.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return id;
}

GLfloat My::Utils::RandomFloat(GLfloat min, GLfloat max) {
	return min + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (max - min)));
}
