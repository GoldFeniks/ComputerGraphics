#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <unordered_map>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>
#include "figures.hpp"
#include "program.hpp"

namespace My {

	namespace Text {

		class Font {
			
		public:

			struct Character {

				unsigned int TextureId = 0, Advance;
				glm::ivec2 Size;
				glm::ivec2 Bearing;
				bool Loaded = false;

				Character(const Character&) = delete;
				Character& operator=(const Character&) = delete;

				Character(FT_Face face, unsigned char c) {
					if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
						return;
					}
					Size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
					Bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
					Advance = face->glyph->advance.x;
					glGenTextures(1, &TextureId);
					glBindTexture(GL_TEXTURE_2D, TextureId);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Size.x, Size.y, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}

				Character(Character&& other) {
					*this = std::move(other);
				}

				Character& operator=(Character&& other) {
					std::swap(TextureId, other.TextureId);
					Advance = other.Advance;
					Size = other.Size;
					Bearing = other.Bearing;
					Loaded = other.Loaded;
					return *this;
				}

				~Character() {
					glDeleteTextures(1, &TextureId);
				}

			};

			Font() = delete;
			Font(const Font&) = delete;
			Font& operator=(const Font&) = delete;

			Font(Font&& other) {
				*this = std::move(other);
			}

			Font& operator=(Font&& other) {
				std::swap(ft, other.ft);
				std::swap(face, other.face);
				return *this;
			}

			Font(std::string path, std::size_t font_size = 48) {
				FT_Init_FreeType(&ft);
				FT_New_Face(ft, path.c_str(), 0, &face);
				FT_Set_Pixel_Sizes(face, 0, font_size);
			};

			Character* GetCharacter(char c) {
				std::unordered_map<char, Character>::iterator it = characters.find(c);
				return it != characters.end() ? &(it->second) : &((*characters.insert(std::make_pair(c, Character(face, c))).first).second);
			}

			~Font() {
				FT_Done_Face(face);
				FT_Done_FreeType(ft);
			}

		private:

			FT_Library ft;
			FT_Face face;
			std::unordered_map<char, Character> characters;

		};

		class OnScreenText : public Figures::Drawable, public Figures::Transformable, protected Figures::DrawablesContainer {

		public:

			glm::vec4 Color = glm::vec4(1, 0, 1, 1);

			OnScreenText() = delete;
			OnScreenText(Font* font, Program* program, GLfloat width, GLfloat height) :
				font(font), Figures::Drawable(program), projection(glm::ortho(0.f, width, 0.f, height)) {};

			void SetText(std::string n_text) {
				text = n_text;
				changed = true;				
			}

			void SetWidthHeight(GLfloat width, GLfloat height) {
				projection = glm::ortho(0.f, width, 0.f, height);
			}

			void Draw() override {
				if (changed) {
					changed = false;
					deleteDrawables();
					GLfloat x = 0;
					for (std::string::iterator it = text.begin(); it != text.end(); ++it) {
						auto figure = My::Figures::IndexedFigure<My::Points::Point2Tex<GLfloat>>(program, 4, 6);
						Font::Character* c = font->GetCharacter(*it);
						GLfloat xpos = x + c->Bearing.x;
						GLfloat ypos = c->Bearing.y - c->Size.y;
						figure.SetPoints({
							{xpos, ypos + c->Size.y, 0, 0},
							{xpos, ypos, 0, 1},
							{xpos + c->Size.x, ypos, 1, 1},
							{xpos + c->Size.x, ypos + c->Size.y, 1, 0}
						});
						figure.SetIndices({0, 1, 2, 0, 2, 3});
						figure.DiffTexture = c->TextureId;
						AddDrawable(figure);
						x += c->Advance >> 6;
					}
				}
				for (vector_type::iterator it = Begin(); it != End(); ++it)
					(*it)->Draw();
			}

			void EnableProgram() override {
				program->Model = GetModelMatrix();
				program->Projection = projection;
				program->Enable();				
				glUniform4f(program->GetUniformLocation("texture_color"), Color.r, Color.g, Color.b, Color.a);
			}

			void Load() override {}

		protected:

			virtual Drawable* copy() const override {
				return new OnScreenText(*this);
			}

			virtual Drawable* move() override {
				return new OnScreenText(std::move(*this));
			}

		private:

			Font* font = nullptr;
			std::string text = "";
			bool changed = false;
			glm::mat4 projection;

		};

	}

}