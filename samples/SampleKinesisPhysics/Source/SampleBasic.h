#ifndef PortabilityTest_h__
#define PortabilityTest_h__

#include <Nephilim/GameCore.h>
#include <Nephilim/Renderer.h>
#include <Nephilim/Shader.h>
#include <Nephilim/ShaderGen.h>
#include <Nephilim/Package.h>
#include <Nephilim/Framebuffer.h>
using namespace pE;

class SampleUI : public GameCore
{
public:
	void onCreate();
	void onRender();
	void onEvent(Event &event);
	void onUpdate(Time time);

	Shader defaultShader;

	Framebuffer fbo;


};

#endif // PortabilityTest_h__
