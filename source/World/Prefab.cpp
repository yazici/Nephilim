#include <Nephilim/Razer/Prefab.h>
#include <Nephilim/Logger.h>

#include <pugixml/pugixml.hpp>

NEPHILIM_NS_BEGIN

bool Prefab::load(const String& filename)
{
	pugi::xml_document document;
	pugi::xml_parse_result result = document.load_file(filename.c_str());

	if (result)
	{
		pugi::xml_node root_node = document.child("root");
		if (root_node)
		{
			// Got the root node, probably a valid prefab file, let's parse
			auto spawn_nodes = root_node.children("spawn");
			int count = 0;
			for (auto it = spawn_nodes.begin(); it != spawn_nodes.end(); ++it)
			{
				count++;
				Spawn spawn;
				spawn.prefab = it->attribute("prefab").as_string();
				spawns.push_back(spawn);
			}

			Log("Found %d spawn nodes!", count);
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool Prefab::save(const String& filename)
{
	// Doesn't need implementation yet
	return false;
}
NEPHILIM_NS_END