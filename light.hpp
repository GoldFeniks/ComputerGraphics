#pragma once
#include <GL/glew.h>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include "vectors.hpp"
#include "program.hpp"
#include "figures.hpp"

namespace My {

	namespace Lights {

		enum class Type { Ambient, Direction, Point, Spot };

		class LightSource : public Figures::Transformable {

		public:

			struct Parameters {

				glm::vec4 Ambient = { 0, 0, 0, 1 }, Diffuse = { 1, 1, 1, 1 }, Specular = { 1, 1, 1, 1 }, Position = { 0, 0, 0, 0 };
				glm::vec3 SpotDirection = { 0, 0, 0 };
				GLfloat SpotExponent = 0, SpotCutoff = 180, ConstantAttenuation = 1, LinearAttenuation = 0, QuadraticAttenuation = 0;

			};

			LightSource(Type);
			LightSource(Parameters);

			static LightSource Ambient();
			static LightSource Direction();
			static LightSource Point();
			static LightSource Spot();

			Parameters& GetParameters();
			void Use(Program*);
			GLuint GetIndex();
			void SetIndex(GLuint);
			void SetType(Type);
			Type GetType();
			bool Toggle();
			bool IsEnabled();

		private:

			Type type;
			Parameters parameters;
			GLuint index = GL_LIGHT0;
			bool enabled; 

		};

	}// namespace Lights

}// namespace My
