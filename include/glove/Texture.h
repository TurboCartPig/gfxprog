#pragma once

/**
 * Texture
 *
 * # Loading from file
 * - Crashes if path is invalid
 * - Crashes if file fails to load for any reason
 * - Assumes the texture is 2D
 * - Assumes pixel components are 8 bit and there's [1,4] of them
 * @see [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)
 *
 * # Binding
 * - Needs to be bound manually with "bindToSlot"
 *
 * # Slots
 * - Which slot to bind to is specified when binding by calling "bindToSlot"
 */
class Texture {
  public:
	/**
	 * Create a new texture from file.
	 *
	 * @param path Path to texture file to load
	 */
	explicit Texture(const std::string &path);

	Texture(const Texture &other) = delete;

	Texture(const Texture &&other) = delete;

	Texture operator=(const Texture &other) = delete;

	Texture operator=(const Texture &&other) = delete;

	~Texture();

	/**
	 * Bind this texture to the given texture unit slot.
	 * You will also need to assign the same slot to a uniform sampler
	 *
	 * @param slot Texture unit slot to bind this texture to
	 */
	void bindToSlot(unsigned int slot);

  private:
	GLuint m_handle;
};