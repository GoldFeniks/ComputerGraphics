#pragma once
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <string>
#include <utility>

namespace My {

	class Program {

	public:

		GLuint Id;
		enum class States { OK = 0, FAIL = 1 };
		States State = States::OK;
		glm::mat4 Projection = glm::mat4(1.f), Model = glm::mat4(1.f), View = glm::mat4(1.f);

		std::string PointAttribute = "position",
			ColorAttribute = "in_color",
			ProjectionAttribute = "projection",
			ViewAttribute = "view",
			ModelAttribute = "model",
			LightTypeAttribute = "light_type",
			NormalAttribute = "vertex_normal",
			CameraAttribute = "camera_pos",
			TextureAttribute = "texture_position",
			LightCountAttribute = "light_count",
			TangentAttribute = "vertex_tangent",
			BitangentAttribute = "vertex_bitangent",
			DiffuseTextureAttribute = "diff",
			HasDiffuseTextureAttribute = "has_diff_texture",
			SpecularTextureAttribute = "spec",
			HasSpecularTextureAttribute = "has_spec_texture",
			NormalTextureAttribute = "norm",
			HasNormalTextureAttribute = "has_norm_texture",
			ShininessUniform = "shininess";

		Program() : Id(glCreateProgram()) {};
		Program(std::string vertex_shader, std::string fragment_shader);
		Program(const Program&) = delete;
		Program(Program&& other);

		Program& operator=(const Program&) = delete;
		Program& operator=(Program&& other);

		~Program() { glDeleteProgram(Id); };
	
		void SetShader(std::string, GLuint);
		void LoadMVP();
		void Link();
		void Enable();
		static void Disable();
		bool IsOk();
		GLint GetUniformLocation(std::string);
		GLint GetAttributeLocation(std::string);

	};

}// namespace My
