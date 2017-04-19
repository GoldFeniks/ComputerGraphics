#pragma once
#include <GL/glew.h>
#include <utility>

namespace My {

	class VAO {

	public:

		VAO();
		VAO(const VAO&) = delete;
		VAO(VAO&&);
		~VAO();

		VAO& operator=(const VAO&) = delete;
		VAO& operator=(VAO&&);

		void Bind();
		static void Unbind();

	private:

		GLuint id = 0;

	};

}// namespace My
