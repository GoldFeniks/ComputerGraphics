#pragma once
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include <vector>
#include "figures.hpp"
#include "camera.hpp"
#include "light.hpp"

namespace My {

	class Window : public Figures::DrawablesContainer {

	public:

		Window(size_t, size_t, std::string, sf::Uint32 = sf::Style::Default, sf::ContextSettings = sf::ContextSettings());
		~Window();

		void MainLoop();
		size_t AddCamera(Camera);
		void UseCamera(size_t);
		Camera& GetCurrentCamera();
		void AddLightSource(Lights::LightSource);
		Lights::LightSource& GetLightSource(size_t);
		bool IsOpen();
		bool PollEvent(sf::Event&);
		void Display();
		void Close();
		GLdouble Ratio();
		sf::Vector2u GetSize();
		Camera& GetCamera(size_t = 0);
		void IncScale(GLfloat step);
		void IncSize(GLfloat step);
		void SetType(GLint n_type);
		void IncPeriod(GLint step);

	private:

		sf::Window* window;
		std::vector<Camera> cameras;
		std::vector<Lights::LightSource> lights;
		size_t camera_index = 0;
		sf::Clock clock;
		GLint type = 0, period = 2500;
		GLfloat scale = 20, size = 2;

	};

}// namespace My
