#ifndef RenderTexture_h__
#define RenderTexture_h__

#include <Nephilim/Platform.h>
#include <Nephilim/Graphics/Framebuffer.h>
#include <Nephilim/Graphics/Texture2D.h>

NEPHILIM_NS_BEGIN

class NEPHILIM_API RenderTexture : public Framebuffer
{
public:

	void create(int width, int height);

	Texture2D m_texture; ///< The target texture
};

NEPHILIM_NS_END
#endif // RenderTexture_h__
