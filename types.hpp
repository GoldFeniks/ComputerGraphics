#pragma once
#include <GL/glew.h>

namespace My {

	namespace Types {

		template<typename T>
		struct Type {};

		template<>
		struct Type<GLfloat> {

			static const GLenum GLType = GL_FLOAT;

		};

	}// namespcae Types

}// namespace My
