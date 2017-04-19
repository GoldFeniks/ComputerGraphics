#include "vao.hpp"

My::VAO::VAO() {
	glGenVertexArrays(1, &id);
}

My::VAO::VAO(VAO&& other) {
	*this = std::move(other);
}

My::VAO::~VAO() {
	glDeleteVertexArrays(1, &id);
}

My::VAO& My::VAO::operator=(VAO&& other) {
	std::swap(id, other.id);
	return *this;
}

void My::VAO::Bind() {
	glBindVertexArray(id);
}

void My::VAO::Unbind() {
	glBindVertexArray(0);
}
