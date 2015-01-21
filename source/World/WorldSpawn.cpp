//////////////////////////////////////////////////////////////////////////
// This file has the functions for spawning prefabs into the world

#include <Nephilim/World/World.h>
#include <Nephilim/Foundation/Logging.h>

NEPHILIM_NS_BEGIN

/// This is the most basic prefab instancing class
/// Returns a pointer to the allocated object if applicable
GameObject* World::spawnPrefab(const Prefab& prefab)
{
	GameObject* GeneratedObject = nullptr;

	String ClassName = prefab.getRootClassName();

	// We're spawning an actor directly
	if (ClassName == "Actor")
	{
		Actor* actor = spawnActor();

		for (auto it = prefab.objectData.cbegin(); it != prefab.objectData.cend(); ++it)
		{
			if (it->tag == "Sprite")
			{
				ASpriteComponent* spriteComponent = actor->createComponent<ASpriteComponent>();
				spriteComponent->setSize(it->get("width").toFloat(), it->get("height").toFloat());
				spriteComponent->tex = it->get("asset");
			}
		}

		GeneratedObject = actor;

		Log("Actor spawned from a prefab");
	}

	return GeneratedObject;
}

/// Variant that allows to spawn the entity/actor/etc in a given location and orientation directly
GameObject* World::spawnPrefab(const Prefab& prefab, Vector3D location, Quaternion orientation)
{
	if (prefab.getRootClassName() == "Entity")
	{
		Log("Spawning entity from prefab");
		Entity entity = createEntity();
		createComponent(CTransform(location), entity);
		createComponent(CColliderBox(), entity);

		StaticMesh* SM = new StaticMesh;
		SM->makeDebugBox(20, 20, 20);

		CStaticMesh csm;
		csm.setStaticMesh(SM);
		createComponent(csm, entity);
	}
	else if (prefab.getRootClassName() == "Actor")
	{

	}

	return nullptr;
}

/// Spawn a prefab definition directly into the world
GameObject* World::spawnPrefab(const String& prefabAsset, Vector3D location, Quaternion orientation)
{
	Prefab prefabDef;
	if (prefabDef.load("./" + prefabAsset + ".prefab"))
	{
		Log("Prefab %s loaded for spawning", prefabAsset.c_str());
	}
	return spawnPrefab(prefabDef, location, orientation);
}

NEPHILIM_NS_END