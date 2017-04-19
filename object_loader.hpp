#pragma once
#include <GL/glew.h>
#include <SOIL.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <vector>
#include "figures.hpp"
#include "program.hpp"
#include <string>
#include "points.hpp"
#include "window.hpp"
#include <utility>
#include <initializer_list>

namespace My {

	class Scene {

	public:

		template<typename T>
		class Converter {

		public:

			typedef T point_type;
			typedef typename point_type::value_type value_type;
			typedef Figures::IndexedFigure<point_type> figure_type;
			typedef std::vector<figure_type> vector_type;

			static vector_type GetFigures(const Scene* scene, Program* program) {
				vector_type result;
				for (size_t i = 0; i < scene->scene->mNumMeshes; ++i)
					result.push_back(loadMesh(scene, scene->scene->mMeshes[i], program));
				return result;
			}

		private:

			static void setPointProperty(size_t size, value_type* point, std::initializer_list<value_type> value, size_t& offset) {
				std::initializer_list<value_type>::iterator it = value.begin();
				if (size) {
					for (size_t i = 0; i < size; ++i)
						new (point + offset + i) value_type(*(it + i));
					offset += size;
				}
			}

			static figure_type loadMesh(const Scene* scene, const aiMesh* mesh, Program* program) {
				figure_type figure(program, mesh->mNumVertices, mesh->mNumFaces * 3);
				for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
					point_type* point = figure.GetPoints() + i;
					size_t offset = 0;
					setPointProperty(Points::Size<point_type>::PointSize, (value_type*)point,
						{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z }, offset);
					if (mesh->mColors[0])
						setPointProperty(Points::Size<point_type>::ColorSize, (value_type*)point,
						{ mesh->mColors[0][i].r,  mesh->mColors[0][i].g, mesh->mColors[0][i].b }, offset);
					else offset += Points::Size<point_type>::ColorSize;
					if (mesh->mNormals)
						setPointProperty(Points::Size<point_type>::NormalSize, (value_type*)point,
						{ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z }, offset);
					else offset += Points::Size<point_type>::NormalSize;
					if (mesh->HasTextureCoords(0))
						setPointProperty(Points::Size<point_type>::TextureSize, (value_type*)point,
						{ mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y }, offset);
					else offset += Points::Size<point_type>::TextureSize;
					if (mesh->mTangents)
						setPointProperty(Points::Size<point_type>::TangentSize, (value_type*)point,
						{ mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z }, offset);
					else offset += Points::Size<point_type>::TangentSize;
					if (mesh->mBitangents)
						setPointProperty(Points::Size<point_type>::BitangentSize, (value_type*)point,
						{ mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z }, offset);
					else offset += Points::Size<point_type>::BitangentSize;
				}
				for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
					GLuint* index = figure.GetIndices() + i * 3;
					for (unsigned int j = 0; j < 3; ++j)
						*(index + j) = mesh->mFaces[i].mIndices[j];
				}
				typename figure_type::Material& material = figure.GetMaterial();
				aiMaterial* pMaterial = scene->scene->mMaterials[mesh->mMaterialIndex];
				aiColor3D color;
				pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
				material.Ambient = { color.r, color.b, color.g, 1 };
				pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
				material.Diffuse = { color.r, color.b, color.g, 1 };
				pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
				material.Specular = { color.r, color.b, color.g, 1 };
				pMaterial->Get(AI_MATKEY_SHININESS, material.Shininess);
				figure.DiffTexture = scene->diff_textures[mesh->mMaterialIndex];
				figure.SpecTexture = scene->spec_textures[mesh->mMaterialIndex];
				figure.NormTexture = scene->norm_textures[mesh->mMaterialIndex];
				return figure;
			}

		};

		Scene(std::string file_path) {
			importer = new Assimp::Importer;
			scene = importer->ReadFile(file_path.c_str(), aiProcess_Triangulate | aiProcess_CalcTangentSpace
				| aiProcess_FlipUVs | aiProcess_GenNormals );
			dir = file_path;
			dir.resize(file_path.rfind("/"));
			dir += "/";
			loadTextures();
		}

		Scene& operator=(const Scene& other) {
			importer = new Assimp::Importer(*other.importer);
			scene = other.scene;
			dir = other.dir;
			diff_textures = other.diff_textures;
			spec_textures = other.spec_textures;
			norm_textures = other.norm_textures;
		}

		Scene& operator=(Scene&& other) {
			std::swap(importer, other.importer);
			std::swap(scene, other.scene);
			std::swap(dir, other.dir);
			std::swap(diff_textures, other.diff_textures);
			std::swap(spec_textures, other.spec_textures);
			std::swap(norm_textures, other.norm_textures);
		}

		~Scene() {
			delete importer;
		}

		template<typename T>
		typename Converter<T>::vector_type GetFigures(Program* program) {
			return Converter<T>::GetFigures(this, program);
		}

		template<typename T>
		Figures::Model GetModel(Program* program) {
			Figures::Model result(program);
			result.SetFigures(GetFigures<T>(program));
			return result;
		}

	private:

		Assimp::Importer* importer;
		const aiScene* scene;
		std::string dir;
		std::vector<GLuint> diff_textures, spec_textures, norm_textures;

		void loadTextures() {
			diff_textures.resize(scene->mNumMaterials, 0);
			spec_textures.resize(scene->mNumMaterials, 0);
			norm_textures.resize(scene->mNumMaterials, 0);
			for (int i = 0; i < scene->mNumMaterials; ++i) {
				aiMaterial* material = scene->mMaterials[i];
				diff_textures[i] = loadTexture(aiTextureType_DIFFUSE, material);
				spec_textures[i] = loadTexture(aiTextureType_SPECULAR, material);
				//norm_textures[i] = loadTexture(aiTextureType_HEIGHT, material);
			}
		}

		GLuint loadTexture(aiTextureType type, aiMaterial* material) {
			GLuint result = 0;
			aiString path;
			if (material->GetTextureCount(type) > 0)
				if (material->GetTexture(type, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
					result = loadTextureFromFile(dir + std::string(path.data));
			return result;
		}

		static GLuint loadTextureFromFile(std::string path) {
			GLuint textureId;
			glGenTextures(1, &textureId);
			int width, height;
			unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			SOIL_free_image_data(image);
			return textureId;
		}

	};

}//namepsace My