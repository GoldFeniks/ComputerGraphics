#include "vbo.hpp"

My::VBO::VBO() {
	glGenBuffers(1, &id);
}

My::VBO::VBO(VBO&& other) {
	*this = std::move(other);
}

My::VBO::~VBO() {
	glDeleteBuffers(1, &id);
}

My::VBO& My::VBO::operator=(VBO&& other) {
	std::swap(id, other.id);
	return *this;
}

void My::VBO::Bind(GLenum target) {
	glBindBuffer(target, id);
}

void My::VBO::Unbind(GLenum target) {
	glBindBuffer(target, 0);
}

void My::VBO::SetVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLuint stride, GLvoid* pointer) {
	Bind();
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	glEnableVertexAttribArray(index);
	Unbind();
}

void My::VBO::BufferData(GLenum target, GLsizeiptr size, GLvoid* data, GLenum usage) {
	Bind();
	glBufferData(target, size, data, usage);
	Unbind();
}
