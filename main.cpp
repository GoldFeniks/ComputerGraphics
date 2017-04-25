#define _USE_MATH_DEFINES
#define GL_MAX_LIGHTS 10
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
float plane_y = -7, plane_x = -40, plane_z = -40;
int lines_count = 40;
float sensativity = 0.5;

std::vector<My::Figures::Transformable*> light_objects;

void makeModelBrightAgain(My::Figures::Model* model) {
	for (size_t i = 0; i < model->Size(); ++i) {
		auto material = model->GetDrawableAs<My::Figures::Material>(i);
		material->Ambient = { 0.1f, 0.1f, 0.1f, 1 };
		material->Diffuse = { 3, 3, 3, 1 };
		material->Specular = { 3, 3, 3, 1 };
	}
}

void setAmbient(My::Figures::Model* model, My::Vectors::Vector4<GLfloat> amb = { 0.1f, 0.1f, 0.1f, 1 }) {
	for (size_t i = 0; i < model->Size(); ++i)
		model->GetDrawableAs<My::Figures::Material>(i)->Ambient = amb;
}

void setShininess(My::Figures::Model* model, GLfloat shn = 64) {
	for (size_t i = 0; i < model->Size(); ++i)
		model->GetDrawableAs<My::Figures::Material>(i)->Shininess = shn;
}


int main() {
	My::Window window(1500, 1500, "Triangle", sf::Style::Default, sf::ContextSettings(24, 8, 0, 4, 4));
	window.GetCurrentCamera().SetProjection(glm::radians(60.f), window.Ratio(), 0.1f, 10000.f);
	window.GetCurrentCamera().Setup(glm::vec3(0, 0, 15), glm::vec3(0, 0, 0));
	window.AddCamera(My::Camera(glm::vec3(0, 3, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)));
	window.GetCamera(1).SetProjection(glm::radians(60.f), window.Ratio(), 0.1f, 100.f);
	window.AddLightSource(My::Lights::LightSource::Ambient());
	light_objects.push_back(nullptr);
	window.GetLightSource(0).GetParameters().Ambient = { 0.1f, 0.1f, 0.1f, 1 };
	window.AddLightSource(My::Lights::LightSource::Spot());
	light_objects.push_back(nullptr);
	window.GetLightSource(1).GetParameters().Position = { 0, 0, 1, 1 };
	window.GetLightSource(1).GetParameters().Diffuse = { 1, 1, 1, 1 };
	window.GetLightSource(1).GetParameters().Specular = { 1, 1, 1, 1 };
	window.GetLightSource(1).GetParameters().SpotDirection = { 1, 1, 1 };
	window.GetLightSource(1).GetParameters().SpotCutoff = 45;
	window.GetLightSource(1).GetParameters().SpotExponent = 15;
	window.GetLightSource(1).GetParameters().LinearAttenuation = 0.1;
	My::Program program("shaders/vertex.glsl", "shaders/fragment.glsl");
	My::Program a_program("shaders/light_source_vertex.glsl", "shaders/light_source_fragment.glsl");
	auto scenes = My::Scene::LoadScenes({ "test/models/BLEND/HUMAN.blend", "test/models/PLY/cube.ply", 
		"test/models/Collada/sphere.dae", "test/dodecahedron_round.stl", "nanosuit/nanosuit.obj",
		"test/Sphere slide.stl"
	});
	window.AddDrawable(scenes[1].GetModel<My::Points::Point3Norm<GLfloat>>(&program));
	setAmbient(window.GetDrawableAs<My::Figures::Model>(0));
	setShininess(window.GetDrawableAs<My::Figures::Model>(0), 1);
	auto tr = window.GetDrawableAs<My::Figures::Transformable>(0);
	tr->Scale(glm::vec3(10, 10, 10));
	tr->Translate(glm::vec3(-15, -8, -10));
	window.AddDrawable(scenes[0].GetModel<My::Points::Point3Norm<GLfloat>>(&program));
	setAmbient(window.GetDrawableAs<My::Figures::Model>(1));
	tr = window.GetDrawableAs<My::Figures::Transformable>(1);
	tr->Rotate(-M_PI / 2, glm::vec3(1, 0, 0));
	tr->Translate(glm::vec3(-14, 7.2, -9));
	tr->Rotate(M_PI / 4, glm::vec3(0, 0, 1));
	window.AddDrawable(*window[1]);
	tr = window.GetDrawableAs<My::Figures::Transformable>(2);
	tr->Rotate(-M_PI / 2, glm::vec3(0, 0, 1));
	tr->Translate(glm::vec3(8, 0, 0));
	window.AddDrawable(*window[1]);
	tr = window.GetDrawableAs<My::Figures::Transformable>(3);
	tr->Rotate(M_PI / 2, glm::vec3(0, 0, 1));
	tr->Translate(glm::vec3(0, 0, 8));
	window.AddDrawable(*window[1]);
	tr = window.GetDrawableAs<My::Figures::Transformable>(4);
	tr->Rotate(M_PI, glm::vec3(0, 0, 1));
	tr->Translate(glm::vec3(8, 0, 8));
	window.AddDrawable(scenes[2].GetModel<My::Points::Point3Norm<GLfloat>>(&a_program));
	setAmbient(window.GetDrawableAs<My::Figures::Model>(5), { 1, 0, 0.5, 1});
	tr = window.GetDrawableAs<My::Figures::Transformable>(5);
	tr->Scale(glm::vec3(0.2, 0.2, 0.2));
	tr->Translate(glm::vec3(-12, 8, -7));
	window.AddLightSource(My::Lights::LightSource::Point());
	window.GetLightSource(2).GetParameters().Position = { -12, 8, -7, 1 };
	window.GetLightSource(2).GetParameters().Diffuse = { 1, 0, 1.5, 1 };
	window.GetLightSource(2).GetParameters().Specular = { 1, 0, 1.5, 1 };
	window.GetLightSource(2).GetParameters().LinearAttenuation = 0.5;
	light_objects.push_back(tr);
	window.AddDrawable(*window[5]);
	setAmbient(window.GetDrawableAs<My::Figures::Model>(6), { 0.2f, 1, 0.2f, 1 });
	tr = window.GetDrawableAs<My::Figures::Transformable>(6);
	tr->Translate(glm::vec3(4, 0, 4));
	window.AddLightSource(My::Lights::LightSource::Point());
	window.GetLightSource(3).GetParameters().Position = { -8, 8, -3, 1 };
	window.GetLightSource(3).GetParameters().Diffuse = { 0.2f, 1, 0.2f, 1 };
	window.GetLightSource(3).GetParameters().Specular = { 0.2f, 1, 0.2f, 1 };
	window.GetLightSource(3).GetParameters().LinearAttenuation = 0.5;
	light_objects.push_back(tr);
	window.AddLightSource(My::Lights::LightSource::Direction());
	window.GetLightSource(4).GetParameters().Position = { -1, -1, -1, 0 };
	window.GetLightSource(4).GetParameters().Diffuse = { 0, 0, 0.2f, 1 };
	window.GetLightSource(4).GetParameters().Specular = { 0, 0, 0.2f, 1 };
	light_objects.push_back(nullptr);
	window.AddDrawable(scenes[3].GetModel<My::Points::Point3Norm<GLfloat>>(&program));
	setAmbient(window.GetDrawableAs<My::Figures::Model>(7));
	tr = window.GetDrawableAs<My::Figures::Transformable>(7);
	tr->Scale(glm::vec3(0.005, 0.005, 0.005));
	tr->Translate(glm::vec3(20, 3, 25));
	tr->Rotate(-M_PI / 2, glm::vec3(1, 0, 0));
	window.AddLightSource(My::Lights::LightSource::Spot());
	window.GetLightSource(5).GetParameters().SpotDirection = { 0, 0, -1 };
	window.GetLightSource(5).GetParameters().Diffuse = { 1, 0, 0, 1 };
	window.GetLightSource(5).GetParameters().Specular = { 1, 0, 0, 1 };
	window.GetLightSource(5).GetParameters().SpotCutoff = 30;
	window.GetLightSource(5).GetParameters().SpotExponent = 60;
	window.GetLightSource(5).GetParameters().Position = { 20, 3, 25, 1 };
	light_objects.push_back(tr);
	window.AddDrawable(scenes[4].GetModel<My::Points::Point3TexNormTangents<GLfloat>>(&program));
	makeModelBrightAgain(window.GetDrawableAs<My::Figures::Model>(8));
	tr = window.GetDrawableAs<My::Figures::Transformable>(8);
	tr->Translate(glm::vec3(20, -7, 20));
	window.AddDrawable(scenes[4].GetModel<My::Points::Point3Norm<GLfloat>>(&program));
	tr = window.GetDrawableAs<My::Figures::Transformable>(9);
	tr->Translate(glm::vec3(20, -7, 35));
	tr->Rotate(M_PI, glm::vec3(0, 1, 0));
	window.AddDrawable(*window[7]);
	tr = window.GetDrawableAs<My::Figures::Transformable>(10);
	tr->Translate(glm::vec3(0, 0, 5));
	tr->Rotate(M_PI, glm::vec3(1, 0, 0));
	window.AddLightSource(My::Lights::LightSource::Spot());
	window.GetLightSource(6).GetParameters().SpotDirection = { 0, 0, 1 };
	window.GetLightSource(6).GetParameters().Diffuse = { 0, 1, 0, 1 };
	window.GetLightSource(6).GetParameters().Specular = { 0, 1, 0, 1 };
	window.GetLightSource(6).GetParameters().SpotCutoff = 60;
	window.GetLightSource(6).GetParameters().SpotExponent = 1;
	window.GetLightSource(6).GetParameters().Position = { 20, 3, 30, 1 };
	light_objects.push_back(tr);
	window.AddDrawable(scenes[3].GetModel<My::Points::Point3Norm<GLfloat>>(&program));
	tr = window.GetDrawableAs<My::Figures::Transformable>(11);
	tr->Translate(glm::vec3(0, -3, -5));
	tr->Scale(glm::vec3(0.005, 0.005, 0.005));
	tr->Rotate(M_PI / 2, glm::vec3(0, 0, 1));
	window.AddLightSource(My::Lights::LightSource::Spot());
	window.GetLightSource(7).GetParameters().SpotDirection = { -1 , 0, 0 };
	window.GetLightSource(7).GetParameters().Diffuse = { 1, 1, 0, 1 };
	window.GetLightSource(7).GetParameters().Specular = { 1, 1, 0, 1 };
	window.GetLightSource(7).GetParameters().SpotCutoff = 15;
	window.GetLightSource(7).GetParameters().SpotExponent = 1;
	window.GetLightSource(7).GetParameters().Position = { 0, -3, -5, 1 };
	light_objects.push_back(tr);
	window.AddDrawable(scenes[5].GetModel<My::Points::Point3Norm<GLfloat>>(&program));
	tr = window.GetDrawableAs<My::Figures::Transformable>(12);
	tr->Rotate(M_PI / 2, glm::vec3(1, 0, 0));
	tr->Translate(glm::vec3(-10, 10, -5));
	tr->Scale(glm::vec3(2, 2, 2));
	window.AddDrawable(*window[12]);
	tr = window.GetDrawableAs<My::Figures::Transformable>(13);
	tr->Scale(glm::vec3(-1, -1, -1));
	My::Figures::BaseLine<My::Points::Point3<GLfloat>> line(&a_program);
	line.Ambient = { 1, 1, 1, 1 };
	line.SetPoints({ 
		{plane_x, plane_y, plane_z},
		{plane_x, plane_y, abs(plane_z)}
	});
	float step = abs(plane_x * 2) / (lines_count - 1);
	for (size_t i = 0; i < lines_count; ++i) {
		window.AddDrawable(line);
		window.GetDrawableAs<My::Figures::Transformable>(14 + i)->Translate(glm::vec3(i * step, 0, 0));
	}
	step = abs(plane_z * 2) / (lines_count - 1);
	line = My::Figures::BaseLine<My::Points::Point3<GLfloat>>(&a_program);
	line.SetPoints({
		{plane_x, plane_y, plane_z},
		{abs(plane_x), plane_y, plane_z}
	});
	line.Ambient = { 1, 1, 1, 1 };
	for (size_t i = 0; i < lines_count; ++i) {
		window.AddDrawable(line);
		window.GetDrawableAs<My::Figures::Transformable>(14 + i + lines_count)->Translate(glm::vec3(0, 0, i * step));
	}
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
				else if (event.key.code == sf::Keyboard::T)
					window.GetLightSource(1).Toggle();
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
		window.GetDrawableAs<My::Figures::Transformable>(12)->Rotate(0.001 * d_time, glm::vec3(0, 0, 1));
		window.GetDrawableAs<My::Figures::Transformable>(13)->Rotate(-0.001 * d_time, glm::vec3(0, 0, 1));
		window.Display();
	}
	return 0;
}