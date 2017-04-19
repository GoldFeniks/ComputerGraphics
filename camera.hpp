#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <GL/glew.h>

namespace My {

	class Camera {

	public:

		static const struct Default {

			static const glm::vec3 Eye, Center, Up;
			static const GLfloat Step, Angle;

		};

		Camera(glm::vec3 eye = Default::Eye, glm::vec3 center = Default::Center, glm::vec3 up = Default::Up) :
			eye(eye), center(center), up(up) {};

		void SetProjection(GLfloat, GLfloat, GLfloat, GLfloat);
		void SetFOV(GLfloat);
		void SetRatio(GLfloat);
		void SetZNear(GLfloat);
		void SetZFar(GLfloat);
		void Reset();
		void ResetEye();
		void ResetCenter();
		void ResetUp();
		void SetEye(glm::vec3);
		void SetCenter(glm::vec3);
		void SetUp(glm::vec3);
		void MoveEye(glm::vec3);
		void MoveCenter(glm::vec3);
		void MoveUp(glm::vec3);
		void Setup(glm::vec3 = Default::Eye, glm::vec3 = Default::Center, glm::vec3 = Default::Up);
		void RotateXY(GLfloat = Default::Angle);
		void RotateYZ(GLfloat = Default::Angle);
		void RotateXZ(GLfloat = Default::Angle);
		void Rotate(GLfloat = Default::Angle, GLfloat = Default::Angle, GLfloat = Default::Angle);
		void Rotate(glm::vec3, GLfloat = Default::Angle);
		void MoveXY(GLfloat = Default::Step);
		void MoveYZ(GLfloat = Default::Step);
		void MoveXZ(GLfloat = Default::Step);
		void Move(GLfloat = Default::Step, GLfloat = Default::Step, GLfloat = Default::Step);
		glm::vec3 GetEye();
		glm::vec3 GetDirection();
		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();

	private:

		glm::vec3 eye, center, up;
		glm::mat4 projection = glm::mat4(1);
		GLfloat fov, ratio, z_near, z_far;

		void moveEyeCenter(glm::vec3);

	};

}// namesapcce My
