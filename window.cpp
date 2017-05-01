#include "window.hpp"

My::Window::Window(size_t width, size_t height, std::string title, sf::Uint32 style, sf::ContextSettings context_settings) {
	AddCamera(Camera());
	window = new sf::Window(sf::VideoMode(width, height), title, style, context_settings);
	window->setMouseCursorVisible(false);
	window->setActive();
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Could not init GLEW");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glViewport(0, 0, window->getSize().x, window->getSize().y);
	glClearColor(0.f, 0.f, 0.f, 1.f);
}

My::Window::~Window() {
	delete window;
}

void My::Window::MainLoop() {
	
}

size_t My::Window::AddCamera(Camera camera) {
	cameras.push_back(camera);
	return cameras.size() - 1;
}

void My::Window::UseCamera(size_t index) {
	camera_index = index;
	GetCurrentCamera().SetRatio(Ratio());
}

My::Camera& My::Window::GetCurrentCamera() {
	return GetCamera(camera_index);
}

void My::Window::AddLightSource(Lights::LightSource light) {
	lights.push_back(light);
	lights.back().SetIndex(lights.size() - 1);
}

My::Lights::LightSource& My::Window::GetLightSource(size_t index) {
	return lights[index];
}

bool My::Window::IsOpen() {
	return window->isOpen();
}

bool My::Window::PollEvent(sf::Event& event) {
	return window->pollEvent(event);
}

void My::Window::Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 view = GetCurrentCamera().GetViewMatrix();
	glm::mat4 projection = GetCurrentCamera().GetProjectionMatrix();
	Camera c = GetCurrentCamera();
	for (vector_type::iterator it = Begin(); it != End(); ++it) {
		Program* program = (*it)->GetProgram();
		program->Projection = projection;
		program->View = view;
		(*it)->EnableProgram();
		program->LoadMVP();
		glUniform3f(glGetUniformLocation(program->Id, program->CameraAttribute.c_str()), c.GetEye().x, c.GetEye().y, c.GetEye().z);
		glUniform1i(glGetUniformLocation(program->Id, program->LightCountAttribute.c_str()), lights.size());
		for (std::vector<Lights::LightSource>::iterator l = lights.begin(); l != lights.end(); ++l) {
			l->Use(program);
		}
		(*it)->Draw();
		(*it)->DisableProgram();
	}
	window->display();
}

void My::Window::Close() {
	window->close();
}

GLdouble My::Window::Ratio() {
	return (GLfloat)window->getSize().x / window->getSize().y;
}

sf::Vector2u My::Window::GetSize() {
	return window->getSize();
}

My::Camera& My::Window::GetCamera(size_t index) {
	return cameras[index];
}
