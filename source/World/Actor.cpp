#include <Nephilim/World/Actor.h>
#include <Nephilim/World/World.h>

NEPHILIM_NS_BEGIN

/// Allows Actor subclasses to do their own logic
void Actor::update(const Time& deltaTime)
{

}

/// Get the world instance that created this actor
World* Actor::getWorld()
{
	return _world;
}

/// Get the bounding box of this actor and its contents
BBox Actor::getActorBounds()
{
	Vector3D extents;

	/*if (dynamic_cast<ASpriteComponent*>(root))
	{
		extents.z = 0.f;
		extents.x = static_cast<ASpriteComponent*>(root)->getSize().x;
		extents.y = static_cast<ASpriteComponent*>(root)->getSize().y;
	}*/


	BBox box;
	box.parameters[0] = getActorLocation();
	box.parameters[1] = extents;
	return box;
}

/// Get the root component
ASceneComponent* Actor::getRootComponent()
{
	return root;
}

/// Set a new root component to this hierarchy
void Actor::setRootComponent(ASceneComponent* component)
{
	root = component;
}

/// Update the local components to make sure they are positioned relative
void Actor::computeTransformHierarchy()
{
	if (root)
	{
		root->updateTransforms();
	}
}

/// Destroy this actor
/// As soon as this function is called, the Actor object may not be used anymore
void Actor::destroy()
{
	if (_world)
	{
		_world->destroyActor(this);
	}
}

/// Get the transform of this Actor
/// This is the transform that converts anything in actor space to world space
Transform Actor::getActorTransform()
{
	Transform transform;
	if (root)
	{
		transform = root->t;
	}
	return transform;
}

/// Set a transform for the root
void Actor::setActorTransform(const Transform& transform)
{
	if (root)
	{
		root->t = transform;
	}
}

/// Get the position of this Actor
vec3 Actor::getActorLocation()
{
	return getActorTransform().position;
}

/// Set the location of this Actor directly
void Actor::setActorLocation(Vector3D location)
{
	if (root)
	{
		root->t.position = location;
	}
}

NEPHILIM_NS_END