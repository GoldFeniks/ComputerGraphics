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
	My::Program program;
	program.SetShader("shaders/vertex.glsl", GL_VERTEX_SHADER);
	program.SetShader("shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	program.Link();
	My::Figures::Figure<My::Points::Point3RGB<GLfloat>> f(&program, 0);
	My::Figures::Figure<My::Points::Point3RGB<GLfloat>> f1 = std::move(f);
	My::Figures::Triangle3RGB<GLfloat> bottom_side(&program);
	bottom_side.SetPoints({
		{1, -std::sqrtf(3) / 2, 0, 0.1f, 0, 0},
		{std::cosf(M_PI * 2 / 3), -std::sqrtf(3) / 2, std::sinf(M_PI * 2 / 3), 0.1f, 0, 0},
		{std::cosf(-M_PI * 2 / 3), -std::sqrtf(3) / 2, std::sinf(-M_PI * 2 / 3), 0.1f, 0, 0 },
	});
	My::Figures::Figure<My::Points::Point3RGB<GLfloat>> dot(&program, 1);
	dot.SetPoints({ {0, 0, 0, 1, 1, 1} });
	dot.Mode = GL_POINTS;
	window.AddDrawable((My::Figures::Drawable*)&dot);
	bottom_side.Rotate(0.5, glm::vec3(1, 0, 0));
	My::Figures::IndexedFigure<My::Points::Point3RGB<GLfloat>> fig(&program, 3, 3);
	fig.SetPoints({
		{ 0.32f, 14.05f, 0.5f, 1.f, 0, 0 },
		{ 0.38f, 13.98f, 0.44f, 1.f, 0, 0 },
		{ 0.41f, 14.11f, 0.46f, 1.f, 0, 0 },
		/*{ -6.54f, 0, 2.5f, 1.f, 0, 0},
		{ -6.54f, 0, -2.5, 1.f, 0, 0}*/
	});
	fig.SetIndices({ 0, 1, 2 });
	fig.Scale(glm::vec3(10, 10, 10));
	//window.AddDrawable((My::Figures::Drawable*)(&fig));
	//window.AddDrawable((My::Figures::Drawable*)(&bottom_side));
	/*std::vector<My::Figures::IndexedFigure<My::Points::Point3RGBTexNorm<GLfloat>>> vec = My::Loader<GLfloat>::Load("test/models/BLEND/HUMAN.blend", &program);
	for (std::vector<My::Figures::IndexedFigure<My::Points::Point3RGBTexNorm<GLfloat>>>::iterator it = vec.begin(); it != vec.end(); ++it) {
		for (int i = 0; i < it->points_count; ++i) {
			file << it->points[i].Point.x << " " << it->points[i].Point.y << " " << it->points[i].Point.z << " " << std::endl;
		}
		it->Load();
		window.AddDrawable((My::Figures::Drawable*)(&(*it)));
	}*/
	//My::Scene scene("nanosuit/nanosuit.obj");
	My::Scene scene("test/models/BLEND/HUMAN.blend");
	//My::Scene scene2("test/models/PLY/cube.ply");
	auto vec1 = scene.GetFigures<My::Points::Point3TexNormTangents<GLfloat>>(&program);
	auto model = scene.GetModel<My::Points::Point3TexNormTangents<GLfloat>>(&program);
	model.Rotate(-M_PI / 2, glm::vec3(1, 0, 0));
	window.AddDrawable((My::Figures::Drawable*)&model);
	auto fig1 = vec1[vec1.size() - 2];
	//fig1.Load();
	auto fig2 = vec1.back();
	//fig2.Load();
	//window.AddDrawable((My::Figures::Drawable*)&fig1);
	//window.AddDrawable((My::Figures::Drawable*)&fig2);
	//auto figu = vec1[0];
	//figu->Load();
	//vec1[0]->Load();
	//window.AddDrawable((My::Figures::Drawable*)vec1[0]);
	//vec1[1]->Load();
	//std::vector<My::Figures::IndexedFigure<My::Points::Point3RGBTexNormTangents<GLfloat>>*> vec1 = My::Loader<GLfloat>::Load("nanosuit/nanosuit.obj", &program);
	for(auto it = vec1.begin(); it != vec1.end(); ++it) {
		(*it).Load();
		//(*it)->Mode = GL_LINE_LOOP;
		//(*it)->Translate(glm::vec3(10, 0, 0));
		//(*it)->Translate(glm::vec3(10, 10, 10));
		//window.AddDrawable((My::Figures::Drawable*)&(*it));
	}
	//vec[3].Load();
	//window.AddDrawable((My::Figures::Drawable*)(&vec[0]));
	/*My::Points::Point3RGBTexNorm<GLfloat>* p = vec[0]->GetPoints();
	GLuint* ind = vec[0]->GetIndices();*/
	//window.AddDrawable((My::Figures::Drawable*)&(fig));
	/*My::Figures::Triangle3RGB<GLfloat> triangle(&program);
	triangle.SetPoints({
		{ 0, std::sqrtf(3) / 2, 0, 0, 0.1, 0 },
		{ std::cosf(M_PI * 2 / 3), -std::sqrtf(3) / 2, std::sinf(M_PI * 2 / 3), 0, 0.1, 0 },
		{ std::cosf(-M_PI * 2 / 3), -std::sqrtf(3) / 2, std::sinf(-M_PI * 2 / 3), 0, 0.1, 0 },
	});
	My::Figures::Triangle3RGB<GLfloat> triangle1(&program);
	triangle1.SetPoints({
		{ 1, -std::sqrtf(3) / 2, 0, 0, 0, 0.1 },
		{ 0, std::sqrtf(3) / 2, 0, 0, 0, 0.1 },
		{ std::cosf(-M_PI * 2 / 3), -std::sqrtf(3) / 2, std::sinf(-M_PI * 2 / 3), 0, 0, 0.1 },
	});
	My::Figures::Triangle3RGB<GLfloat> triangle2(&program);
	triangle2.SetPoints({
		{ 1, -std::sqrtf(3) / 2, 0, 0.1, 0, 0.1 },
		{ std::cosf(M_PI * 2 / 3), -std::sqrtf(3) / 2, std::sinf(M_PI * 2 / 3), 0.1, 0, 0.1 },
		{ 0, std::sqrtf(3) / 2, 0, 0.1, 0, 0.1 },
	});*/
	sf::Clock clock;
	c_time = clock.getElapsedTime().asMilliseconds();
	//window.AddDrawable((My::Figures::Drawable*)(&bottom_side));
	//window.AddDrawable((My::Figures::Drawable*)(&dot));
	/*window.AddDrawable((My::Figures::Drawable*)(&triangle1));
	window.AddDrawable((My::Figures::Drawable*)(&triangle2));
	window.AddDrawable((My::Figures::Drawable*)(&bottom_side));*/
	/*float step_x = std::abs(2 * plane_x) / (lines_count - 1), step_z = std::abs(2 * plane_z) / (lines_count - 1);
	for (int i = 0; i < lines_count; ++i) {
		My::Figures::Line3RGB<GLfloat> *line = new My::Figures::Line3RGB<GLfloat>(&program);
		line->SetPoints({
			{plane_x + i * step_x, plane_y, plane_z, 1, 1, 1 },
			{plane_x + i * step_x, plane_y, -plane_z, 1, 1, 1 }
		});
		window.AddDrawable((My::Figures::Drawable*)(line));
		line = new My::Figures::Line3RGB<GLfloat>(&program);
		line->SetPoints({
			{ plane_x, plane_y, plane_z + i * step_z, 1, 1, 1 },
			{ -plane_x, plane_y, plane_z + i * step_z, 1, 1, 1 }
		});
		window.AddDrawable((My::Figures::Drawable*)(line));
		line = new My::Figures::Line3RGB<GLfloat>(&program);
		line->SetPoints({
			{ plane_x, plane_y, plane_z + i * step_z, 1, 1, 1 },
			{ plane_x + i * step_x, plane_y, plane_z, 1, 1, 1 }
		});
		window.AddDrawable((My::Figures::Drawable*)(line));
		line = new My::Figures::Line3RGB<GLfloat>(&program);
		line->SetPoints({
			{ -plane_x, plane_y, -plane_z - i * step_z, 1, 1, 1 },
			{ -plane_x - i * step_x, plane_y, -plane_z, 1, 1, 1 }
		});
		window.AddDrawable((My::Figures::Drawable*)(line));
		line = new My::Figures::Line3RGB<GLfloat>(&program);
		line->SetPoints({
			{ plane_x, plane_y, -plane_z - i * step_z, 1, 1, 1 },
			{ plane_x + i * step_x, plane_y, -plane_z, 1, 1, 1 }
		});
		window.AddDrawable((My::Figures::Drawable*)(line));
		line = new My::Figures::Line3RGB<GLfloat>(&program);
		line->SetPoints({
			{ -plane_x, plane_y, plane_z + i * step_z, 1, 1, 1 },
			{ -plane_x - i * step_x, plane_y, plane_z, 1, 1, 1 }
		});
		window.AddDrawable((My::Figures::Drawable*)(line));
	}*/
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