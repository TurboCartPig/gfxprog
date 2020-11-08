#pragma once

#include <GL/glew.h>
#include <chrono>
#include <glm/glm.hpp>
#include <glove/Texture.h>
#include <initializer_list>
#include <memory>
#include <vector>

using std::chrono::milliseconds;

/**
 * Animation consisting of keyframes designating sprites in a spritesheet.
 */
class SpriteSheetAnimation {
	friend class AnimatedSpriteSheet;

  public:
	/**
	 * Create an empty animation.
	 * @note Must add keyframes via "pushKeyFrame".
	 */
	SpriteSheetAnimation() = default;

	/**
	 * Create new spritesheet animation from a list of keyframes.
	 * @param keyframes List of keyframes that define the animation.
	 */
	SpriteSheetAnimation(std::initializer_list<glm::ivec4> keyframes);

	/**
	 * Add another key frame to the animation.
	 * @param keyFrame
	 */
	void pushKeyFrame(glm::ivec4 keyFrame) { m_key_frames.push_back(keyFrame); }

  private:
	std::vector<glm::ivec4> m_key_frames;
};

/**
 * An abstraction for animating sprites from a spritesheet / texture atlas using
 * keyframes.
 */
class AnimatedSpriteSheet {
  public:
	/**
	 * Construct a new animated spritesheet.
	 *
	 * # Sprite sheet
	 * "spritesheet" uses "GL_TEXTURE_RECTANGLE" as opposed to "GL_TEXTURE_2D"
	 * in order to sample the texture using NON-normalized coordinates. The
	 * alternative would be to use "GL_TEXTURE_2D", and query the size of the
	 * texture from the shader using "textureSize". It is not obvious what the
	 * benefit of this approach would be.
	 *
	 * # Animation
	 * You must call "playAnimation" before first call to "getUniform" or
	 * "update".
	 *
	 * @param frameTime How long should each frame be displayed?
	 * @param animation Animation describing keyframes to sample from the
	 * spritesheet.
	 * @param spritesheet Texture of type "GL_TEXTURE_RECTANGLE" containing all
	 * the sprites involved in the animation.
	 */
	AnimatedSpriteSheet(milliseconds             frameTime,
	                    std::shared_ptr<Texture> spritesheet);

	/**
	 * Drive to animation forwards and loop if necessary.
	 * @param dt
	 */
	void update(std::chrono::duration<double> dt);

	/**
	 * Play a new animation, cancelling the current one.
	 * @param animation The new animation to play.
	 */
	void playAnimation(SpriteSheetAnimation animation);

	/**
	 * Play or pause the animation based on "shouldPlay".
	 * @param shouldPlay Should the animation play?
	 */
	void play(bool shouldPlay = true) { m_paused = !shouldPlay; }

	/**
	 * Set whether the animation should loop.
	 * @param shouldLoop Should the animation loop?
	 */
	void loop(bool shouldLoop = true) { m_looped = shouldLoop; }

	/**
	 * Get the uniform data to send to the shaders.
	 * @return Uniform data to be sent.
	 */
	[[nodiscard]] auto getUniform() const {
		assert(!m_animation.m_key_frames.empty());
		return m_animation.m_key_frames[m_current_key_frame];
	}

	/**
	 * Get a reference to the internally used texture that contains the sprite
	 * sheet.
	 * @return Sprite sheet texture.
	 */
	[[nodiscard]] const auto &getTexture() const { return m_sprite_sheet; }

  private:
	bool                          m_paused            = false;
	bool                          m_looped            = true;
	size_t                        m_current_key_frame = 0;
	std::chrono::duration<double> m_frame_time;
	std::chrono::duration<double> m_from_last_key_frame = milliseconds(0);
	SpriteSheetAnimation          m_animation;

	// Not used per say, but useful for keeping track of
	// lifetime of the texture
	std::shared_ptr<Texture> m_sprite_sheet;
};
