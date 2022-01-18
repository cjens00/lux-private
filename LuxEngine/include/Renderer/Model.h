#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

#ifndef __glad_h_
#include <glad/glad.h>
#endif

#include <Renderer/Material.h>

// tinyobjloader - github.com/tinyobjloader/tinyobjloader
#ifndef TINY_OBJ_LOADER_H_
#include "External/TinyOBJ/tiny_obj_loader.h"
#endif

namespace lux
{
	class Model;
	class Mesh;
	struct Vertex;

	/// -------------------------------------------------------------- ///
	/// -------------------------------------------------------------- ///

	class Model
	{
		unsigned int num_objects = 0;
		std::string file_name = {};
		std::vector<std::string> names = {};
		std::vector<Mesh> meshes = {};
		std::vector<Material> materials = {};
	public:
		Model();
		void Draw();
		bool LoadObj(std::string path_to_file);
	};

	/// -------------------------------------------------------------- ///
	/// -------------------------------------------------------------- ///

	class Mesh
	{
		std::string name;

		// Indices
		std::vector<glm::u64> indices;

		// Vertices
		std::vector<Vertex> vertices;

		// Textures
		std::vector<Texture> textures;

		// Vertex Attribute Object ID
		GLuint VAO = 0;

		// Vertex Buffer Object ID
		GLuint VBO = 0;

		// Element Buffer Object ID
		GLuint EBO = 0;

	public:
		Mesh() = default;
		void Initialize(int mesh_number,
		                const tinyobj::attrib_t&& attributes,
		                const std::vector<tinyobj::material_t>&& materials,
		                const std::vector<tinyobj::shape_t>&& shapes);
		void LoadToGPU();

		// Getters
		glm::uint GetVAO();
		glm::uint GetVBO();
		glm::uint GetEBO();
		glm::uint GetNumIndices();
	};

	struct Vertex
	{
		glm::fvec3 Position;
		glm::fvec3 Normal;
		glm::fvec2 TexCoords;
	};
}
