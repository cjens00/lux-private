#include <Core/Scene.h>


lux::Scene::Scene()
{
	Model model = {};
	model.LoadObj("res/assets/models/obj/default");
	models.emplace_back(std::move(model));
}

void lux::Scene::SubmitDrawData()
{
	glClearColor
	(
		env_color.r, env_color.g,
		env_color.b, env_color.a
	);
	for (auto& model : models)
	{
		model.Draw();
	}
}

bool lux::Scene::LoadFromFile()
{
	// TODO: implement
	return false;
}

bool lux::Scene::WriteToFile()
{
	// TODO: implement
	return false;
}
