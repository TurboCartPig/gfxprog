#include <glove/Framebuffer.h>

Framebuffer::Framebuffer() : m_has_depth_stencil(false) {
	glGenFramebuffers(1, &m_fbo);
}

Framebuffer::Framebuffer(GLuint fbo)
    : m_fbo(fbo), m_has_depth_stencil(true), m_color_attachments(1) {}

Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &m_fbo); }

auto Framebuffer::defaultFramebuffer() -> std::unique_ptr<Framebuffer> {
	return std::unique_ptr<Framebuffer>(new Framebuffer(0));
}

auto Framebuffer::valid() const -> bool {
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void Framebuffer::bind(GLenum target) const {
	glBindFramebuffer(target, m_fbo);
}

void Framebuffer::addAttachment(AttachmentType type, glm::ivec2 dimensions) {
	// Create texture
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	GLenum attachment;
	GLint  internal_format;
	GLenum sample_format;
	GLenum pixel_format;

	switch (type) {
		case AttachmentType::Color:
			attachment      = GL_COLOR_ATTACHMENT0 + m_color_attachments++;
			internal_format = GL_RGB8;
			sample_format   = GL_RGB;
			pixel_format    = GL_UNSIGNED_BYTE;
			break;
		case AttachmentType::DepthStencil:
			m_has_depth_stencil = true;
			attachment          = GL_DEPTH_STENCIL_ATTACHMENT;
			internal_format     = GL_DEPTH24_STENCIL8;
			sample_format       = GL_DEPTH_STENCIL;
			pixel_format        = GL_UNSIGNED_INT_24_8;
			break;
	}

	// Allocate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, dimensions.x, dimensions.y,
	             0, sample_format, pixel_format, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex, 0);
}

void Framebuffer::clear() const {
	glClear((m_color_attachments > 0 ? GL_COLOR_BUFFER_BIT : 0) |
	        (m_has_depth_stencil ? GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT
	                             : 0));
}

void Framebuffer::blit(const Framebuffer *source, glm::ivec4 source_extents,
                       glm::ivec4 destination_extents) {
	source->bind(GL_READ_FRAMEBUFFER);
	this->bind(GL_DRAW_FRAMEBUFFER);
	glBlitFramebuffer(source_extents.x, source_extents.y, source_extents.z,
	                  source_extents.w, destination_extents.x,
	                  destination_extents.y, destination_extents.z,
	                  destination_extents.w, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
