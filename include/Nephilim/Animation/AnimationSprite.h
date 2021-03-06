#ifndef PARABOLA_ANIMATIONSPRITE_H
#define PARABOLA_ANIMATIONSPRITE_H

#include <Nephilim/Platform.h>
#include "AnimationInterface.h"
#include "Sprite.h"
#include "Rect.h"
#include "Texture.h"

NEPHILIM_NS_BEGIN

/**
	\ingroup Animation
	\class AnimationSprite
	\brief Represents an animation of image frames in a set of sprites

	Can animate a set of sprites at the same time and will set them with the
	right texture sub rect along time, to simulate movement.
*/
class NEPHILIM_API AnimationSprite : public AnimationInterface{
public:
	class AnimationFrame;
	/// Creates a default sprite animation
	AnimationSprite();

	/// Add a frame
	void addFrame(GLTexture2D* texture, FloatRect rect, float duration);
	/// Add a new frame to the animation
	void addFrame2(AnimationFrame &frame);

	/// Get the number of frames in this animation
	int getFrameCount();

	/// Applies one of the frames to all sprites
	void applyFrame(int index);

	/// Overload of AnimationInterface. Plays the animation.
	void play();

	/// Overload of AnimationInterface. Stops the animation.
	void stop();

	/// Reset the animation to the beginning
	void reset();

	/// Update the sprites over time. Essentially means changing its texture when needed!
	void update(float elapsedTime);

	/**
		Builds the frames from a sprite sheet, in this case only a one-line sprite sheet.
		Just specify the width of each frame, and how many you want, and from what texture, and it will do the work.
	*/
	int buildFromHorizontalSheet(float frameWidth, int frameCount, GLTexture2D* texture);

	sigc::signal<void, GLTexture2D*, FloatRect> onFrameChange;

	class NEPHILIM_API AnimationFrame{
	public:
		AnimationFrame();
		AnimationFrame(FloatRect &box, GLTexture2D *texture, double time);

		/// Define the texture wanted, if the rect is still undefined (positioned at -1 -1), it becomes the full texture rect
		void setTexture(GLTexture2D* texture);

		double time;
		FloatRect myRect;

		friend class AnimationSprite;
	private:
		GLTexture2D *myTexture;
	};

private:
	std::vector<AnimationFrame> frames;
	std::vector<Sprite*> animationSprites;
	double currentTime;
	/// Sum of frame durations
	double totalDuration;
	double localTime;
	int frameIndex;
};

NEPHILIM_NS_END
#endif