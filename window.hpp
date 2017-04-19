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

	class Window {

	public:

		Window(size_t, size_t, std::string, sf::Uint32 = sf::Style::Default, sf::ContextSettings = sf::ContextSettings());
		~Window();

		void MainLoop();
		size_t AddCamera(Camera);
		void UseCamera(size_t);
		Camera& GetCurrentCamera();
		void AddLightSource(Lights::LightSource);
		Lights::LightSource& GetLightSource(size_t);
		size_t AddDrawable(Figures::Drawable*);
		Figures::Drawable* GetDrawable(size_t);
		bool IsOpen();
		bool PollEvent(sf::Event&);
		void Display();
		void Close();
		GLdouble Ratio();
		sf::Vector2u GetSize();
		Camera& GetCamera(size_t = 0);

	private:

		sf::Window* window;
		std::vector<Figures::Drawable*> drawables;
		std::vector<Camera> cameras;
		std::vector<Lights::LightSource> lights;
		size_t camera_index = 0;

	};

}// namespace My
