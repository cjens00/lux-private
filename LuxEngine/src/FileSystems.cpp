#include <FileSystems.h>
using namespace lux;

void file::LoadPNGImage(const flecs::world& world, const char* filename)
{
	int num_channels = 0;
	auto image_entity = world.entity();
	auto image = lux::components::Image{filename, 0, 0};
	image_entity.add<tags::isPNG>();

	FILE* imfile;
	fopen_s(&imfile, filename, "rb");

	if (imfile == nullptr)
	{
		std::cerr << std::format("Could not open [%s].", filename) << std::endl;
		image_entity.destruct();
	}
	else
	{
		auto stb_img = stbi_load_from_file(imfile, &image.width,
		                                   &image.height, &num_channels, 4);

		// Now load into GPU
		unsigned int glTextureID = 0;
		glGenTextures(1, &glTextureID);
		glBindTexture(GL_TEXTURE_2D, glTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, stb_img);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Free up the memory used by stb_image's loader
		stbi_image_free(stb_img);

		// Lastly, add the component with the finalized data
		image_entity.set<lux::components::Image>({
			filename, glTextureID, image.width, image.height
		});
	}
}
