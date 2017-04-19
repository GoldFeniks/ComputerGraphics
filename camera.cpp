#include "camera.hpp"
#include <glm/gtx/rotate_vector.hpp>

const glm::vec3 My::Camera::Default::Eye = glm::vec3(0, 0, 1);
const glm::vec3 My::Camera::Default::Center = glm::vec3(0, 0, 0);
const glm::vec3 My::Camera::Default::Up = glm::vec3(0, 1, 0);
const GLfloat My::Camera::Default::Step = 0.005;
const GLfloat My::Camera::Default::Angle = 0.005;

void My::Camera::SetProjection(GLfloat angle, GLfloat ratio, GLfloat z_near, GLfloat z_far) {
	fov = angle;
	this->ratio = ratio;
	this->z_near = z_near;
	this->z_far = z_far;
	projection = glm::perspective(angle, ratio, z_near, z_far);
}

void My::Camera::SetFOV(GLfloat angle) {
	SetProjection(angle, ratio, z_near, z_far);
}

void My::Camera::SetRatio(GLfloat ratio) {
	SetProjection(fov, ratio, z_near, z_far);
}

void My::Camera::SetZNear(GLfloat z_near) {
	SetProjection(fov, ratio, z_near, z_far);
}

void My::Camera::SetZFar(GLfloat z_far) {
	SetProjection(fov, ratio, z_near, z_far);
}

void My::Camera::Reset() {
	ResetEye();
	ResetCenter();
	ResetUp();
}

void My::Camera::ResetEye() {
	eye = glm::vec3(0, 0, 1);
}

void My::Camera::ResetCenter() {
	center = glm::vec3(0, 0, 0);
}

void My::Camera::ResetUp() {
	up = glm::vec3(0, 1, 0);
}

void My::Camera::SetEye(glm::vec3 vec) {
	eye = vec;
}

void My::Camera::SetCenter(glm::vec3 vec) {
	center = vec;
}

void My::Camera::SetUp(glm::vec3 vec) {
	up = vec;
}

void My::Camera::MoveEye(glm::vec3 vec) {
	eye += vec;
}

void My::Camera::MoveCenter(glm::vec3 vec) {
	center += vec;
}

void My::Camera::MoveUp(glm::vec3 vec) {
	up += vec;
}

void My::Camera::Setup(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
	this->eye = eye;
	this->center = center;
	this->up = up;
}

void My::Camera::RotateXY(GLfloat angle) {
	up = glm::rotate(up, angle, glm::normalize(center - eye));
}

void My::Camera::RotateYZ(GLfloat angle) {
	Rotate(glm::normalize(glm::cross(center - eye, up)), angle);
}

void My::Camera::RotateXZ(GLfloat angle) {
	center = glm::rotate(center - eye, angle, glm::normalize(up)) + eye;
}

void My::Camera::Rotate(GLfloat XY, GLfloat XZ, GLfloat YZ) {
	RotateXY(XY);
	RotateXZ(XZ);
	RotateYZ(YZ);
}

void My::Camera::Rotate(glm::vec3 vec, GLfloat angle) {
	center = glm::rotate(center - eye, angle, vec) + eye;
	up = glm::normalize(glm::rotate(up, angle, vec));
}

void My::Camera::MoveXY(GLfloat step) {
	moveEyeCenter(glm::normalize(glm::cross(center - eye, up)) * step);
}

void My::Camera::MoveYZ(GLfloat step) {
	moveEyeCenter(glm::normalize(up) * step);
}

void My::Camera::MoveXZ(GLfloat step) {
	moveEyeCenter(glm::normalize(center - eye) * step);
}

void My::Camera::Move(GLfloat XY, GLfloat XZ, GLfloat YZ) {
	MoveXY(XY);
	MoveXZ(XZ);
	MoveYZ(YZ);
}

glm::vec3 My::Camera::GetEye() {
	return eye;
}

glm::vec3 My::Camera::GetDirection() {
	return center - eye;
}

glm::mat4 My::Camera::GetViewMatrix() {
	return glm::lookAt(eye, center, up);
}

glm::mat4 My::Camera::GetProjectionMatrix() {
	return projection;
}

void My::Camera::moveEyeCenter(glm::vec3 vec) {
	eye += vec;
	center += vec;
}
