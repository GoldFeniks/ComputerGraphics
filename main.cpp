#define _USE_MATH_DEFINES
#define GL_MAX_LIGHTS 8
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include "program.hpp"
#include "triangles.hpp"
#include "window.hpp"
#include <cmath>
#include "vao.hpp"
#include "vbo.hpp"
#include "lines.hpp"
#include "object_loader.hpp"
#include <fstream>

unsigned long long c_time = 0;
int mouse_x = 1600, mouse_y = 900;
double step = 0.001;
float plane_y = -std::sqrtf(3) / 2, plane_x = -5, plane_z = -5;
int lines_count = 20;
float sensativity = 0.5;

int main() {
	My::Window window(1500, 1500, "Triangle", sf::Style::Default, sf::ContextSettings(24, 8, 0, 4, 4));
	window.GetCurrentCamera().SetProjection(glm::radians(60.f), window.Ratio(), 0.1f, 10000.f);
	window.GetCurrentCamera().Setup(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0));
	window.AddCamera(My::Camera(glm::vec3(0, 3, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)));
	window.GetCamera(1).SetProjection(glm::radians(60.f), window.Ratio(), 0.1f, 100.f);
	window.AddLightSource(My::Lights::LightSource::Ambient());
	window.GetLightSource(0).GetParameters().Ambient = { 1.2f, 1.2f, 1.2f, 1 };
	window.AddLightSource(My::Lights::LightSource::Spot());
	window.GetLightSource(1).GetParameters().Position = { 0, 0, 1, 1 };
	window.GetLightSource(1).GetParameters().Diffuse = { 3, 3, 3, 1 };
	window.GetLightSource(1).GetParameters().Specular = { 3, 3, 3, 1 };
	window.GetLightSource(1).GetParameters().SpotDirection = { 1, 1, 1 };
	window.GetLightSource(1).GetParameters().SpotCutoff = 45;
	window.GetLightSource(1).GetParameters().SpotExponent = 15;
	window.GetLightSource(1).GetParameters().LinearAttenuation = 0.1;
	My::Program program("shaders/vertex.glsl", "shaders/fragment.glsl");
	My::Scene human("test/models/BLEND/HUMAN.blend");
	window.AddDrawable(human.GetModel<My::Points::Point3Norm<GLfloat>>(&program));
	sf::Clock clock;
	c_time = clock.getElapsedTime().asMilliseconds();
	while (window.IsOpen()) {
		sf::Event event;
		while (window.PollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.Close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape)
					window.Close();
				else if (event.key.code == sf::Keyboard::Num0)
					window.UseCamera(0);
				else if (event.key.code == sf::Keyboard::Num1)
					window.UseCamera(1);
			}
			if (event.type == sf::Event::Resized) {
				glViewport(0, 0, window.GetSize().x, window.GetSize().y);
				window.GetCurrentCamera().SetProjection(glm::radians(60.f), window.Ratio(), 0.1f, 10000.f);
			}
		}
		unsigned long long d_time = (unsigned long long)clock.getElapsedTime().asMilliseconds() - c_time;
		c_time += d_time;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			window.GetCurrentCamera().RotateYZ(My::Camera::Default::Angle * d_time);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			window.GetCurrentCamera().RotateYZ(-My::Camera::Default::Angle * d_time);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			window.GetCurrentCamera().RotateXZ(My::Camera::Default::Angle * d_time);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			window.GetCurrentCamera().RotateXZ(-My::Camera::Default::Angle * d_time);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			window.GetCurrentCamera().RotateXY(My::Camera::Default::Angle * d_time);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			window.GetCurrentCamera().RotateXY(-My::Camera::Default::Angle * d_time);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			window.GetCurrentCamera().MoveXZ(My::Camera::Default::Step * d_time);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			window.GetCurrentCamera().MoveXZ(-My::Camera::Default::Step * d_time);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			window.GetCurrentCamera().MoveXY(My::Camera::Default::Step * d_time);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			window.GetCurrentCamera().MoveXY(-My::Camera::Default::Step * d_time);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			window.GetCurrentCamera().MoveYZ(-My::Camera::Default::Step * d_time);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
			window.GetCurrentCamera().MoveYZ(My::Camera::Default::Step * d_time);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			window.GetCurrentCamera().Reset();
		//window.GetCurrentCamera().RotateYZ(My::Camera::Default::Angle * (mouse_y - sf::Mouse::getPosition().y) * sensativity);
		//window.GetCurrentCamera().RotateXZ(My::Camera::Default::Angle * (mouse_x - sf::Mouse::getPosition().x) * sensativity);
		//sf::Mouse::setPosition(sf::Vector2i(mouse_x, mouse_y));
		glm::vec3 c_pos = window.GetCurrentCamera().GetEye();
		glm::vec3 c_dir = window.GetCurrentCamera().GetDirection();
		window.GetLightSource(1).GetParameters().SpotDirection = { c_dir.x, c_dir.y, c_dir.z };
		window.GetLightSource(1).GetParameters().Position = { c_pos.x, c_pos.y, c_pos.z, 1 };
		window.Display();
	}
	return 0;
}