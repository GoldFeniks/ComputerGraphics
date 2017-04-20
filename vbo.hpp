#pragma once
#include <GL/glew.h>
#include <utility>

namespace My {

	class VBO {

	public:

		VBO();
		VBO(const VBO&) = delete;
		VBO(VBO&&);
		~VBO();

		VBO& operator=(const VBO&) = delete;
		VBO& operator=(VBO&&);

		void Bind(GLenum = GL_ARRAY_BUFFER);
		static void Unbind(GLenum = GL_ARRAY_BUFFER);

		void SetVertexAttribPointer(GLuint, GLint, GLenum, GLboolean, GLuint, GLvoid*);
		void BufferData(GLenum, GLsizeiptr, GLvoid*, GLenum);

	private:

		GLuint id = 0;

	};

}// namespacce My
