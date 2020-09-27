/* This should only be defined once.
 * @see [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) for
 * details */
#define STB_IMAGE_IMPLEMENTATION

#include <glove/Texture.h>
#include <stb_image.h>

Texture::Texture(const std::string &path, GLuint type) {
	// Vertical direction of images
	stbi_set_flip_vertically_on_load(false);

	// Load image from file
	int  w, h, n;
	auto data = stbi_load(path.c_str(), &w, &h, &n, 0);
	assert(data != nullptr);

	// Determines file format based on number of 8 bit components reported by
	// stbi
	GLuint file_format;
	switch (n) {
		case 1: file_format = GL_R8; break;
		case 2: file_format = GL_RG8; break;
		case 3: file_format = GL_RGB8; break;
		case 4: file_format = GL_RGBA8; break;
		default: assert(false);
	}

	// Determines the corresponding internal image format
	GLuint image_format;
	switch (file_format) {
		case GL_R8: image_format = GL_RED; break;
		case GL_RG8: image_format = GL_RG; break;
		case GL_RGB8: image_format = GL_RGB; break;
		case GL_RGBA8: image_format = GL_RGBA; break;
	}

	// Create the texture
	glGenTextures(1, &m_handle);
	glBindTexture(GL_TEXTURE_2D, m_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(type, 0, file_format, w, h, 0, image_format, GL_UNSIGNED_BYTE,
	             data);

	stbi_image_free(data);
}

Texture::~Texture() { glDeleteTextures(1, &m_handle); }

void Texture::bindToSlot(unsigned int slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_handle);
}
