// tinyobjloader - github.com/tinyobjloader/tinyobjloader
#define TINYOBJLOADER_IMPLEMENTATION
#include <Renderer/Model.h>

lux::Model::Model()
{
	file_name = {};
	meshes = {};
	materials = {};
}

bool lux::Model::LoadObj(std::string path_to_file)
{
	std::string warning_message;
	std::string error_message;

	std::string obj_file_path = std::string(path_to_file + ".obj");
	std::string mtl_file_path = std::string(path_to_file + ".mtl");

	tinyobj::ObjReader obj_reader;
	tinyobj::ObjReaderConfig obj_config;
	obj_config.triangulate = true;

	if (obj_reader.ParseFromFile(obj_file_path, obj_config))
	{
		num_objects = obj_reader.GetShapes().size();

		for (unsigned int i = 0; i < num_objects; i++)
		{
			Mesh new_obj_mesh{};
			new_obj_mesh.Initialize(i, std::move(obj_reader.GetAttrib()),
			                        std::move(obj_reader.GetMaterials()),
			                        std::move(obj_reader.GetShapes()));
			new_obj_mesh.LoadToGPU();
			meshes.emplace_back(new_obj_mesh);
		}
		return true;
	}
	else
	{
		return false;
	}
}

void lux::Model::Draw()
{
	for (auto mesh : meshes)
	{
		glBindVertexArray(mesh.GetVAO());
		glDrawElements(GL_TRIANGLES, mesh.GetNumIndices(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
}

/// -------------------------------------------------------------- ///
/// -------------------------------------------------------------- ///

void lux::Mesh::Initialize(int mesh_number,
                           const tinyobj::attrib_t&& attributes,
                           const std::vector<tinyobj::material_t>&& materials,
                           const std::vector<tinyobj::shape_t>&& shapes)
{
	name = shapes[mesh_number].name;
	const auto num_vertices = (attributes.vertices.size() / 3);

	for (int i = 0; i < (shapes[mesh_number].mesh.indices.size() / 3); i++)
	{
		indices.emplace_back(shapes[mesh_number].mesh.indices[i * 3].vertex_index);
	}

	for (glm::uint i = 0; i < num_vertices; i++)
	{
		vertices.emplace_back(Vertex
			{
				{
					attributes.vertices[(i * 3)],
					attributes.vertices[(i * 3) + 1],
					attributes.vertices[(i * 3) + 2]
				},
				{
					attributes.normals[(i * 3)],
					attributes.normals[(i * 3) + 1],
					attributes.normals[(i * 3) + 2]
				},
				{
					attributes.texcoords[(i * 2)],
					attributes.texcoords[(i * 2) + 1],
				}
			}
		);
	}
	std::cout << "debug" << std::endl;
}

void lux::Mesh::LoadToGPU()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
	             &indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

glm::uint lux::Mesh::GetVAO()
{
	return VAO;
}

glm::uint lux::Mesh::GetVBO()
{
	return VBO;
}

glm::uint lux::Mesh::GetEBO()
{
	return EBO;
}

glm::uint lux::Mesh::GetNumIndices()
{
	return indices.size();
}
