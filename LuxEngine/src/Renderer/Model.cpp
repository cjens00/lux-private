#include <Renderer/Model.h>

// tinyobjloader - github.com/tinyobjloader/tinyobjloader
#define TINYOBJLOADER_IMPLEMENTATION
#include "External/TinyOBJ/tiny_obj_loader.h"

lux::Model::Model(std::string filename)
{
	file_name = std::move(filename);
	meshes = {};
	materials = {};
}

bool lux::Model::LoadObj()
{
	std::string warning_message;
	std::string error_message;

	tinyobj::attrib_t obj_attributes;
	std::vector<tinyobj::material_t> obj_materials;
	std::vector<tinyobj::shape_t> obj_shapes;

	return tinyobj::LoadObj
	(
		&obj_attributes, &obj_shapes, &obj_materials,
		&warning_message, &error_message, this->file_name.c_str(),
		nullptr, true, true
	);
}

void lux::Model::Draw()
{
	// do the drawing
}
