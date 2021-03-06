#ifndef NephilimPluginPhysicsPhysX_h__
#define NephilimPluginPhysicsPhysX_h__

#include <Nephilim/World/PhysicsSystem.h>
#include <Nephilim/World/Actor.h>

#include <Nephilim/Foundation/Logging.h>
#include <Nephilim/Foundation/Vector.h>
#include <Nephilim/Graphics/VertexArray.h>
#include <Nephilim/Graphics/IndexArray.h>
using namespace nx;

namespace physx
{
	class PxPhysics;
	class PxScene;
	class PxControllerManager;
}

/**
	\class PhysicsSystemPhysX
	\brief Implementation of the PhysX physics library

	This class implements the famous PhysX library into the engine,
	allowing it to simulate our scene in interactive ways.

	It is possible to include this class into the project files and use it directly,
	as well as leave it as a plugin and use it that way.
*/
class PhysicsSystemPhysX : public PhysicsSystem
{
public:


	physx::PxPhysics* mPhysics;
	physx::PxScene* mScene;
	physx::PxControllerManager* manager;
public:

	/// Initialize the scene already
	PhysicsSystemPhysX();

	/// Let's check if actors are okay
	void syncActors();

	void TestCharacterMove(Vector3D displacement, Actor* a);

	/// Get the name of this implementation (PhysX)
	virtual String getName();

	/// Step the simulation forward
	virtual void update(const Time& deltaTime);
};

#endif // NephilimPluginPhysicsPhysX_h__
