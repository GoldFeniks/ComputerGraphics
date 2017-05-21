#define _USE_MATH_DEFINES
#define _SCL_SECURE_NO_WARNINGS
#define GL_MAX_LIGHTS 10
#include <glm/gtc/noise.hpp>
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
#include "text.hpp"
#include "utils.hpp"

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

My::Text::OnScreenText* text;
size_t light_index = 1;

void chooseLight(size_t i, My::Window* window) {
	light_index = i;
	std::string t = "Current light: #" + std::to_string(i) + " Type: ";
	switch (window->GetLightSource(i).GetType()) {
	case My::Lights::Type::Ambient:
		t += "Ambient CAN'T MOVE OR ROTATE";
		break;
	case My::Lights::Type::Direction:
		t += "Direction CAN'T MOVE OR ROTATE";
		break;
	case My::Lights::Type::Point:
		t += "Point CAN'T ROTATE";
		break;
	case My::Lights::Type::Spot:
		t += "Spot";
		break;
	}
	text->SetText(t);
}

void rotateLight(size_t index, GLfloat angle, glm::vec3 vec, My::Window* window) {
	window->GetLightSource(index).Rotate(angle, vec);
	if (light_objects[index] != nullptr)
		light_objects[index]->Rotate(angle, vec);
}

void moveLight(size_t index, glm::vec3 vec, My::Window* window) {
	window->GetLightSource(index).Translate(vec);
	if (light_objects[index] != nullptr)
		light_objects[index]->Translate(vec);
}

int main() {
	My::Window window(1500, 1500, "Triangle", sf::Style::Default, sf::ContextSettings(24, 8, 0, 4, 4));
	window.GetCurrentCamera().SetProjection(glm::radians(60.f), window.Ratio(), 0.01f, 10000.f);
	window.GetCurrentCamera().Setup(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0));
	My::Program text_program("shaders/text_vertex.glsl", "shaders/text_fragment.glsl");
	My::Text::Font font("fonts/papyrus.ttf", 64);
	My::Text::Font font1("fonts/bungeeshade.ttf", 80);
	My::Text::Font font2("fonts/bungeeinline.ttf", 80);
	light_objects.push_back(nullptr);
	window.AddLightSource(My::Lights::LightSource::Ambient());
	window.GetLightSource(0).GetParameters().Ambient = { 0.2f, 0.2f, 0.2f, 1 };
	window.AddLightSource(My::Lights::LightSource::Spot());
	light_objects.push_back(nullptr);
	window.GetLightSource(1).GetParameters().Position = { 0, 0, 1, 1 };
	window.GetLightSource(1).GetParameters().Diffuse = { 1, 1, 1, 1 };
	window.GetLightSource(1).GetParameters().Specular = { 0, 0, 0, 0 };
	window.GetLightSource(1).GetParameters().SpotDirection = { 1, 1, 1 };
	window.GetLightSource(1).GetParameters().SpotCutoff = 45;
	window.GetLightSource(1).GetParameters().SpotExponent = 10;
	window.GetLightSource(1).GetParameters().LinearAttenuation = 0;
	My::Program program("shaders/vertex.glsl", "shaders/fragment.glsl");
	My::Program a_program("shaders/light_source_vertex.glsl", "shaders/light_source_fragment.glsl");
	My::Program b_program("shaders/vertex.glsl", "shaders/function_fragment.glsl");
	My::Program lines_program("shaders/simple_vertex.glsl", "shaders/fragment_lines.glsl");
	My::Program squares_program("shaders/simple_vertex.glsl", "shaders/fragment_squares.glsl");
	My::Program circles_program("shaders/simple_vertex.glsl", "shaders/circles_fragment.glsl");
	My::Program waves_program("shaders/simple_vertex.glsl", "shaders/fragment_waves.glsl");
	//auto scenes = My::Scene::LoadScenes({ "test/models/BLEND/HUMAN.blend", "test/models/PLY/cube.ply", 
		//"test/models/Collada/sphere.dae", "test/dodecahedron_round.stl", "nanosuit/nanosuit.obj",
		//"test/Sphere slide.stl"
	//});
	My::Figures::IndexedFigure<My::Points::Point3NormTexTanBitan<GLfloat>> f(&lines_program, 4, 6);
	f.SetPoints({
		{-1,  1, 0,   0, 0, 1,   0, 1,   1, 0, 0,   0, 1, 0},
		{-1, -1, 0,   0, 0, 1,   0, 0,   1, 0, 0,   0, 1, 0},
		{ 1, -1, 0,   0, 0, 1,   1, 0,   1, 0, 0,   0, 1, 0},
		{ 1,  1, 0,   0, 0, 1,   1, 1,   1, 0, 0,   0, 1, 0}
	});
	f.SetIndices({0, 2, 1, 0, 3, 2});
	f.Specular = { 0, 0, 0, 0 };
	f.Shininess = 1;
	f.Scale(glm::vec3(10, 10, 10));
	window.AddDrawable(f);
	My::Scene earth("test/earth/earth.obj");
	auto em = earth.GetModel<My::Points::Point3NormTexTanBitan<GLfloat>>(&lines_program);
	em.Scale(glm::vec3(0.01, 0.01, 0.01));
	em.Translate(glm::vec3(15, 0, 0));
	window.AddDrawable(em);
	GLuint perlin_noise = My::Utils::NoiseTexture(256, 256, 10, true, time(NULL));
	GLuint simplex_noise = My::Utils::NoiseTexture(256, 256, 10, false, time(NULL));
	f.SetProgram(&program);
	f.Rotate(-M_PI / 2, glm::vec3(1, 0, 0));
	window.AddDrawable(f);
	auto t = window.GetDrawableAs<My::Figures::Transformable>(2);
	t->Translate(glm::vec3(-50, 0, 0));
	window.GetDrawableAs<My::Figures::IndexedFigure<My::Points::Point3NormTexTanBitan<GLfloat>>>(2)->NormTexture = perlin_noise;
	My::Scene cube("test/models/PLY/cube.ply");
	auto c = cube.GetModel<My::Points::Point3<GLfloat>>(&a_program);
	c.Translate(glm::vec3(-50, 3, 0));
	setAmbient(&c, { 1, 1, 1, 1 });
	window.AddDrawable(c);
	light_objects.push_back(window.GetDrawableAs<My::Figures::Transformable>(3));
	window.AddLightSource(My::Lights::LightSource::Point());
	window.GetLightSource(2).GetParameters().Position = { -50, 3, 0, 1 };
	em = earth.GetModel<My::Points::Point3NormTexTanBitan<GLfloat>>(&program);
	for (size_t i = 0; i < em.Size(); ++i) {
		em.GetDrawableAs<My::Figures::IndexedFigure<My::Points::Point3NormTexTanBitan<GLfloat>>>(i)->NormTexture = perlin_noise;
	}
	em.Translate(glm::vec3(-50, 0, -30));
	em.Scale(glm::vec3(0.05, 0.05, 0.05));
	window.AddDrawable(em);
	text = new My::Text::OnScreenText(&font2, &text_program, 1500, 1500);
	window.AddDrawable(text);
	text->Translate(glm::vec3(100, 100, 0));
	sf::Clock clock;
	c_time = clock.getElapsedTime().asMilliseconds();
	chooseLight(1, &window);
	while (window.IsOpen()) {
		sf::Event event;
		while (window.PollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.Close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape)
					window.Close();
				else if (event.key.code == sf::Keyboard::T)
					window.GetLightSource(light_index).Toggle();
				else if (event.key.code == sf::Keyboard::Equal && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
						window.IncSize(0.1);
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
						window.IncScale(0.1);
					else
						window.IncPeriod(100);
				else if (event.key.code == sf::Keyboard::Dash)
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
						window.IncSize(-0.1);
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
						window.IncScale(-0.1);
					else
						window.IncPeriod(-100);
			}
			if (event.type == sf::Event::Resized) {
				glViewport(0, 0, window.GetSize().x, window.GetSize().y);
				window.GetCurrentCamera().SetProjection(glm::radians(60.f), window.Ratio(), 0.01f, 10000.f);
				text->SetWidthHeight(window.GetSize().x, window.GetSize().y);
			}
		}
		unsigned long long d_time = (unsigned long long)clock.getElapsedTime().asMilliseconds() - c_time;
		c_time += d_time;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
					window.SetType(0);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
					window.SetType(1);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
					window.SetType(2);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
					window.SetType(3);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
					window.SetType(4);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
					window.SetType(5);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
					window.SetType(6);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
					window.SetType(7);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
					window.SetType(8);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
					window.SetType(9);
			}
			else {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
					chooseLight(0, &window);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
					chooseLight(1, &window);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
					chooseLight(2, &window);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
					chooseLight(3, &window);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
					chooseLight(4, &window);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
					chooseLight(5, &window);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
					chooseLight(6, &window);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
					chooseLight(7, &window);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
				window.GetDrawableAs<My::Figures::Drawable>(0)->SetProgram(&lines_program);
				window.GetDrawableAs<My::Figures::Drawable>(1)->SetProgram(&lines_program);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
				window.GetDrawableAs<My::Figures::Drawable>(0)->SetProgram(&squares_program);
				window.GetDrawableAs<My::Figures::Drawable>(1)->SetProgram(&squares_program);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
				window.GetDrawableAs<My::Figures::Drawable>(0)->SetProgram(&circles_program);
				window.GetDrawableAs<My::Figures::Drawable>(1)->SetProgram(&circles_program);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
				window.GetDrawableAs<My::Figures::Drawable>(0)->SetProgram(&waves_program);
				window.GetDrawableAs<My::Figures::Drawable>(1)->SetProgram(&waves_program);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			if (light_index == 1)
				window.GetCurrentCamera().RotateYZ(My::Camera::Default::Angle * d_time);
			else rotateLight(light_index, My::Camera::Default::Angle * d_time, glm::vec3(1, 0, 0), &window);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			if (light_index == 1)
				window.GetCurrentCamera().RotateYZ(-My::Camera::Default::Angle * d_time);
			else rotateLight(light_index, -My::Camera::Default::Angle * d_time, glm::vec3(1, 0, 0), &window);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			if (light_index == 1)
				window.GetCurrentCamera().RotateXZ(My::Camera::Default::Angle * d_time);
			else rotateLight(light_index, My::Camera::Default::Angle * d_time, glm::vec3(0, 1, 0), &window);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			if (light_index == 1)
				window.GetCurrentCamera().RotateXZ(-My::Camera::Default::Angle * d_time);
			else rotateLight(light_index, -My::Camera::Default::Angle * d_time, glm::vec3(0, 1, 0), &window);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			if (light_index == 1)
				window.GetCurrentCamera().RotateXY(My::Camera::Default::Angle * d_time);
			else rotateLight(light_index, My::Camera::Default::Angle * d_time, glm::vec3(0, 0, 1), &window);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			if (light_index == 1)
				window.GetCurrentCamera().RotateXY(-My::Camera::Default::Angle * d_time);
			else rotateLight(light_index, -My::Camera::Default::Angle * d_time, glm::vec3(0, 0, 1), &window);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			if (light_index == 1)
				window.GetCurrentCamera().MoveXZ(My::Camera::Default::Step * d_time);
			else moveLight(light_index, glm::vec3(1, 0, 0) * (My::Camera::Default::Step * d_time), &window);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			if (light_index == 1)
				window.GetCurrentCamera().MoveXZ(-My::Camera::Default::Step * d_time);
			else moveLight(light_index, glm::vec3(-1, 0, 0) * (My::Camera::Default::Step * d_time), &window);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			if (light_index == 1)
				window.GetCurrentCamera().MoveXY(My::Camera::Default::Step * d_time);
			else moveLight(light_index, glm::vec3(0, 0, 1) * (My::Camera::Default::Step * d_time), &window);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			if (light_index == 1)
				window.GetCurrentCamera().MoveXY(-My::Camera::Default::Step * d_time);
			else moveLight(light_index, glm::vec3(0, 0, -1) * (My::Camera::Default::Step * d_time), &window);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			if (light_index == 1)
				window.GetCurrentCamera().MoveYZ(-My::Camera::Default::Step * d_time);
			else moveLight(light_index, glm::vec3(0, -1, 0) * (My::Camera::Default::Step * d_time), &window);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
			if (light_index == 1)
				window.GetCurrentCamera().MoveYZ(My::Camera::Default::Step * d_time);
			else moveLight(light_index, glm::vec3(0, 1, 0) * (My::Camera::Default::Step * d_time), &window);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			window.GetCurrentCamera().Reset();
		//window.GetCurrentCamera().RotateYZ(My::Camera::Default::Angle * (mouse_y - sf::Mouse::getPosition().y) * sensativity);
		//window.GetCurrentCamera().RotateXZ(My::Camera::Default::Angle * (mouse_x - sf::Mouse::getPosition().x) * sensativity);
		//sf::Mouse::setPosition(sf::Vector2i(mouse_x, mouse_y));
		//window.GetDrawableAs<My::Figures::Transformable>(1)->Rotate(0.01, glm::vec3(0, 1, 0));
		glm::vec3 c_pos = window.GetCurrentCamera().GetEye();
		glm::vec3 c_dir = window.GetCurrentCamera().GetDirection();
		//text->SetText(std::to_string(c_time));
		window.GetLightSource(1).GetParameters().SpotDirection = { c_dir.x, c_dir.y, c_dir.z };
		window.GetLightSource(1).GetParameters().Position = { c_pos.x, c_pos.y, c_pos.z, 1 };
		window.Display();
	}
	//return 0;
}