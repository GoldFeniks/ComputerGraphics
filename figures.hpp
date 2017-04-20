#pragma once
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include "program.hpp"
#include <initializer_list>
#include <iostream>
#include "points.hpp"
#include "types.hpp"
#include "vao.hpp"
#include "vbo.hpp"
#include <utility>
#include <vector>
#include <type_traits>

namespace My {

	namespace Figures {

		class Drawable {

		public:

			Drawable(Program* program) : program(program) {};
			virtual ~Drawable() {};

			virtual Program* GetProgram() {
				return program;
			}

			virtual void SetProgram(Program* program) {
				this->program = program;
			}

			virtual void Draw() = 0;
			virtual void EnableProgram() = 0;
			virtual void Load() = 0;

			virtual void DisableProgram() {
				program->Disable();
			};

		protected:

			friend class DrawablesContainer;

			virtual Drawable* copy() const = 0;
			virtual Drawable* move() const = 0;

			Program* program;

		};

		class Transformable {

		public:

			virtual ~Transformable() {};

			virtual void ResetRotation() {
				rotation = glm::mat4(1.f);
			}

			virtual void ResetTranslation() {
				translation = glm::mat4(1.f);
			}

			virtual void ResetScale() {
				scale = glm::mat4(1.f);
			}

			virtual void Rotate(GLfloat angle, glm::vec3 vec) {
				rotation = glm::rotate(rotation, angle, vec);
			}

			virtual void Translate(glm::vec3 vec) {
				translation = glm::translate(translation, vec);
			}

			virtual void Scale(glm::vec3 vec) {
				scale = glm::scale(scale, vec);
			}

			virtual void SetRotation(GLfloat angle, glm::vec3 vec) {
				ResetRotation();
				Rotate(angle, vec);
			}

			virtual void SetTranslation(glm::vec3 vec) {
				ResetTranslation();
				Translate(vec);
			}

			virtual void SetScale(glm::vec3 vec) {
				ResetScale();
				Scale(vec);
			}

			glm::mat4 GetModelMatrix() {
				return translation * scale * rotation;
			}

		protected:

			glm::mat4 rotation = glm::mat4(1.f), translation = glm::mat4(1.f), scale = glm::mat4(1.f);

		};

		template<typename T>
		class Figure : public Drawable, public Transformable {

		public:

			typedef T point_type;
			typedef typename T::value_type value_type;
			typedef size_t index_type;

			struct Material {

				My::Vectors::Vector4<GLfloat> Ambient = { 0.2, 0.2, 0.2, 1 }, Diffuse = { 0.8, 0.8, 0.8, 1 }, Specular = { 0.2, 0.2, 0.2, 1 };
				GLfloat Shininess = 0.5;

			};

			GLenum Target = GL_ARRAY_BUFFER, Mode = GL_TRIANGLES, Usage = GL_STATIC_DRAW;
			GLuint DiffTexture = 0, SpecTexture = 0, NormTexture = 0;

			Figure(Program* program, index_type points_count) : Drawable(program), points_count(points_count) {
				points = new T[points_count];
			};

			Figure(const Figure<T>& other) : Drawable(other), Transformable(other) {
				*this = other;
			}

			Figure(Figure<T>&& other) : Drawable(other), Transformable(other) {
				*this = std::move(other);
			}

			Figure<T>& operator=(const Figure<T>& other) {
				Drawable::operator=(other);
				Transformable::operator=(other);
				freePoints();
				copyFrom(other);
				points = new T[points_count];
				SetPoints(other.points, other.points + other.points_count);
				return *this;
			}

			Figure<T>& operator=(Figure<T>&& other) {
				Drawable::operator=(std::move(other));
				Transformable::operator=(std::move(other));
				std::swap(points, other.points);
				std::swap(vbo, other.vbo);
				std::swap(vao, other.vao);
				copyFrom(other);
				Load();
				return *this;
			}
			
			virtual ~Figure() {
				freePoints();
			};

			virtual void Load() override {
				vao.Bind();
				vbo.BufferData(Target, sizeof(point_type) * points_count, points, Usage);
				size_t offset = 0;
				setAttribPointer(program->PointAttribute, Points::Size<point_type>::PointSize, offset);
				setAttribPointer(program->ColorAttribute, Points::Size<point_type>::ColorSize, offset);
				setAttribPointer(program->NormalAttribute, Points::Size<point_type>::NormalSize, offset);
				setAttribPointer(program->TextureAttribute, Points::Size<point_type>::TextureSize, offset);
				setAttribPointer(program->TangentAttribute, Points::Size<point_type>::TangentSize, offset);
				setAttribPointer(program->BitangentAttribute, Points::Size<point_type>::BitangentSize, offset);
				vao.Unbind();
				loaded = true;
			}

			void SetPoints(std::initializer_list<point_type> list) {
				std::initializer_list<point_type>::iterator it = list.begin();
				for (index_type i = 0; i < points_count; ++i)
					new (points + i) point_type(*(it + i));
				Load();
			}

			template<typename It>
			void SetPoints(It from, It to, index_type i = 0) {
				for (It it = from; it != to; ++it)
					new (points + i++) point_type(*it);
				Load();
			}

			point_type& operator[](index_type index) {
				return points[index];
			}

			virtual void EnableProgram() override {
				program->Enable();
				program->Model = GetModelMatrix();
			}

			virtual void Draw() override {
				vao.Bind();
				loadMaterial();
				loadTextures();
				glDrawArrays(Mode, 0, points_count);
				vao.Unbind();
			}

			Material& GetMaterial() {
				return material;
			}

			point_type* GetPoints() {
				return points;
			}

			bool IsLoaded() {
				return loaded;
			}

		protected:

			GLenum type = Types::Type<value_type>::GLType;
			VAO vao;
			VBO vbo;
			Material material;
			bool loaded = false;
			point_type* points = nullptr;
			index_type points_count = 0;

			void setAttribPointer(std::string name, size_t size, size_t& offset) {
				if (size) {
					vbo.SetVertexAttribPointer(program->GetAttributeLocation(name), size, type, GL_FALSE, sizeof(point_type), (void*)(offset * sizeof(value_type)));
					offset += size;
				}
			}

			void loadTexture(GLuint texture, size_t index, std::string texture_name, std::string bool_name) {
				if (texture) {
					glActiveTexture(GL_TEXTURE0 + index);
					glUniform1i(program->GetUniformLocation(texture_name.c_str()), index);
					glUniform1i(program->GetUniformLocation(bool_name.c_str()), 1);
					glBindTexture(GL_TEXTURE_2D, texture);
				}
				else {
					glUniform1i(program->GetUniformLocation(bool_name.c_str()), 0);
				}
			}

			void copyFrom(const Figure<T>& other) {
				points_count = other.points_count;
				type = other.type;
				material = other.material;
				loaded = other.loaded;
				Target = other.Target;
				Mode = other.Mode;
				Usage = other.Usage;
				DiffTexture = other.DiffTexture;
				SpecTexture = other.SpecTexture;
				NormTexture = other.NormTexture;
			}

			void freePoints() {
				for (index_type i = 0; i < points_count; ++i)
					points[i].~T();
				delete points;
			}

			void loadMaterial() {
				glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat*)&material.Ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat*)&material.Diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat*)&material.Specular);
				glUniform1f(program->GetUniformLocation(program->ShininessUniform), material.Shininess);
			}


			void loadTextures() {
				loadTexture(DiffTexture, 0, program->DiffuseTextureAttribute, program->HasDiffuseTextureAttribute);
				loadTexture(SpecTexture, 1, program->SpecularTextureAttribute, program->HasSpecularTextureAttribute);
				loadTexture(NormTexture, 2, program->NormalAttribute, program->HasNormalTextureAttribute);
			}

			virtual Drawable* copy() const override {
				return new Figure<T>(*this);
			}

			virtual Drawable* move() const override {
				return new Figure<T>(std::move(*this));
			}

		}; 

		template<typename T>
		class BaseTriangle : public Figure<T> {

		public:

			BaseTriangle(Program* program) : Figure<T>(program, 3) {};

		};

		template<typename T>
		class BaseLine : public Figure<T> {

		public:

			BaseLine(Program* program) : Figure<T>(program, 2) { Mode = GL_LINES; };

		};

		template<typename T>
		class IndexedFigure : public Figure<T> {

		public:

			IndexedFigure(Program* program, index_type points_count, GLsizei indices_count) : Figure<T>(program, points_count), indices_count(indices_count) {
				indices = new GLuint[indices_count];
			};

			IndexedFigure(const IndexedFigure<T>& other) : Figure<T>(other) {
				*this = other;
			}

			IndexedFigure(IndexedFigure<T>&& other) : Figure<T>(other) {
				*this = std::move(other);
			}

			IndexedFigure<T>& operator=(const IndexedFigure<T>& other) {
				Figure<T>::operator=(other);
				delete indices;
				indices_count = other.indices_count;
				indices = new GLuint[indices_count];
				for (index_type i = 0; i < indices_count; ++i)
					indices[i] = other.indices[i];
				return *this;
			}

			IndexedFigure<T>& operator=(IndexedFigure<T>&& other) {
				Figure<T>::operator=(std::move(other));
				std::swap(indices, other.indices);
				indices_count = other.indices_count;
				return *this;
			}

			GLuint* GetIndices() {
				return indices;
			}

			void SetIndices(std::initializer_list<GLuint> list) {
				std::initializer_list<GLuint>::iterator it = list.begin();
				for (index_type i = 0; i < indices_count; ++i)
					indices[i] = *(it + i);
			}

			template<typename It>
			void SetIndices(It from, It to, index_type i = 0) {
				for (It it = from; it != to; ++it)
					indices[i++] = *it;
			}

			virtual void Draw() override {
				vao.Bind();
				loadMaterial();
				loadTextures();
				glDrawElements(Mode, indices_count, GL_UNSIGNED_INT, indices);
				vao.Unbind();
			}

		protected:

			GLuint* indices = nullptr;
			GLsizei indices_count = 0;

			virtual Drawable* copy() const override {
				return new IndexedFigure(*this);
			}

			virtual Drawable* move() const override {
				return new IndexedFigure(std::move(*this));
			}
		};

		class DrawablesContainer {

		public:

			typedef std::vector<Drawable*> vector_type;

			DrawablesContainer() {};
			DrawablesContainer(const DrawablesContainer& other) {
				*this = other;
			};

			DrawablesContainer(DrawablesContainer&& other) {
				*this = std::move(other);
			};
			~DrawablesContainer() {
				deleteDrawables();
			}

			DrawablesContainer& operator=(const DrawablesContainer& other) {
				deleteDrawables();
				drawables.resize(other.Size());
				for (size_t i = 0; i < other.Size(); ++i)
					drawables[i] = other.drawables[i]->copy();
				return *this;
			};

			DrawablesContainer& operator=(DrawablesContainer&& other) {
				std::swap(drawables, other.drawables);
				return *this;
			}

			template<typename T>
			void SetFigures(T from, T to) {
				drawables.assign(from, to);
			}

			template<typename T>
			void SetFigures(const std::vector<T>& source) {
				deleteDrawables();
				drawables.resize(source.size());
				for (size_t i = 0; i < source.size(); ++i)
					drawables[i] = new T(source[i]);
			}

			template<typename T>
			void SetFigures(std::vector<T>&& source) {
				deleteDrawables();
				drawables.resize(source.size());
				for (size_t i = 0; i < source.size(); ++i)
					drawables[i] = new T(std::move(source[i]));
			}

			void AddDrawable(Drawable& drawable) {
				drawables.push_back(drawable.copy());
				drawables.back()->Load();
			}

			void AddDrawable(Drawable&& drawable) {
				drawables.push_back(drawable.move());
				drawables.back()->Load();
			}

			void DeleteDrawable(size_t i) {
				drawables[i]->~Drawable();
				delete drawables[i];
				drawables.erase(drawables.begin() + i);
			}

			void LoadFigures() {
				for (vector_type::iterator it = Begin(); it != End(); ++it)
					(*it)->Load();
			}

			vector_type::iterator Begin() {
				return drawables.begin();
			}

			vector_type::iterator End() {
				return drawables.end();
			}

			std::vector<Drawable*>& GetDrawables() {
				return drawables;
			}

			Drawable* operator[](size_t i) {
				return drawables[i];
			}

			size_t Size() const {
				return drawables.size();
			}

		protected:

			std::vector<Drawable*> drawables;

			void deleteDrawables() {
				for (std::vector<Drawable*>::iterator it = drawables.begin(); it != drawables.end(); ++it) {
					(*it)->~Drawable();
					delete *it;
				}
				drawables.resize(0);
			}

		};

		class Model : public Drawable, public Transformable, public DrawablesContainer {

		public:

			Model(Program* program) : Drawable(program) {};

			Model(const Model& other) : Drawable(other) {
				*this = other;
			};

			Model(Model&& other) : Drawable(std::move(other)) {
				*this = std::move(other);
			}

			Model& operator=(const Model& other) {
				Drawable::operator=(other);
				Transformable::operator=(other);
				DrawablesContainer::operator=(other);
				return *this;
			};

			Model& operator=(Model&& other) {
				Drawable::operator=(std::move(other));
				Transformable::operator=(std::move(other));
				DrawablesContainer::operator=(std::move(other));
				return *this;
			}

			virtual void EnableProgram() override {
				program->Enable();
				program->Model = GetModelMatrix();
			}			

			virtual void Draw() override {
				for (vector_type::iterator it = Begin(); it != End(); ++it)
					(*it)->Draw();
			}

			virtual void Load() override {
				LoadFigures();
			};

		protected:

			virtual Drawable* copy() const override {
				return new Model(*this);
			}

			virtual Drawable* move() const override {
				return new Model(std::move(*this));
			}

		};

	}// namespace Figures

}// namespace My
