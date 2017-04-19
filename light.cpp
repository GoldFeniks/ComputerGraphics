#include "light.hpp"
#include <glm/gtc/type_ptr.hpp>

My::Lights::LightSource::LightSource(Type type) : type(type) {};

My::Lights::LightSource::LightSource(Parameters params) {
	parameters = params;

}

My::Lights::LightSource My::Lights::LightSource::Ambient() {
	return LightSource(Type::Ambient);
}

My::Lights::LightSource My::Lights::LightSource::Direction() {
	LightSource result(Type::Direction);
	result.parameters.Position = glm::vec4(0, 0, -1, 0);
	return result;
}

My::Lights::LightSource My::Lights::LightSource::Point() {
	LightSource result(Type::Point);
	result.parameters.Position = glm::vec4(0, 0, 0, 1);
	return result;
}

My::Lights::LightSource My::Lights::LightSource::Spot() {
	LightSource result(Type::Spot);
	result.parameters.Position = glm::vec4(0, 0, 0, 1);
	result.parameters.SpotDirection = glm::vec3(0, 0, -1);
	return result;
}

typename My::Lights::LightSource::Parameters& My::Lights::LightSource::GetParameters() {
	return parameters;
}

void My::Lights::LightSource::Use(Program* program) {
	glLightfv(index, GL_AMBIENT, glm::value_ptr(parameters.Ambient));
	glLightfv(index, GL_DIFFUSE, glm::value_ptr(parameters.Diffuse));
	glLightfv(index, GL_SPECULAR, glm::value_ptr(parameters.Specular));
	glLightfv(index, GL_POSITION, glm::value_ptr(parameters.Position));
	glLightfv(index, GL_SPOT_DIRECTION, glm::value_ptr(parameters.SpotDirection));
	glLightfv(index, GL_SPOT_CUTOFF, (GLfloat*)&parameters.SpotCutoff);
	glLightfv(index, GL_SPOT_EXPONENT, (GLfloat*)&parameters.SpotExponent);
	glLightfv(index, GL_CONSTANT_ATTENUATION, (GLfloat*)&parameters.ConstantAttenuation);
	glLightfv(index, GL_LINEAR_ATTENUATION, (GLfloat*)&parameters.LinearAttenuation);
	glLightfv(index, GL_QUADRATIC_ATTENUATION, (GLfloat*)&parameters.QuadraticAttenuation);
}

GLuint My::Lights::LightSource::GetIndex() {
	return index;
}

void My::Lights::LightSource::SetIndex(GLuint i) {
	index = GL_LIGHT0 + i;
}

void My::Lights::LightSource::SetType(Type n_type) {
	if (n_type == type)
		return;
	if (n_type != Type::Spot) {
		parameters.SpotDirection = glm::vec3(0);
		if (n_type != Type::Point) {
			parameters.Position.w = 0;
			if (n_type != Type::Direction)
				parameters.Position = glm::vec4(0);
			else if (!glm::length(parameters.Position))
				parameters.Position = glm::vec4(0, 0, -1, 0);
		}
		else if (!parameters.Position.w)
			parameters.Position.w = 1;
	}
	else
		parameters.SpotDirection = glm::vec3(0, 0, -1);
}

My::Lights::Type My::Lights::LightSource::GetType() {
	return type;
}

